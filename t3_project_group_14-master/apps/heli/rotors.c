/**
 * @file rotors.c
 * @author Zach Preston         81883956
 * @author Hank Wu              34748017
 * @author Jason UI             27674749
 * @author Randipa Gunathilake  66941908
 * @brief Handles initialising and controlling power of the main and tail rotors
 * of the helirig.
 * @version 0.1
 * @date 2021-07-14
 *
 */
//        >(')____,  >(')____,  >(')____,  >(')____,  >(') ___,
//         (` =~~/    (` =~~/    (` =~~/    (` =~~/    (` =~~/
//    ~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~
/************************************************************************/
/*                                                                      */
/*                             rotors.c                                 */
/*                                                                      */
/************************************************************************/
// Standard Library includes
#include <stdint.h>
#include <stdbool.h>

// TIVA Library includes
#include <driverlib/pwm.h>

// FreeRTOS Library includes
#include <FreeRTOS.h>

// Application Library includes
#include "rotors.h"
#include "pwm.h"
#include "config/target.h"
#include "config/pwm_cfg.h"

/**
 * @brief An array that stores rotors' configurations, containing hardware, frequency,
 *  and speed-limiting variables to change the power of the throttle
 * 
 */
static pwm_t *rotor[ROTOR_NUM];

/**
 * @brief Initialises the main and tail rotors. It should be noted that both rotors 
 * are initially set to run at zero altitude and heading
 * 
 */
void rotors_initialise(void)
{
    SysCtlPWMClockSet(ROTOR_PWM_DIV_MAC);
    rotor[ROTOR_TAIL] = pwm_init(&rotor_tail_cfg);   
    rotor[ROTOR_MAIN] = pwm_init(&rotor_main_cfg);
    rotor_set_pwr(ROTOR_TAIL,0); 
    rotor_set_pwr(ROTOR_MAIN,0); 
}

/**
 * @brief A function used to set the duty cycle of the PWM for selected rotor
 * 
 * @param rotor_type Select the rotor (main or tail rotor) using ROTOR_TAIL or ROTOR_MAIN
 *  that you want to change power 
 * @param throttle Select the rotor power
 * Range 2-98 
 */
void rotor_set_pwr(int8_t rotor_type, uint8_t throttle) {

    if(throttle > ROTOR_MAX_THROTTLE)
        throttle = ROTOR_MAX_THROTTLE;
    else if (throttle < ROTOR_MIN_THROTTLE)
        throttle = ROTOR_MIN_THROTTLE;

    PWM_duty_set(rotor[rotor_type], throttle);

}