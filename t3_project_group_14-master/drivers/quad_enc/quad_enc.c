/**
 * @file quad_enc.c
 * @author Zach Preston         81883956
 * @author Hank Wu              34748017
 * @author Jason UI             27674749
 * @author Randipa Gunathilake  66941908
 * @brief Quadrature encoder driver
 * @version 0.1
 * @date 2021-08-20
 *
 */
//        >(')____,  >(')____,  >(')____,  >(')____,  >(') ___,
//         (` =~~/    (` =~~/    (` =~~/    (` =~~/    (` =~~/
//    ~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~
/************************************************************************/
/*                                                                      */
/*                            quad_enc.c                                */
/*                                                                      */
/************************************************************************/
// Standard library includes
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// Tiva library includes
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/uart.h>
#include <driverlib/pin_map.h>
#include <utils/ustdlib.h>
#include "inc/tm4c123gh6pm.h"

// RTOS Library includes
#include <FreeRTOS.h>
#include <task.h>

#include "quad_enc.h"
#include "math_ext/math_ext.h"

/**
 * @brief Internal quad encoder struct
 * 
 */
struct quad_enc_dev_s
{
  uint32_t enc_port;
  uint32_t enc_a_pin;
  uint32_t enc_b_pin;

  uint32_t ref_port;
  uint32_t ref_pin;

  volatile uint32_t heading_tick;
  // Heading ticks clockwise -ve, anti-clockwise +ve
  uint16_t heading;  // Heading in degrees
  volatile uint8_t a_prev;
  volatile uint8_t b_prev;
  volatile uint8_t a_curr;
  volatile uint8_t b_curr;
  volatile bool b_reset_heading;
  uint32_t ticks_per_rev;
};


// Maximum number of encoders
#define QUAD_ENC_MAX_CHAN 2

// Statically allocated encoders
static quad_enc_dev_t encoders[QUAD_ENC_MAX_CHAN];

/**
 * @brief Encoder ISR 
 * 
 * Triggered on rising and falling edge of both channel A and B
 * 
 */
static void enc_isr0()
{
    GPIOIntClear(encoders[0].enc_port, encoders[0].enc_a_pin | encoders[0].enc_b_pin);

    // Get current state of the channels
    encoders[0].a_curr = (GPIOPinRead(encoders[0].enc_port, encoders[0].enc_a_pin) & (encoders[0].enc_a_pin)) == encoders[0].enc_a_pin;
    encoders[0].b_curr = (GPIOPinRead(encoders[0].enc_port, encoders[0].enc_b_pin) & (encoders[0].enc_b_pin)) == encoders[0].enc_b_pin;

    // Calculate heading using XOR method
    encoders[0].heading_tick -= encoders[0].b_prev ^ encoders[0].a_curr;
    encoders[0].heading_tick += encoders[0].b_curr ^ encoders[0].a_prev;

    // If a flags is set to reset the heading, reset it now
    // The flag is set from heading pin ISR
    if(encoders[0].b_reset_heading) {
      encoders[0].heading_tick = 0;
      encoders[0].b_reset_heading = false;
    }

    encoders[0].a_prev = encoders[0].a_curr;
    encoders[0].b_prev = encoders[0].b_curr;
}

/**
 * @brief Heading ISR; used to reset the heading back to 0
 * 
 */
static void enc_ref_isr0()
{
    GPIOIntClear(encoders[0].ref_port, encoders[0].ref_pin);
    encoders[0].b_reset_heading = true;
}

/**
 * @brief Inti the hardware needed for the encoder
 * 
 * @param cfg : Configuration struct
 * @param instance : Instance of the encoder to init
 */
static void quad_enc_init_hw(quad_enc_cfg_t *cfg, uint8_t instance)
{
    //Enable the clock for the GPIO used
    SysCtlPeripheralEnable(cfg->enc_periph);
    SysCtlPeripheralEnable(cfg->ref_periph);

    if(instance == 0)
        GPIOIntRegister(cfg->enc_port, enc_isr0); 

    //Setup the quad encoder input channels as input
    GPIOPinTypeGPIOInput(cfg->enc_port, cfg->enc_a_pin);
    GPIOPinTypeGPIOInput(cfg->enc_port, cfg->enc_b_pin);

    GPIOIntTypeSet(cfg->enc_port, cfg->enc_a_pin, GPIO_BOTH_EDGES);
    GPIOIntTypeSet(cfg->enc_port, cfg->enc_b_pin, GPIO_BOTH_EDGES);

    GPIOPadConfigSet(cfg->enc_port, cfg->enc_a_pin, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(cfg->enc_port, cfg->enc_b_pin, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    GPIOIntEnable(cfg->enc_port, cfg->enc_a_pin | cfg->enc_b_pin);

    //Set up reference as input. Goes low when it is at reference position
    if(instance == 0)
        GPIOIntRegister(cfg->ref_port, enc_ref_isr0);

    GPIOPinTypeGPIOInput(cfg->ref_port, cfg->ref_pin);

    GPIOIntTypeSet(cfg->ref_port, cfg->ref_pin, GPIO_FALLING_EDGE);

    GPIOPadConfigSet(cfg->ref_port, cfg->ref_pin, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    GPIOIntEnable(cfg->ref_port, cfg->ref_pin);
}

/**
 * @brief Get the current heading
 * 
 * @param enc : Encoder hander
 * @return uint32_t : Heading in degrees
 */
uint32_t quad_enc_heading_get(quad_enc_t enc) 
{
    return enc->heading;
}

/**
 * @brief Calculate the heading
 * 
 * @param enc : Encoder hander
 */
void quad_enc_heading_calc(quad_enc_t enc) {
    enc->heading = math_ext_modulo((DEG_PER_REV * enc->heading_tick) / enc->ticks_per_rev, DEG_PER_REV);
}

/**
 * @brief Initialise and return a quad encoder object using configuration struct passed
 * 
 * @param cfg : Configuration struct used to init
 * @return quad_enc_t : Handler
 */
quad_enc_t quad_enc_init(quad_enc_cfg_t *cfg)
{
    // Keep track of how many encoder objects we have created
    static int instance = 0;

    if (instance > QUAD_ENC_MAX_CHAN) {
        // Infinite loop, for development
        while(1) {}
    }

    // Initialise the encoder hardware
    quad_enc_init_hw(cfg, instance);

    quad_enc_dev_t qc = {.enc_port = cfg->enc_port,
                     .enc_a_pin = cfg->enc_a_pin,
                     .enc_b_pin = cfg->enc_b_pin,
                     .ref_port = cfg->ref_port,
                     .ref_pin = cfg->ref_pin,
                     .heading_tick = 112,
                     .heading = 0,
                     .a_prev = 0,
                     .b_prev = 0,
                     .a_curr = 0,
                     .b_curr = 0,
                     .b_reset_heading = false,
                     .ticks_per_rev = cfg->ticks_per_rev};

    // Store the configured encoder
    encoders[instance] = qc;

    // Return an handler for the encoder
    return &encoders[instance++];
}