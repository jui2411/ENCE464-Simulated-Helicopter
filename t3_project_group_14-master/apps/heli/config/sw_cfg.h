/**
 * @file sw_cfg.h
 * @author Zach Preston         81883956
 * @author Hank Wu              34748017
 * @author Jason UI             27674749
 * @author Randipa Gunathilake  66941908
 * @brief Switch configs
 * @version 0.1
 * @date 2021-07-14
 *
 */
//        >(')____,  >(')____,  >(')____,  >(')____,  >(') ___,
//         (` =~~/    (` =~~/    (` =~~/    (` =~~/    (` =~~/
//    ~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~
/************************************************************************/
/*                                                                      */
/*                              sw_cfg.h                                */
/*                                                                      */
/************************************************************************/


//switch configurations.
switch_cfg_t sw_right_cfg = {
    .periph = RSW_PERIPH,
    .port = RSW_PORT_BASE,
    .pin = RSW_PIN,
    .activate_tick = 3};

switch_cfg_t sw_left_cfg = {
    .periph = LSW_PERIPH,
    .port = LSW_PORT_BASE,
    .pin = LSW_PIN,
    .activate_tick = 3};