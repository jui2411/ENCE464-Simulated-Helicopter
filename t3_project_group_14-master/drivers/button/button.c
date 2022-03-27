/**
 * @file button.c
 * @author Zach Preston         81883956
 * @author Hank Wu              34748017
 * @author Jason UI             27674749
 * @author Randipa Gunathilake  66941908
 * @brief Driver for handling buttons
 * @version 0.1
 * @date 2021-07-14
 *
 */
//        >(')____,  >(')____,  >(')____,  >(')____,  >(') ___,
//         (` =~~/    (` =~~/    (` =~~/    (` =~~/    (` =~~/
//    ~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~
/************************************************************************/
/*                                                                      */
/*                              button.c                                */
/*                                                                      */
/************************************************************************/

// Standard library includes
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// Tiva library includes
#include <inc/hw_memmap.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include "inc/tm4c123gh6pm.h"

// RTOS Library includes
#include <FreeRTOS.h>

// Application Library includes
#include "button.h"

typedef struct button_s
{
    //settings.
    const uint32_t button_port;
    const uint8_t button_pin;
    const bool button_active_high;
    const uint32_t activate_tick;
    const uint32_t long_press_tick;

    bool button_status;      //holding the previous button status for program logic.
    uint32_t debounce_ticks; //ticks for program logic.
    uint32_t ticks;
    button_state_t button_reading; //the state of button.
} button_t;

//initialize hardware.
static void button_init_hw(button_cfg_t *cfg)
{
    SysCtlPeripheralEnable(cfg->button_periph);
    // Note that PF0 is one of a handful of GPIO pins that need to be
    // "unlocked" before they can be reconfigured.  This also requires
    //      #include "inc/tm4c123gh6pm.h"

    if (cfg->button_port == GPIO_PORTF_BASE)
    {
        GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
        GPIO_PORTF_CR_R |= GPIO_PIN_0; //PF0 unlocked
        GPIO_PORTF_LOCK_R = GPIO_LOCK_M;
    }

    GPIOPinTypeGPIOInput(cfg->button_port, cfg->button_pin);
    GPIOPadConfigSet(cfg->button_port, cfg->button_pin, GPIO_STRENGTH_2MA, cfg->pin_type);
}

/*Initialize button. pass in a button_cfg_t for setup.
This will allocate a button struct in the heap memory.*/
button_t *button_init(button_cfg_t *cfg)
{
    //init hardware.
    button_init_hw(cfg);

    // malloc a button struct in heap.
    button_t *button_pt = pvPortMalloc(sizeof(button_t));

    bool active_high = 0;

    if(cfg->pin_type == GPIO_PIN_TYPE_STD_WPD){
        active_high = true;
    }else{
        active_high = false;
    }

    //initialize a button struct in stack.
    button_t button_init = {
        .button_port = cfg->button_port,
        .button_pin = cfg->button_pin,
        .button_active_high = active_high,
        .activate_tick = cfg->activate_tick,
        .long_press_tick = cfg->long_press_tick,
        .button_status = 0,
        .debounce_ticks = 0,
        .ticks = 0};

    //copy the content in stack to heap.
    memcpy(button_pt, &button_init, sizeof(button_t));

    return button_pt;
}

/*Update button.*/
void button_update(button_t *button)
{

    // Read the pins; true means HIGH, false means LOW
    bool button_read = (GPIOPinRead(button->button_port, button->button_pin) == button->button_pin);

    //if the button status is not equal to the reading, increment the debounce ticks.
    if (button->button_status != button_read)
        button->debounce_ticks++;
    else if (button->button_status == button->button_active_high)
        button->ticks++;
    else
        button->ticks = 0;
        

    //check debouce tick.
    if (button->debounce_ticks > button->activate_tick)
    {
        button->debounce_ticks = 0;
        button->button_status = button_read;

        //check active high or low to determine pused or released.
        if (button->button_status == button->button_active_high)
            button->button_reading = BUTTON_PUSHED;
        else
            button->button_reading = BUTTON_RELEASED;
    }

    //check ticks.
    if (button->ticks == button->long_press_tick) //needs to be equal so that BUTTON HOLD only been sent once.
    {
        if (button->button_status == button->button_active_high)
            button->button_reading = BUTTON_HOLD;
    }
}

/*read button state.*/
button_state_t button_read(button_t *button)
{
    button_state_t state = button->button_reading;
    //once read, reset the reading to NO_CHANGE.
    if (state != BUTTON_NO_CHANGE)
        button->button_reading = BUTTON_NO_CHANGE;

    return state;
}

/*de-initilize button.*/
int button_deinit(button_t *button)
{
    vPortFree(button);
    return 1;
}