/**
 * @file flight_controller.h
 * @author Zach Preston         81883956
 * @author Hank Wu              34748017
 * @author Jason UI             27674749
 * @author Randipa Gunathilake  66941908
 * @brief Controls the main and tail motor using closed loop PID to a desired
 * altitude and heading
 * @version 0.1
 * @date 2021-07-14
 *
 */
//        >(')____,  >(')____,  >(')____,  >(')____,  >(') ___,
//         (` =~~/    (` =~~/    (` =~~/    (` =~~/    (` =~~/
//    ~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~
/************************************************************************/
/*                                                                      */
/*                      flight_controller.h                             */
/*                                                                      */
/************************************************************************/

#ifndef FLIGHT_CONTROLLER_H_
#define FLIGHT_CONTROLLER_H_

// PID controller update rate in ms
#define FLIGHT_CONTROLLER_UPDATE_MS 10

// PID Kp gain for the altitude
#define PID_ALTITUDE_KP 1.0f//5.0f
// PID Ki gain for the altitude
#define PID_ALTITUDE_KI 5.0f
// PID Kd gain for the altitude; not used right now
#define PID_ALTITUDE_KD 0.0f

// Derivative low pass filter time constant for altitude
#define PID_ALTITUDE_TAU 0.1f

// PID output min limit for the main motor
#define PID_ALTITUDE_LIM_MIN ROTOR_MIN_THROTTLE
// PID output max limit for the main motor
#define PID_ALTITUDE_LIM_MAX ROTOR_MAX_THROTTLE

// PID anti wind up limit min
#define PID_ALTITUDE_INTER_LIM_MIN -50.0f
// PID anti wind up limit max
#define PID_ALTITUDE_INTER_LIM_MAX 50.0f

// PID update rate used for the integral control
#define PID_ALTITUDE_T 0.01f

// PID Kp gain for the heading
#define PID_HEADING_KP 3.0f
// PID Ki gain for the heading
#define PID_HEADING_KI 2.0f
// PID Kd gain for the heading; not used right now
#define PID_HEADING_KD 0.0f

// Derivative low pass filter time constant for heading
#define PID_HEADING_TAU 0.1f

// PID output min limit for the tail motor
#define PID_HEADING_LIM_MIN ROTOR_MIN_THROTTLE
// PID output max limit for the tail motor
#define PID_HEADING_LIM_MAX ROTOR_MAX_THROTTLE

// PID anti wind up limit min
#define PID_HEADING_INTER_LIM_MIN -50.0f
// PID anti wind up limit max
#define PID_HEADING_INTER_LIM_MAX 50.0f

// PID update rate used for the integral control
#define PID_HEADING_T 0.01f

/**
 * @brief Initilise the flight controller
 * 
 * Not doing anything right now; nothing to init
 * 
 */
void flight_controller_initialise(void);

/**
 * @brief RTOS task to update the PID
 * 
 * @param pvParameters task parameters not used
 */
void flight_controller_task(void *pvParameters);

#endif