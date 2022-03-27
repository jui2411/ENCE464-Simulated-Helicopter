/**
 * @file switch.h
 * @author Zach Preston         81883956
 * @author Hank Wu              34748017
 * @author Jason UI             27674749
 * @author Randipa Gunathilake  66941908
 * @brief Driver for handling switches
 * @version 0.1
 * @date 2021-08-08
 *
 */
//        >(')____,  >(')____,  >(')____,  >(')____,  >(') ___,
//         (` =~~/    (` =~~/    (` =~~/    (` =~~/    (` =~~/
//    ~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~
/************************************************************************/
/*                                                                      */
/*                              switch.h                                */
/*                                                                      */
/************************************************************************/
#ifndef _SWITCH_H_
#define _SWITCH_H_

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct switch_cfg_s
    {
        uint32_t periph;
        uint32_t port;
        uint8_t pin;
        uint32_t activate_tick;
    } switch_cfg_t;

    struct switch_s;
    typedef struct switch_s switch_t;

    typedef enum
    {
        SWITCH_UP,
        SWITCH_DOWN,
        SWITCH_NO_CHANGE
    } switch_state_t;

    /**
     * @brief initialise switch
     * 
     * @param cfg 
     * @return switch_t* 
     */
    switch_t *switch_init(switch_cfg_t *cfg);

    /**
     * @brief update switch.
     * 
     * @param sw 
     */
    void switch_update(switch_t *sw);

    /**
     * @brief read switch. return value change when state change, otherwise return SWITCH_NO_CHANGE.
     * 
     * @param sw 
     * @return switch_state_t 
     */
    switch_state_t switch_read(switch_t *sw);


    /**
     * @brief read switch state, either up or down.
     * 
     * @param sw 
     * @return bool 
     */
    switch_state_t switch_read_state(switch_t *sw);

    /**
     * @brief free the switch struct in the heap memory.
     * 
     */
    int switch_free(switch_t *sw);

#ifdef __cplusplus
}
#endif

#endif /* _SWITCH_H_ */