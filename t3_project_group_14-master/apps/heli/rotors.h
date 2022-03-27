/**
 * @file rotors.h
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
/*                             rotors.h                                 */
/*                                                                      */
/************************************************************************/

#ifndef ROTORS_H_
#define ROTORS_H_

/**
 * @brief Definition of rotor variables
 * 
 */
#define ROTOR_NUM 2
#define ROTOR_TAIL 0
#define ROTOR_MAIN 1

//For a 50 KHz frequency, the period = 1/50,000, or 20
// microseconds. For a 20 MHz clock, this translates to 400 clock ticks.
// Use this value to set the period.

/**
 * @brief Definition of pwm variables
 * 
 */
#define ROTOR_PWM_DIV 8
#define ROTOR_PWM_DIV_MAC SYSCTL_PWMDIV_8
#define ROTOR_PWM_FREQ 200

/**
 * @brief Definition of throttle limiting variables
 * 
 */
#define ROTOR_MAX_THROTTLE 98
#define ROTOR_MIN_THROTTLE 2

/**
 * @brief Initialises the main and tail rotors. It should be noted that both rotors 
 * are initially set to run at zero altitude and heading
 * 
 */
void rotors_initialise(void);

/**
 * @brief A function used to set the duty cycle of the PWM for selected rotor
 * 
 * @param rotor_type Select the rotor (main or tail rotor) using ROTOR_TAIL or ROTOR_MAIN
 *  that you want to change power 
 * @param throttle Select the rotor power
 * Range 2-98 
 */
void rotor_set_pwr(int8_t rotor_type, uint8_t throttle);


#endif