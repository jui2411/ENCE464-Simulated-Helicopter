/**
 * @file pwm.c
 * @author Zach Preston         81883956
 * @author Hank Wu              34748017
 * @author Jason UI             27674749
 * @author Randipa Gunathilake  66941908
 * @brief PWM Driver for controlling the amplitude of digital signals 
 * @version 0.1
 * @date 2021-08-14
 *
 */
//        >(')____,  >(')____,  >(')____,  >(')____,  >(') ___,
//         (` =~~/    (` =~~/    (` =~~/    (` =~~/    (` =~~/
//    ~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~
/************************************************************************/
/*                                                                      */
/*                                 pwm.c                                */
/*                                                                      */
/************************************************************************/

// Standard Library includes
#include <stdint.h>
#include <stdbool.h>

// TIVA Library includes
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pwm.h>

// FreeRTOS Library includes
#include <FreeRTOS.h>

// Application Library includes
#include "pwm.h"

/**
 * @brief typedef struct containing hardware, frequency,and speed-limiting variables
 *  to change the power of the throttle
 * 
 */
typedef struct pwm_s
{
    uint32_t periph_pwm;
    uint32_t port;
    uint32_t gpio;
    uint8_t pin;
    uint32_t base;
    uint32_t gen;
    uint32_t out;
    uint32_t div;
    uint32_t freq; 
} pwm_t;

/**
 * @brief An array that acts as stack memory for stack/static memory allocation to
 *  store rotors' configurations
 * 
 */
pwm_t pwm_channel[PWM_MAX_HANDLES];

/**
 * @brief initialises the hardware to create a pwm
 * 
 * @param cfg Configuration for either main or tail rotor containing definition of
 *  hardware, frequency, and speed-limiting variables to run a pwm
 */
static void pwm_init_hw(pwm_cfg_t *cfg)
{
    // Enable the peripheral, GPIO, and pin for pwm to run a rotor
    SysCtlPeripheralEnable(cfg->periph_pwm);
    SysCtlPeripheralEnable(cfg->gpio);
    GPIOPinTypePWM(cfg->port, cfg->pin);
    GPIOPinConfigure(cfg->gpio);

    // Wait for the pwm peripheral to be ready.
    while(!SysCtlPeripheralReady(cfg->periph_pwm))
    {
    }

    // Configure the PWM generator for count down mode with immediate updates
    // to the parameters.
    PWMGenConfigure(cfg->base, cfg->gen,
    PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);

    // Start the timers in generator for a rotor.
    PWMGenEnable(cfg->base, cfg->gen);

    // Enable a rotor's output.
    PWMOutputState(cfg->base, cfg->out_bit, true); 
}

/**
 * @brief initialises pwm, check for stack memory failure, and create pwm 
 * configurations to change duty cycle
 * 
 * @param cfg Configuration for either main or tail rotor containing definition of
 *  hardware, frequency, and speed-limiting variables to run a pwm
 * @return pwm_t* pwm configuration for duty cycle
 */
pwm_t *pwm_init(pwm_cfg_t *cfg)
{
    // stack memory counter
    static int i = 0;
    
    // If exceeding the maximum size of stack memory, then stop program
    while(i > PWM_MAX_HANDLES)
    {   
    }

    // initialises the hardware to run a pwm.
    pwm_init_hw(cfg);

    // create a pwm configuration containing hardware, frequency,
    // and speed-limiting variables to change the duty cycle of the pwm
    pwm_t pwm_init = {.periph_pwm = cfg->periph_pwm,
                      .port = cfg->port,
                      .gpio = cfg->gpio,
                      .pin = cfg->pin,
                      .base = cfg->base,
                      .gen = cfg->gen,
                      .out = cfg->out,
                      .div = cfg->div,
                      .freq = cfg->freq};

    // Add the pwm configuration into the stack memory 
    pwm_channel[i] = pwm_init;

    // increment stack memory counter
    i++;

    return &pwm_channel[i - 1];
}

/**
 * @brief Change the duty cycle of the PWM
 * 
 * @param pwm select the pwm that you want to change the duty cycle
 * @param new_duty_cycle the duty cycle you want to set
 */
void PWM_duty_set(pwm_t *pwm, int8_t new_duty_cycle) {

    // Set and enable clock to PWM module - (Clock Ticks = Processor Clock Rate/ Frequency)
    uint32_t pwm_ticks = SysCtlClockGet()/pwm->div/pwm->freq;

    // Set the period for PWM.
    PWMGenPeriodSet(pwm->base, pwm->gen, pwm_ticks);

    // Set the pulse width of PWM.
    PWMPulseWidthSet(pwm->base, pwm->out, pwm_ticks*new_duty_cycle/100);

    // Enable the PWM module
    SysCtlPeripheralEnable(pwm->periph_pwm);

    // Enable Port for use by the PWM peripheral
    SysCtlPeripheralEnable(pwm->gpio);

    // Configures pin for use by the PWM peripheral.
    GPIOPinTypePWM(pwm->port, pwm->pin);

    // Configures the alternate function of a GPIO pin.
    GPIOPinConfigure(pwm->gpio);
}