/**
 * @file target.h
 * @author Zach Preston         81883956
 * @author Hank Wu              34748017
 * @author Jason UI             27674749
 * @author Randipa Gunathilake  66941908
 * @brief Common configs for Tiva board and the project
 * @version 0.1
 * @date 2021-07-14
 *
 */
//        >(')____,  >(')____,  >(')____,  >(')____,  >(') ___,
//         (` =~~/    (` =~~/    (` =~~/    (` =~~/    (` =~~/
//    ~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~
/************************************************************************/
/*                                                                      */
/*                              target.h                                */
/*                                                                      */
/************************************************************************/

// Standard library includes
#include <stdint.h>
#include <stdbool.h>

// Tiva library includes
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>

#define UNUSED(x) ((void)(x))

#define ADC_MAX_VAL 4095

//max speed = 0, a larger number correspond to a lower changing speed.
#define UI_LONG_PRESS_HEADING_CHANGING_SPEED 0
#define UI_LONG_PRESS_ALTITUDE_CHANGING_SPEED 3

// for pilot output.
#define HEADING_MAX_VAL 360
#define HEADING_MIN_VAL 0

#define ALTITUDE_MAX_VAL 100
#define ALTITUDE_MIN_VAL 0

//----------buttons----------
#define UP_BUT_PERIPH  SYSCTL_PERIPH_GPIOE
#define UP_BUT_PORT_BASE  GPIO_PORTE_BASE
#define UP_BUT_PIN  GPIO_PIN_0
#define UP_BUT_NORMAL  false
// DOWN button
#define DOWN_BUT_PERIPH  SYSCTL_PERIPH_GPIOD
#define DOWN_BUT_PORT_BASE  GPIO_PORTD_BASE
#define DOWN_BUT_PIN  GPIO_PIN_2
#define DOWN_BUT_NORMAL  false
// LEFT button
#define LEFT_BUT_PERIPH  SYSCTL_PERIPH_GPIOF
#define LEFT_BUT_PORT_BASE  GPIO_PORTF_BASE
#define LEFT_BUT_PIN  GPIO_PIN_4
#define LEFT_BUT_NORMAL  true
// RIGHT button
#define RIGHT_BUT_PERIPH  SYSCTL_PERIPH_GPIOF
#define RIGHT_BUT_PORT_BASE  GPIO_PORTF_BASE
#define RIGHT_BUT_PIN  GPIO_PIN_0
#define RIGHT_BUT_NORMAL  true
//LSW
#define LSW_PERIPH  SYSCTL_PERIPH_GPIOA
#define LSW_PORT_BASE  GPIO_PORTA_BASE
#define LSW_PIN  GPIO_PIN_7
//RSW
#define RSW_PERIPH  SYSCTL_PERIPH_GPIOA
#define RSW_PORT_BASE  GPIO_PORTA_BASE
#define RSW_PIN  GPIO_PIN_6
//button config
#define BUTTON_LONGPRESS_TICK 50
#define BUTTON_DEBOUNCE_TICK 3


#define LED_PERIPH  SYSCTL_PERIPH_GPIOF
#define LED_PORT    GPIO_PORTF_BASE
#define LED_R_PIN       GPIO_PIN_1
#define LED_G_PIN        GPIO_PIN_3
#define LED_B_PIN        GPIO_PIN_2
#define LED_RGB     (LED_R_PIN | LED_G_PIN | LED_B_PIN)

#define LED_D1_PERIPH SYSCTL_PERIPH_GPIOC
#define LED_D1_PORT GPIO_PORTC_BASE
#define LED_D1_PIN GPIO_PIN_6

#define LED_D2_PERIPH SYSCTL_PERIPH_GPIOC
#define LED_D2_PORT GPIO_PORTC_BASE
#define LED_D2_PIN GPIO_PIN_7

#define LED_D3_PERIPH SYSCTL_PERIPH_GPIOD
#define LED_D3_PORT GPIO_PORTD_BASE
#define LED_D3_PIN GPIO_PIN_6

#define LED_D4_PERIPH SYSCTL_PERIPH_GPIOB
#define LED_D4_PORT GPIO_PORTB_BASE
#define LED_D4_PIN GPIO_PIN_5

#define YAW_SENS_PERIPH     SYSCTL_PERIPH_GPIOB
#define YAW_SENS_PORT       GPIO_PORTB_BASE
#define YAW_SENS_A_PIN      GPIO_PIN_0
#define YAW_SENS_B_PIN      GPIO_PIN_1

#define YAW_REF_PERIPH      SYSCTL_PERIPH_GPIOC
#define YAW_REF_PORT        GPIO_PORTC_BASE
#define YAW_REF_PIN         GPIO_PIN_4

// Config for altitude pin: PE4 (A9)
#define ALT_SENS_PERIPH     SYSCTL_PERIPH_GPIOE
#define ALT_SENS_PORT       GPIO_PORTE_BASE

#define ALT_ADC_BASE        ADC0_BASE // Use ADC coverter 0
#define ALT_SENS_CHANNEL    ADC_CTL_CH9 // Pin PE4 (A9)
#define ALT_SEQUENCE_NO     3
#define ANALOGUE_ADC_MULTIPLIER 1241 //1365 // ~ 4096/3

//Main Rotor PWM Definitions for Initialization
#define ROTOR_TAIL_BASE PWM1_BASE
#define ROTOR_TAIL_PERIPH SYSCTL_PERIPH_PWM1
#define ROTOR_TAIL_GEN PWM_GEN_2
#define ROTOR_TAIL_OUT PWM_OUT_5
#define ROTOR_TAIL_OUT_BIT PWM_OUT_5_BIT
#define ROTOR_TAIL_PERIPH_GPIO SYSCTL_PERIPH_GPIOF
#define ROTOR_TAIL_GPIO GPIO_PF1_M1PWM5
#define ROTOR_TAIL_PIN GPIO_PIN_1
#define ROTOR_TAIL_PORT GPIO_PORTF_BASE

//Main Rotor PWM Definitions for Initialization
#define ROTOR_MAIN_BASE PWM0_BASE
#define ROTOR_MAIN_PERIPH SYSCTL_PERIPH_PWM0
#define ROTOR_MAIN_GEN PWM_GEN_3
#define ROTOR_MAIN_OUT PWM_OUT_7
#define ROTOR_MAIN_OUT_BIT PWM_OUT_7_BIT
#define ROTOR_MAIN_PERIPH_GPIO SYSCTL_PERIPH_GPIOC
#define ROTOR_MAIN_GPIO GPIO_PC5_M0PWM7
#define ROTOR_MAIN_PIN GPIO_PIN_5
#define ROTOR_MAIN_PORT GPIO_PORTC_BASE