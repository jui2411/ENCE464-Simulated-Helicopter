/**
 * @file main.c
 * @author Zach Preston         81883956
 * @author Hank Wu              34748017
 * @author Jason UI             27674749
 * @author Randipa Gunathilake  66941908
 * @brief
 * @version 0.1
 * @date 2021-08-21
 *
 */
//        >(')____,  >(')____,  >(')____,  >(')____,  >(') ___,
//         (` =~~/    (` =~~/    (` =~~/    (` =~~/    (` =~~/
//    ~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~
/************************************************************************/
/*                                                                      */
/*                               main.c                                 */
/*                                                                      */
/************************************************************************/
/************************************************************************/
/*  Attributions                                                        */
/*  Driver Libraries assembled by Ben Mitchell                          */
/************************************************************************/

// RTOS Library includes
#include <FreeRTOS.h>

// Application Library includes
#include "led_controller.h"
#include "ui_input_handler.h"
#include "altitude_handler.h"
#include "heading_handler.h"
#include "display_handler.h"
#include "flight_controller.h"
#include "rotors.h"
#include "pilot.h"



/**
 * @brief Initialising SysTick timer clock rate for the program
 * 
 */
void clock_initialise(void) {

    // Set the clock rate to 20 MHz
    SysCtlClockSet (SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
}


/**
 * @brief The entry point of the program
 * 
 * @return int 0 for success and anything else for failure
 */
int main(void) {

    // Initialise the clocks of the mcu
    clock_initialise();

    // Initialise the the status LEDs
    leds_initialise();

    // Initialise the OLED display
    display_initialise();

    // Initialise UI module for things such as button handling
    ui_initialise();

    // Initialise pilot module; this is responsible for managing the targeted
    // altitude and heading
    pilot_initialise();

    // Initialise the module to keep track of the heading
    // uses quadrature encoder to calculate the heading
    heading_initialise();

    // Initialise the module to keep track of the altitude
    // uses ADC reading
    altitude_initialise();

    // Initialise the rotors module; this is responsible for controlling the
    // main and tail motor
    rotors_initialise();

    // Initialise the PID module which is used to track and change the target
    // altitude and heading
    flight_controller_initialise();

    //cpu_load_task_init();

    // Create the RTOS tasks
    xTaskCreate(&heading_task, "heading", configMINIMAL_STACK_SIZE, NULL, 0, NULL);
    xTaskCreate(&altitude_sample_task, "altitude", configMINIMAL_STACK_SIZE, NULL, 0, NULL);
    xTaskCreate(&flight_controller_task, "flight_ctrl", configMINIMAL_STACK_SIZE, NULL, 0, NULL);
    xTaskCreate(&update_display_task, "display", configMINIMAL_STACK_SIZE, NULL, 0, NULL);
    xTaskCreate(&ui_task, "ui", configMINIMAL_STACK_SIZE, NULL, 0, NULL);
    xTaskCreate(&pilot_task, "pilot", configMINIMAL_STACK_SIZE, NULL, 0, NULL);

    // Run program kernel
    vTaskStartScheduler();

    return 0;
}

/**
 * @brief This is an error handling function called when FreeRTOS asserts.
 * This should be used for debugging purposes
 * @param pcFile 
 * @param ulLine 
 */
void vAssertCalled( const char * pcFile, unsigned long ulLine ) {
    UNUSED(pcFile); // unused
    UNUSED(ulLine); // unused
    while (true) ;
}