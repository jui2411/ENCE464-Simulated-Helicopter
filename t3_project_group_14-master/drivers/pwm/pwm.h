/**
 * @file pwm.h
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
/*                                 pwm.h                                */
/*                                                                      */
/************************************************************************/

#ifndef _PWM_H_
#define _PWM_H_

#ifdef __cplusplus
extern "C"
{
#endif

// Standard Library includes

// Size of stack memory
#define PWM_MAX_HANDLES 5

// pwm configuration for initialisation
typedef struct pwm_cfg_s {
  uint32_t periph_pwm;
  uint32_t gpio;
  uint32_t port;
  uint8_t pin;
  uint32_t base;
  uint32_t gen;
  uint32_t out;
  uint32_t out_bit;
  uint32_t periph_gpio;
  uint32_t div;   // eg. ROTOR_PWM_DIV
  uint32_t freq;  // eg. ROTOR_PWM_FREQ
  uint8_t lim;    // eg. ROTOR_PWM_FREQ
} pwm_cfg_t;

// pwm configuration for changing duty cycle
struct pwm_s;
typedef struct pwm_s pwm_t;

/**
 * @brief initialises pwm, check for stack memory failure, and create pwm 
 * configurations to change duty cycle
 * 
 * @param cfg Configuration for either main or tail rotor containing definition of
 *  hardware, frequency, and speed-limiting variables to run a pwm
 * @return pwm_t* pwm configuration for duty cycle
 */
pwm_t *pwm_init(pwm_cfg_t *cfg);

/**
 * @brief Change the duty cycle of the PWM
 * 
 * @param pwm select the pwm that you want to change the duty cycle
 * @param new_duty_cycle the duty cycle you want to set
 */
void PWM_duty_set(pwm_t *pwm, int8_t new_duty_cycle);

#ifdef __cplusplus
}
#endif

#endif /* _ROTORS_H_ */