/**
 * @file led.c
 * @author Zach Preston         81883956
 * @author Hank Wu              34748017
 * @author Jason UI             27674749
 * @author Randipa Gunathilake  66941908
 * @brief Driver for handling LEDs
 * @version 0.1
 * @date 2021-08-04
 *
 */
//        >(')____,  >(')____,  >(')____,  >(')____,  >(') ___,
//         (` =~~/    (` =~~/    (` =~~/    (` =~~/    (` =~~/
//    ~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~
/************************************************************************/
/*                                                                      */
/*                                led.c                                 */
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
#include "led.h"

typedef struct led_s
{
    //pins
    const uint32_t port;
    const uint8_t pin;
    //logic variables
    bool state;
} led_t;

static void led_init_hw(led_cfg_t *cfg)
{

    SysCtlPeripheralEnable(cfg->periph);
    GPIOPadConfigSet(cfg->port, cfg->pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPD);
    GPIODirModeSet(cfg->port, cfg->pin, GPIO_DIR_MODE_OUT);
    GPIOPinWrite(cfg->port, cfg->pin, 0x00);
}

led_t *led_init(led_cfg_t *cfg)
{
    //init hardware.
    led_init_hw(cfg);

    // malloc a sw struct in heap.
    led_t *led_pt = pvPortMalloc(sizeof(led_t));

    //initialize a sw struct in stack.
    led_t led_init = {
        .port = cfg->port,
        .pin = cfg->pin,
        .state = false};

    //copy the content in stack to heap.
    memcpy(led_pt, &led_init, sizeof(led_t));

    return led_pt;
}

void led_on(led_t *led)
{
    led->state = true;
    GPIOPinWrite(led->port, led->pin, led->pin);
}

void led_off(led_t *led)
{
    led->state = false;
    GPIOPinWrite(led->port, led->pin, 0x00);
}

void led_toggle(led_t *led)
{
    if (led->state)
        led_off(led);
    else
        led_on(led);
}

led_state_t led_state(led_t *led)
{
    return led->state;
}

int led_free(led_t *led)
{
    vPortFree(led);
    return 1;
}