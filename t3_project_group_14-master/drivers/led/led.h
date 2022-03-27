/**
 * @file led.h
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
/*                                led.h                                 */
/*                                                                      */
/************************************************************************/

#ifndef _LED_H_
#define _LED_H_

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct led_cfg_s
    {
        uint32_t periph;
        uint32_t port;
        uint8_t pin;
    } led_cfg_t;

    struct led_s;
    typedef struct led_s led_t;

    typedef enum
    {
        LED_OFF,
        LED_ON
    } led_state_t;

    /**
     * @brief initialise led GPIO and logic. The return pointer needs to be catched.
     * 
     * @param cfg 
     * @return led_t* 
     */
    led_t *led_init(led_cfg_t *cfg);

    /**
     * @brief turn on LED.
     * 
     * @param led 
     */
    void led_on(led_t *led);

    /**
     * @brief turn off led.
     * 
     * @param led 
     */
    void led_off(led_t *led);

    /**
     * @brief toggle led.
     * 
     * @param led 
     */
    void led_toggle(led_t *led);

    /**
     * @brief return the led status.
     * 
     * @param led 
     * @return led_state_t 
     */
    led_state_t led_state(led_t *led);

    /**
     * @brief free up the dynamic memory for storing led struct.
     * 
     * @param led 
     * @return int 
     */
    int led_free(led_t *led);

#ifdef __cplusplus
}
#endif

#endif /* _LED_H_ */