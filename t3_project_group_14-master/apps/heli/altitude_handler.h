/**
 * @file altitude_handler.h
 * @author Zach Preston         81883956
 * @author Hank Wu              34748017
 * @author Jason UI             27674749
 * @author Randipa Gunathilake  66941908
 * @brief Handles interfacing with the ADC for reading and processing the
 * altitude from the heliRig
 * @version 0.1
 * @date 2021-08-03
 *
 */

//        >(')____,  >(')____,  >(')____,  >(')____,  >(') ___,
//         (` =~~/    (` =~~/    (` =~~/    (` =~~/    (` =~~/
//    ~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~

/************************************************************************/
/*                                                                      */
//                           altitude_handler.h
/*                                                                      */
/************************************************************************/

#ifndef ALTITUDE_HANDLER_H_
#define ALTITUDE_HANDLER_H_


#define ALTITUDE_TASK_PERIOD 100

/**
 * @brief Method to initialise the adc used to 
 * sample the altitude analogue in port
 */
void altitude_initialise(void);

/**
 * @brief FreeRTOS task to initiate new analogue conversion every ALTITUDE_TASK_PERIOD ms
 * If a landed voltage has not been set, this task will automatically set the landed
 * voltage to the first (smoothed) reading obtained from the altitude analogue pin
 *
 * @param args Parameters for the task; not used for this task
 */
void altitude_sample_task(void* pvParameters);

/**
 * @brief Get() method to read the latest altitude as a processed 0 to 100 value
 *
 * @return uint32_t the current altitude
 */
uint32_t altitude_get(void);

#endif