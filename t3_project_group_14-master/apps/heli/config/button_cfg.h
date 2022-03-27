/**
 * @file button_cfg.h
 * @author Zach Preston         81883956
 * @author Hank Wu              34748017
 * @author Jason UI             27674749
 * @author Randipa Gunathilake  66941908
 * @brief Button configs
 * @version 0.1
 * @date 2021-07-14
 *
 */
//        >(')____,  >(')____,  >(')____,  >(')____,  >(') ___,
//         (` =~~/    (` =~~/    (` =~~/    (` =~~/    (` =~~/
//    ~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~
/************************************************************************/
/*                                                                      */
/*                          button_cfg.h                                */
/*                                                                      */
/************************************************************************/

// Button configs
button_cfg_t button_left_cfg = {
    .button_periph = LEFT_BUT_PERIPH,
    .button_port = LEFT_BUT_PORT_BASE,
    .button_pin = LEFT_BUT_PIN,
    .pin_type = GPIO_PIN_TYPE_STD_WPU,
    .activate_tick = BUTTON_DEBOUNCE_TICK,
    .long_press_tick = BUTTON_LONGPRESS_TICK};

button_cfg_t button_right_cfg = {
    .button_periph = RIGHT_BUT_PERIPH,
    .button_port = RIGHT_BUT_PORT_BASE,
    .button_pin = RIGHT_BUT_PIN,
    .pin_type = GPIO_PIN_TYPE_STD_WPU,
    .activate_tick = BUTTON_DEBOUNCE_TICK,
    .long_press_tick = BUTTON_LONGPRESS_TICK};

button_cfg_t button_up_cfg = {
    .button_periph = UP_BUT_PERIPH,
    .button_port = UP_BUT_PORT_BASE,
    .button_pin = UP_BUT_PIN,
    .pin_type = GPIO_PIN_TYPE_STD_WPD,
    .activate_tick = BUTTON_DEBOUNCE_TICK,
    .long_press_tick = BUTTON_LONGPRESS_TICK};

button_cfg_t button_down_cfg = {
    .button_periph = DOWN_BUT_PERIPH,
    .button_port = DOWN_BUT_PORT_BASE,
    .button_pin = DOWN_BUT_PIN,
    .pin_type = GPIO_PIN_TYPE_STD_WPD,
    .activate_tick = BUTTON_DEBOUNCE_TICK,
    .long_press_tick = BUTTON_LONGPRESS_TICK};