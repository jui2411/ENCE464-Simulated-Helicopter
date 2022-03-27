/**
 * @file led_cfg.h
 * @author Zach Preston         81883956
 * @author Hank Wu              34748017
 * @author Jason UI             27674749
 * @author Randipa Gunathilake  66941908
 * @brief LEDs configs
 * @version 0.1
 * @date 2021-07-14
 *
 */
//        >(')____,  >(')____,  >(')____,  >(')____,  >(') ___,
//         (` =~~/    (` =~~/    (` =~~/    (` =~~/    (` =~~/
//    ~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~
/************************************************************************/
/*                                                                      */
/*                          led_cfg.h                                   */
/*                                                                      */
/************************************************************************/

// LEDs configs
led_cfg_t led_r_cfg = {
    .periph = LED_PERIPH,
    .port = LED_PORT,
    .pin = LED_R_PIN};

led_cfg_t led_g_cfg = {
    .periph = LED_PERIPH,
    .port = LED_PORT,
    .pin = LED_G_PIN};

led_cfg_t led_b_cfg = {
    .periph = LED_PERIPH,
    .port = LED_PORT,
    .pin = LED_B_PIN};

led_cfg_t led_d1_cfg = {
    .periph = LED_D1_PERIPH,
    .port = LED_D1_PORT,
    .pin = LED_D1_PIN};

led_cfg_t led_d2_cfg = {
    .periph = LED_D2_PERIPH,
    .port = LED_D2_PORT,
    .pin = LED_D2_PIN};

led_cfg_t led_d3_cfg = {
    .periph = LED_D3_PERIPH,
    .port = LED_D3_PORT,
    .pin = LED_D3_PIN};

led_cfg_t led_d4_cfg = {
    .periph = LED_D4_PERIPH,
    .port = LED_D4_PORT,
    .pin = LED_D4_PIN};