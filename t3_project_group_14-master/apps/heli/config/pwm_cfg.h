/**
 * @file pwm_cfg.h
 * @author Zach Preston         81883956
 * @author Hank Wu              34748017
 * @author Jason UI             27674749
 * @author Randipa Gunathilake  66941908
 * @brief PWM configs
 * @version 0.1
 * @date 2021-08-22
 *
 */
//        >(')____,  >(')____,  >(')____,  >(')____,  >(') ___,
//         (` =~~/    (` =~~/    (` =~~/    (` =~~/    (` =~~/
//    ~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~
/************************************************************************/
/*                                                                      */
/*                              PWM.h                                */
/*                                                                      */
/************************************************************************/

/**
 * @brief Configuration for the tail rotor containing definition of hardware, 
 * frequency, and speed-limiting variables to run a pwm
 * 
 */
static pwm_cfg_t rotor_tail_cfg = {
    .periph_pwm = ROTOR_TAIL_PERIPH,
    .gpio = ROTOR_TAIL_GPIO,
    .port = ROTOR_TAIL_PORT,
    .pin = ROTOR_TAIL_PIN,
    .base = ROTOR_TAIL_BASE,
    .gen = ROTOR_TAIL_GEN,
    .out = ROTOR_TAIL_OUT,
    .out_bit = ROTOR_TAIL_OUT_BIT,
    .periph_gpio = ROTOR_TAIL_PERIPH_GPIO,
    .div = ROTOR_PWM_DIV, 
    .freq = ROTOR_PWM_FREQ, 
    .lim = ROTOR_MAX_THROTTLE}; 

/**
 * @brief Configuration for the main rotor containing definition of hardware, 
 * frequency, and speed-limiting variables to run a pwm
 * 
 */
static pwm_cfg_t rotor_main_cfg = {
    .periph_pwm = ROTOR_MAIN_PERIPH,
    .gpio = ROTOR_MAIN_GPIO,
    .port = ROTOR_MAIN_PORT,
    .pin = ROTOR_MAIN_PIN,
    .base = ROTOR_MAIN_BASE,
    .gen = ROTOR_MAIN_GEN,
    .out = ROTOR_MAIN_OUT,
    .out_bit = ROTOR_MAIN_OUT_BIT,
    .periph_gpio = ROTOR_MAIN_PERIPH_GPIO,
    .div = ROTOR_PWM_DIV, 
    .freq = ROTOR_PWM_FREQ, 
    .lim = ROTOR_MAX_THROTTLE};