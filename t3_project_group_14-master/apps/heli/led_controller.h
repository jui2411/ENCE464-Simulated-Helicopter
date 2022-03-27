/**
 * @file led_controller.h
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
/*                      led_controller.h                                */
/*                                                                      */
/************************************************************************/

#ifndef LEDCONTROLLER_H_
#define LEDCONTROLLER_H_

enum {LED_RED, LED_BLUE, LED_GREEN, LED_D1, LED_D2, LED_D3, LED_D4, LED_NUM};

/**
 * @brief 
 * 
 * @param pvParameters 
 */
void blink_task(void* pvParameters);

/**
 * @brief 
 * 
 */
void leds_initialise(void);

/**
 * @brief 
 * 
 * @param led 
 */
void leds_on(int led);

/**
 * @brief 
 * 
 * @param led 
 */
void leds_off(int led);

/**
 * @brief 
 * 
 * @param led 
 */
void leds_toggle(int led);

#endif