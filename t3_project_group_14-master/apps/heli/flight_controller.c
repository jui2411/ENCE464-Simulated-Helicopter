/**
 * @file flight_controller.c
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
/*                      flight_controller.c                             */
/*                                                                      */
/************************************************************************/

// RTOS Library includes
#include <FreeRTOS.h>
#include <task.h>

// Application Library includes
#include "flight_controller.h"
#include "config/target.h"
#include "pid/pid.h"
#include "math_ext/math_ext.h"
#include "heading_handler.h"
#include "altitude_handler.h"
#include "rotors.h"
#include "pilot.h"

// A PID is used for each axis (i.e. altitude and heading)
// These defines are used to make it clear which pid handler we are referring to
#define PID_ALTITUDE 0
#define PID_HEADING 1

// Handlers for the PIDs
static pid_controller_t pids[2];

/**
 * @brief Initilise the flight controller
 * 
 * Not doing anything right now; nothing to init
 * 
 */
void flight_controller_initialise(void)
{
}

/**
 * @brief RTOS task to update the PID
 * 
 * @param pvParameters task parameters not used
 */
void flight_controller_task(void *pvParameters)
{
    UNUSED(pvParameters);

    // PID configuration for altitude, includes configs such as PID gain and max output values
    pid_cfg_t pid_altitude_cfg = {.k_p = PID_ALTITUDE_KP,
                                  .k_i = PID_ALTITUDE_KI,
                                  .k_d = PID_ALTITUDE_KD,
                                  .tau = PID_ALTITUDE_TAU,
                                  .out_lim_min = PID_ALTITUDE_LIM_MIN,
                                  .out_lim_max = PID_ALTITUDE_LIM_MAX,
                                  .inter_lim_min = PID_ALTITUDE_INTER_LIM_MIN,
                                  .inter_lim_max = PID_ALTITUDE_INTER_LIM_MAX,
                                  .T = PID_ALTITUDE_T};

    // PID configuration for heading, includes configs such as PID gain and max output values
    pid_cfg_t pid_heading_cfg = {.k_p = PID_HEADING_KP,
                                 .k_i = PID_HEADING_KI,
                                 .k_d = PID_HEADING_KD,
                                 .tau = PID_HEADING_TAU,
                                 .out_lim_min = PID_HEADING_LIM_MIN,
                                 .out_lim_max = PID_HEADING_LIM_MAX,
                                 .inter_lim_min = PID_HEADING_INTER_LIM_MIN,
                                 .inter_lim_max = PID_HEADING_INTER_LIM_MAX,
                                 .T = PID_HEADING_T};

    // Init the PID for altitude, returns a OOD style handler
    pids[PID_ALTITUDE] = pid_init(&pid_altitude_cfg);

    // Init the PID for heading, returns a OOD style handler
    pids[PID_HEADING] = pid_init(&pid_heading_cfg);

    TickType_t wake_time = xTaskGetTickCount();

    while (1)
    {
        // Control signals for heading and altitude, these will be written to the motors
        uint32_t heading_ctrl, altitude_ctrl;

        // Disable global interrupts so we run the next operations without any
        // interrupts or task switching
        taskENTER_CRITICAL();

        // Calculate the altitude control signal using target altitude and the current altitude
        altitude_ctrl = pid_controller_update(pids[PID_ALTITUDE], pilot_get_altitude_goal(), altitude_get());

        // Calculate the heading control signal using target heading and the current heading
        // We are using a special update called mod to handle wrap around errors
        // It is more efficient to go from for example 10deg to 350deg in counter clockwise instead of anti-clockwise
        heading_ctrl = pid_controller_mod_update(pids[PID_HEADING], pilot_get_heading_goal(), heading_get(), DEG_PER_REV);

        // Set the main rotor speed for altitude control from PID calculation
        rotor_set_pwr(ROTOR_MAIN, altitude_ctrl);

        // Set the tail rotor speed for heading control from PID calculation
        rotor_set_pwr(ROTOR_TAIL, heading_ctrl);

        // Reenable the interrupts
        taskEXIT_CRITICAL();

        vTaskDelayUntil(&wake_time, FLIGHT_CONTROLLER_UPDATE_MS );
    }
}