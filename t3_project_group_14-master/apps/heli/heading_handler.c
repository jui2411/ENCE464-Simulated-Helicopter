/**
 * @file heading_handler.c
 * @author Zach Preston         81883956
 * @author Hank Wu              34748017
 * @author Jason UI             27674749
 * @author Randipa Gunathilake  66941908
 * @brief Estimate the heading (yaw) of the helicopter by calculating phase
 * difference of quadrature encoder; also buffers and filters the data
 * @version 0.1
 * @date 2021-07-14
 *
 */

//        >(')____,  >(')____,  >(')____,  >(')____,  >(') ___,
//         (` =~~/    (` =~~/    (` =~~/    (` =~~/    (` =~~/
//    ~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~
/************************************************************************/
/*                                                                      */
/*                           heading_handler.c                          */
/*                                                                      */
/************************************************************************/

// RTOS Library includes
#include <FreeRTOS.h>
#include <task.h>

// Application Library includes
#include "config/target.h"
#include "heading_handler.h"
#include "quad_enc.h"

// Handle for the quadrature encoder
static quad_enc_t encoder;

// Configuration for the quadrature encoder
quad_enc_cfg_t encoder_cfg = {.enc_periph = YAW_SENS_PERIPH,
                              .enc_port = YAW_SENS_PORT,
                              .enc_a_pin = YAW_SENS_A_PIN,
                              .enc_b_pin = YAW_SENS_B_PIN,
                              .ref_periph = YAW_REF_PERIPH,
                              .ref_port = YAW_REF_PORT,
                              .ref_pin = YAW_REF_PIN,
                              .ticks_per_rev = YAW_TICKS_PER_REV};

/**
 * @brief Get the current estimated heading
 * 
 * @return uint32_t the current heading
 */
uint32_t heading_get(void)
{
  // Get the current heading from the quad encoder driver
  return quad_enc_heading_get(encoder);
}

/**
 * @brief RTOS task to buffer, filter and calculate the heading
 * 
 * @param pvParameters Parameters for the task not use here
 */
void heading_task(void *args)
{
    UNUSED(args);

    TickType_t wake_time = xTaskGetTickCount();
    while (1)
    {
        // TODO: Add filtering ?
        //Calculate the heading using the quad encoder driver
        quad_enc_heading_calc(encoder);

        vTaskDelayUntil(&wake_time, HEADING_HANDLER_UPDATE_MS);
    }
}

/**
 * @brief Function to init the heading module; this inits the peripherals needed
 * for the quadrature encoder reading
 */
void heading_initialise(void) {
  // Init the quad encoder driver using the configurations provided
  encoder = quad_enc_init(&encoder_cfg);
}