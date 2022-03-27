/**
 * @file switch.c
 * @author Zach Preston         81883956
 * @author Hank Wu              34748017
 * @author Jason UI             27674749
 * @author Randipa Gunathilake  66941908
 * @brief Driver for handling switches
 * @version 0.1
 * @date 2021-08-08
 *
 */
//        >(')____,  >(')____,  >(')____,  >(')____,  >(') ___,
//         (` =~~/    (` =~~/    (` =~~/    (` =~~/    (` =~~/
//    ~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~
/************************************************************************/
/*                                                                      */
/*                              switch.c                                */
/*                                                                      */
/************************************************************************/
// Standard library includes
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// Tiva library includes
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>

// RTOS Library includes
#include <FreeRTOS.h>

// Application Library includes
#include <switch.h>

#define SWITCH_GPIO_STRENGTH GPIO_STRENGTH_2MA
#define SWITCH_GPIO_TYPE GPIO_PIN_TYPE_STD

typedef struct switch_s
{
    //pins
    const uint32_t port;
    const uint8_t pin;
    const uint32_t activate_tick;

    //internal logic variables
    uint32_t tick;
    bool state;
    switch_state_t reading;
} switch_t;

static void switch_init_hw(switch_cfg_t *cfg)
{
    // LSW slide switch
    SysCtlPeripheralEnable(cfg->periph);
    GPIOPinTypeGPIOInput(cfg->port, cfg->pin);
    GPIOPadConfigSet(cfg->port, cfg->pin, SWITCH_GPIO_STRENGTH, SWITCH_GPIO_TYPE);
}

/**
 * @brief initialise switch
 * 
 * @param cfg 
 * @return switch_t* 
 */
switch_t *switch_init(switch_cfg_t *cfg)
{
    //init hardware.
    switch_init_hw(cfg);

    // malloc a sw struct in heap.
    switch_t *sw_pt = pvPortMalloc(sizeof(switch_t));

    //initialize a sw struct in stack.
    switch_t sw_init = {
        .port = cfg->port,
        .pin = cfg->pin,
        .activate_tick = cfg->activate_tick};

    //copy the content in stack to heap.
    memcpy(sw_pt, &sw_init, sizeof(switch_t));

    return sw_pt;
}

/**
 * @brief update switch.
 * 
 * @param sw 
 */
void switch_update(switch_t *sw)
{
    // Read the pins; true means HIGH, false means LOW
    bool sw_new_state = (GPIOPinRead(sw->port, sw->pin) == sw->pin);

    // Iterate ticks.
    if (sw->state != sw_new_state)
        sw->tick++;
    else
        sw->tick = 0;

    //check ticks
    if (sw->tick >= sw->activate_tick)
    {
        sw->state = sw_new_state;
        sw->reading = sw_new_state;
        sw->tick = 0;
    }
}

/**
 * @brief read switch. return value change when state change, otherwise return SWITCH_NO_CHANGE.
 * 
 * @param sw 
 * @return switch_state_t 
 */
switch_state_t switch_read(switch_t *sw)
{
    switch_state_t reading = sw->reading;
    //once read, reset the reading to NO_CHANGE.
    if (reading != SWITCH_NO_CHANGE)
        sw->reading = SWITCH_NO_CHANGE;

    return reading;
}

/**
 * @brief read switch state, either up or down.
 * 
 * @param sw 
 * @return bool 
 */
switch_state_t switch_read_state(switch_t *sw)
{
    return sw->state;
}

/**
 * @brief free the switch struct in the heap memory.
 * 
 */
int switch_free(switch_t *sw)
{
    vPortFree(sw);
    return 1;
}