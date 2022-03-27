/**
 * @file heading_handler.h
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
/*                           heading_handler.h                          */
/*                                                                      */
/************************************************************************/

#ifndef HEADING_HANDLER_H_
#define HEADING_HANDLER_H_

// Number of pulses for a single revolution; needs to be changed for different
// configurations
#define YAW_TICKS_PER_REV 442

// The update rate in ms for the heading handler task
#define HEADING_HANDLER_UPDATE_MS 10

/**
 * @brief Function to init the heading module; this inits the peripherals needed
 * for the quadrature encoder reading
 */
void heading_initialise(void);

/**
 * @brief Get the current estimated heading
 * 
 * @return uint32_t the current heading
 */
uint32_t heading_get(void);

/**
 * @brief RTOS task to buffer, filter and calculate the heading
 * 
 * @param pvParameters Parameters for the task not use here
 */
void heading_task(void* pvParameters);

#endif