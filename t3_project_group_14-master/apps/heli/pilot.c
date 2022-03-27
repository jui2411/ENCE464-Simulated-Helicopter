/**
 * @file pilot.c
 * @author Hank Wu              34748017
 * @brief Pilot stream data via a queue from user interface module and
 * interpret it into a heading goal and altitude goal
 *
 * @version 0.1
 * @date 2021-07-18
 *
 */
//        >(')____,  >(')____,  >(')____,  >(')____,  >(') ___,
//         (` =~~/    (` =~~/    (` =~~/    (` =~~/    (` =~~/
//    ~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~
/************************************************************************/
/*                                                                      */
/*                               pilot.c                                */
/*                                                                      */
/************************************************************************/

// RTOS Library includes
#include <FreeRTOS.h>

// Application Library includes
#include "ui_input_handler.h"
#include "led_controller.h"
#include "uartd.h"

#define PILOT_TASK_PERIOD 10
#define PILOT_DELAY_IF_EMPTY_QUEUE 5

//define changing speed when long press.
#define HEADING_CHANGING_SPEED UI_LONG_PRESS_HEADING_CHANGING_SPEED
#define ALTITUDE_CHANGING_SPEED UI_LONG_PRESS_ALTITUDE_CHANGING_SPEED

//static variables.
static bool initilised = false;
static QueueHandle_t user_input_queue;
static uint16_t heading_goal = 0;
static uint16_t altitude_goal = 0;
static uart_t *uart;

typedef enum
{
    NO_ACTION,

    INCREASE_ALTITUDE,
    INCREASE_ALTITUDE_CONTINUOUS,
    INCREASE_ALTITUDE_STOP,

    DECREASE_ALTITUDE,
    DECREASE_ALTITUDE_CONTINUOUS,
    DECREASE_ALTITUDE_STOP,

    INCREASE_HEADING,
    INCREASE_HEADING_CONTINUOUS,
    INCREASE_HEADING_STOP,

    DECREASE_HEADING,
    DECREASE_HEADING_CONTINUOUS,
    DECREASE_HEADING_STOP,

    INPUT_SOURCE_BUTTON,
    INPUT_SOURCE_POT
} pilot_action_t;

void pilot_initialise(void)
{
    user_input_queue = ui_get_queue();
    uart = uart0_init();
    initilised = true;
}

uint16_t pilot_get_heading_goal(void)
{
    return heading_goal;
}

uint16_t pilot_get_altitude_goal(void)
{
    return altitude_goal;
}
/**
 * @brief translate the button/switch changes to action. Assuming one change per packet.
 * 
 * @param packet 
 * @return pilot_action_t 
 */
static pilot_action_t pilot_read_packet(ui_input_packet_t *packet)
{
    if (packet->button_up == BUTTON_PUSHED)
        return INCREASE_ALTITUDE;
    if (packet->button_down == BUTTON_PUSHED)
        return DECREASE_ALTITUDE;
    if (packet->button_left == BUTTON_PUSHED)
        return DECREASE_HEADING;
    if (packet->button_right == BUTTON_PUSHED)
        return INCREASE_HEADING;
    if (packet->button_up == BUTTON_HOLD)
        return INCREASE_ALTITUDE_CONTINUOUS;
    if (packet->button_up == BUTTON_RELEASED)
        return INCREASE_ALTITUDE_STOP;
    if (packet->button_down == BUTTON_HOLD)
        return DECREASE_ALTITUDE_CONTINUOUS;
    if (packet->button_down == BUTTON_RELEASED)
        return DECREASE_ALTITUDE_STOP;
    if (packet->button_left == BUTTON_HOLD)
        return DECREASE_HEADING_CONTINUOUS;
    if (packet->button_left == BUTTON_RELEASED)
        return DECREASE_HEADING_STOP;
    if (packet->button_right == BUTTON_HOLD)
        return INCREASE_HEADING_CONTINUOUS;
    if (packet->button_right == BUTTON_RELEASED)
        return INCREASE_HEADING_STOP;
    if (packet->sw_left == SWITCH_UP)
        return INPUT_SOURCE_BUTTON;
    if (packet->sw_left == SWITCH_DOWN)
        return INPUT_SOURCE_POT;
    return NO_ACTION;
}

/**
 * @brief execute single action. If potentiometer reading does not metter, pass in -1.
 * 
 * @param action 
 * @param pot_new 
 */
