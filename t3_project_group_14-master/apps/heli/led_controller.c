/**
 * @file led_controller.c
 * @author Hank Wu              34748017
 * @brief 
 * @version 0.1
 * @date 2021-07-14
 *
 */
//        >(')____,  >(')____,  >(')____,  >(')____,  >(') ___,
//         (` =~~/    (` =~~/    (` =~~/    (` =~~/    (` =~~/
//    ~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~
/************************************************************************/
/*                                                                      */
/*                      led_controller.c                                */
/*                                                                      */
/************************************************************************/

// RTOS Library includes
#include <FreeRTOS.h>
#include <task.h>

// Application Library includes
#include "led_controller.h"
#include "led.h"
#include "config/target.h"
#include "config/led_cfg.h"

static led_t *leds[LED_NUM];

/**
 * @brief Initialise leds
 * 
 */
void leds_initialise(void)
{
    leds[LED_RED] = led_init(&led_r_cfg);
    leds[LED_GREEN] = led_init(&led_g_cfg);
    leds[LED_BLUE] = led_init(&led_b_cfg);

    leds[LED_D1] = led_init(&led_d1_cfg);
    leds[LED_D2] = led_init(&led_d2_cfg);
    leds[LED_D3] = led_init(&led_d3_cfg);
    leds[LED_D4] = led_init(&led_d4_cfg);
}

/**
 * @brief 
 * 
 * @param led 
 */
void leds_on(int led)
{
    if (led < LED_NUM)
        led_on(leds[led]);
}

/**
 * @brief 
 * 
 * @param led 
 */
void leds_off(int led)
{
    if (led < LED_NUM)
        led_off(leds[led]);
}

/**
 * @brief 
 * 
 * @param led 
 */
void leds_toggle(int led)
{
    if (led < LED_NUM)
        led_toggle(leds[led]);
}

/**
 * @brief Blink task
 * 
 * @param pvParameters 
 */
void blink_task(void *pvParameters)
{
    (void)pvParameters; // unused

    TickType_t wake_time = xTaskGetTickCount();

    while (true)
    {

        leds_toggle(LED_D2);
        vTaskDelayUntil(&wake_time, 2000);
    }
}
