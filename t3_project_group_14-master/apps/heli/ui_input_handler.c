/**
 * @file ui_input_handler.h
 * @author Zach Preston         81883956
 * @author Hank Wu              34748017
 * @author Jason UI             27674749
 * @author Randipa Gunathilake  66941908
 * @brief This Module monitors the user input and sent the data via a queue.
 *
 *  The module would only sent out data when the input status (i.g. button
 * status) changes. For the input element that does not change, a
 * <ELEMENT_TYPE>_NO_CHANGE would be sent instead.
 *  For example, if only button_up is pressed:
 *  The value in packet.button_up would equals to BUTTON_PRESSED, with the
 * packet.button_down equals to BUTTON_NO_CHANGE.
 *  NOTE: The initilise function i.e. ui_initialise() needed to be called
 *  before passing ui_task to RTOS. Otherwise, ui_task will return.
 * @version 0.1
 * @date 2021-07-14
 *
 */
//        >(')____,  >(')____,  >(')____,  >(')____,  >(') ___,
//         (` =~~/    (` =~~/    (` =~~/    (` =~~/    (` =~~/
//    ~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~
/************************************************************************/
/*                                                                      */
/*                      ui_input_handler.h                              */
/*                                                                      */
/************************************************************************/

// FreeRTOS Library includes
#include <FreeRTOS.h>

// Application Library includes
#include "ui_input_handler.h"
#include "led_controller.h"
#include "adcd.h"
#include "config/button_cfg.h"
#include "config/sw_cfg.h"

enum
{
    BUTTON_UP,
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_DOWN,
    BUTTON_NUM
};

enum
{
    SW_RIGHT,
    SW_LEFT,
    SW_NUM
};

//static variables for holding stuff.
static QueueHandle_t user_input_queue;
static button_t *buttons[BUTTON_NUM];
static switch_t *switches[SW_NUM];
static adc_t *pot;
static bool initilized = false;

static void ui_adc_isr(void)
{
    adc_isr(pot);
}

/**
 * @brief update buttons.(polling)
 * 
 */
static void ui_update_buttons(void)
{
    //loop through all buttons.
    for (uint8_t i = 0; i < BUTTON_NUM; i++)
        button_update(buttons[i]);
}

/**
 * @brief update switches. (polling)
 * 
 */
static void ui_update_sw(void)
{
    //loop through all switches.
    for (uint8_t i = 0; i < SW_NUM; i++)
        switch_update(switches[i]);
}
/**
 * @brief initSwitchs: initialise GPIO of SW1 and SW2
 * 
 */
static void ui_init_switches(void)
{
    switches[SW_RIGHT] = switch_init(&sw_right_cfg);
    switches[SW_LEFT] = switch_init(&sw_left_cfg);
}

/**
 * @brief initilise buttons
 * 
 */
static void ui_init_buttons(void)
{
    buttons[BUTTON_UP] = button_init(&button_up_cfg);
    buttons[BUTTON_DOWN] = button_init(&button_down_cfg);
    buttons[BUTTON_RIGHT] = button_init(&button_right_cfg);
    buttons[BUTTON_LEFT] = button_init(&button_left_cfg);
}

/**
 * @brief initialise adc for potentiometer.
 * 
 */
static void ui_init_adc(void)
{
    adc_cfg_t cfg = {
        .periph = SYSCTL_PERIPH_ADC0,
        .priority = 0,
        .base = ADC0_BASE,
        .sequencer = 2,
        .channel = ADC_CTL_CH0,
        .buffer_size = 5,
        .adc_isr = ui_adc_isr};
    pot = adc_init(&cfg);
}

/**
 * @brief initilise user interface module. This function must be called before passing ui_task to RTOS.
 * 
 */
void ui_initialise(void)
{
    ui_init_buttons();
    ui_init_switches();
    ui_init_adc();

    //init queue.
    user_input_queue = xQueueCreate(UI_QUEUE_LENGTH, sizeof(ui_input_packet_t));
    initilized = true;
}

/**
 * @brief RTOS task for monitoring user input.
 * 
 * @param args 
 */
void ui_task(void *args)
{
    (void)args; //unused argument.

    static int32_t pot_val;
    TickType_t wake_time = xTaskGetTickCount();
    //check if initilised.
    while (initilized)
    {
        //poll the buttons and switches first.
        ui_update_buttons();
        ui_update_sw();
        adc_read_trigger(pot);

        bool send_command = false; //flag for sending command.

        button_state_t buttons_state[BUTTON_NUM];
        switch_state_t sw_state[SW_NUM];

        //read the buttons' status.
        for (uint8_t i = 0; i < BUTTON_NUM; i++)
        {
            buttons_state[i] = button_read(buttons[i]);
            if (buttons_state[i] != BUTTON_NO_CHANGE) //send data if button status changed,
                send_command = true;
        }

        //read the switches' status.
        for (uint8_t i = 0; i < SW_NUM; i++)
        {
            sw_state[i] = switch_read(switches[i]);
            if (sw_state[i] != SWITCH_NO_CHANGE) //send data if switch status changed,
                send_command = true;
        }

        //read the potentiometer value.
        int32_t new_pot_val = adc_read_buffer_avg(pot) * HEADING_MAX_VAL / ADC_MAX_VAL;
        if (new_pot_val != pot_val)
            send_command = true;

        pot_val = new_pot_val;

        //create packet.
        ui_input_packet_t packet = {
            .button_up = buttons_state[BUTTON_UP],
            .button_down = buttons_state[BUTTON_DOWN],
            .button_right = buttons_state[BUTTON_RIGHT],
            .button_left = buttons_state[BUTTON_LEFT],
            .sw_right = sw_state[SW_RIGHT],
            .sw_left = sw_state[SW_LEFT],
            .potentiometer = new_pot_val};

        //only sent command if user input has chagned.
        if (send_command)
        {
            if (xQueueSend(user_input_queue, (void *)&packet, 10) != pdTRUE)
                leds_on(LED_RED);
        }

        vTaskDelayUntil(&wake_time, UI_TASK_PERIOD);
    }
}

/**
 * @brief Get the queue of the user input data.
 * 
 * @return QueueHandle_t 
 */
QueueHandle_t ui_get_queue(void)
{
    return user_input_queue;
}