static void pilot_execute_action(pilot_action_t action, int pot_new)
{
    static uint32_t pot_val;
    static int input_source = INPUT_SOURCE_BUTTON;

    switch (action)
    {
    case DECREASE_HEADING:
        if (heading_goal > HEADING_MIN_VAL)
            heading_goal--;
        else
            heading_goal = HEADING_MAX_VAL;
        break;
    case INCREASE_HEADING:
        if (heading_goal < HEADING_MAX_VAL)
            heading_goal++;
        else
            heading_goal = HEADING_MIN_VAL;
        break;
    case INCREASE_ALTITUDE:
        if (altitude_goal < ALTITUDE_MAX_VAL)
            altitude_goal++;
        break;
    case DECREASE_ALTITUDE:
        if (altitude_goal > ALTITUDE_MIN_VAL)
            altitude_goal--;
        break;
    case INPUT_SOURCE_BUTTON:
        input_source = INPUT_SOURCE_BUTTON;
        break;
    case INPUT_SOURCE_POT:
        input_source = INPUT_SOURCE_POT;
        break;
    default:
        break;
    }

    //if input soruce is potentiometer.
    if (input_source == INPUT_SOURCE_POT && pot_new != -1)
    {
        int pot_diff = pot_new - pot_val;
        int goal = heading_goal + pot_diff;
        if (goal > HEADING_MAX_VAL)
            heading_goal = goal - HEADING_MAX_VAL;
        else if (goal < HEADING_MIN_VAL)
            heading_goal = goal + HEADING_MAX_VAL;
        else
            heading_goal = goal;
    }
    //update the pot_val.
    if (pot_new != -1)
        pot_val = pot_new;
}

/**
 * @brief execute continuous action. If potentiometer reading does not metter, pass in -1.
 * 
 * @param action 
 * @param pot_new 
 */
static void pilot_execute_continuous_action(pilot_action_t action, int pot_new)
{
    static bool i_alt_flag = false;
    static bool d_alt_flag = false;
    static bool i_hed_flag = false;
    static bool d_hed_flag = false;
    static uint32_t alt_tick = 0;
    static uint32_t hed_tick = 0;

    //flagging.
    switch (action)
    {
    case INCREASE_ALTITUDE_CONTINUOUS:
        i_alt_flag = true;
        break;
    case INCREASE_ALTITUDE_STOP:
        i_alt_flag = false;
        break;
    case DECREASE_ALTITUDE_CONTINUOUS:
        d_alt_flag = true;
        break;
    case DECREASE_ALTITUDE_STOP:
        d_alt_flag = false;
        break;
    case INCREASE_HEADING_CONTINUOUS:
        i_hed_flag = true;
        break;
    case DECREASE_HEADING_CONTINUOUS:
        d_hed_flag = true;
        break;
    case INCREASE_HEADING_STOP:
        i_hed_flag = false;
        break;
    case DECREASE_HEADING_STOP:
        d_hed_flag = false;
        break;
    default:
        break;
    }

    if (alt_tick > ALTITUDE_CHANGING_SPEED)
    {
        if (i_alt_flag)
            pilot_execute_action(INCREASE_ALTITUDE, pot_new);
        if (d_alt_flag)
            pilot_execute_action(DECREASE_ALTITUDE, pot_new);
        alt_tick = 0;
    }

    if (hed_tick > HEADING_CHANGING_SPEED)
    {
        if (i_hed_flag)
            pilot_execute_action(INCREASE_HEADING, pot_new);
        if (d_hed_flag)
            pilot_execute_action(DECREASE_HEADING, pot_new);
        hed_tick = 0;
    }

    //increament ticks.
    alt_tick++;
    hed_tick++;
}


/**
 * @brief 
 * 
 * @param pvParameters 
 */
void pilot_task(void *pvParameters)
{
    (void)pvParameters;

    TickType_t wake_time = xTaskGetTickCount();

    pilot_action_t action = NO_ACTION;
    while (initilised)
    {
        ui_input_packet_t packet = {0};
        if (xQueueReceive(user_input_queue, (void *)&packet, PILOT_DELAY_IF_EMPTY_QUEUE) == pdTRUE)
        {
            action = pilot_read_packet(&packet);
            pilot_execute_action(action, packet.potentiometer);
            pilot_execute_continuous_action(action, packet.potentiometer);
            char* str_buf = "recieved packet!\n";
            uart_puts(uart, str_buf);
            leds_on(LED_D1);
        }
        else
        {
            pilot_execute_continuous_action(action, -1);
            action = NO_ACTION;
            leds_off(LED_D1);
        }
        
        vTaskDelayUntil(&wake_time, PILOT_TASK_PERIOD);
    }
}