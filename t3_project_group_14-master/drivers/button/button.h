/**
 * @file button.h
 * @author Zach Preston         81883956
 * @author Hank Wu              34748017
 * @author Jason UI             27674749
 * @author Randipa Gunathilake  66941908
 * @brief Driver for handling buttons
 * @version 0.1
 * @date 2021-07-14
 *
 */
//        >(')____,  >(')____,  >(')____,  >(')____,  >(') ___,
//         (` =~~/    (` =~~/    (` =~~/    (` =~~/    (` =~~/
//    ~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~
/************************************************************************/
/*                                                                      */
/*                              button.h                                */
/*                                                                      */
/************************************************************************/

#ifndef _BUTTON_H_
#define _BUTTON_H_

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct button_cfg_s
    {
        uint32_t button_periph;
        uint32_t button_port;
        uint8_t button_pin;
        uint32_t pin_type;
        uint32_t activate_tick;
        uint32_t long_press_tick;
    } button_cfg_t;

    struct button_s;
    typedef struct button_s button_t;

    typedef enum
    {
        BUTTON_RELEASED,
        BUTTON_PUSHED,
        BUTTON_NO_CHANGE,
        BUTTON_HOLD
    } button_state_t;

    /*Initialize button. pass in a button_cfg_t for setup.
    This will allocate a button struct in the heap memory.*/
    button_t *button_init(button_cfg_t *cfg);

    /*Update button.*/
    void button_update(button_t *button);

    /*read button state.*/
    button_state_t button_read(button_t *button);

    /*de-initilize button.*/
    int button_deinit(button_t *button);

#ifdef __cplusplus
}
#endif

#endif /* _BUTTON_H_ */