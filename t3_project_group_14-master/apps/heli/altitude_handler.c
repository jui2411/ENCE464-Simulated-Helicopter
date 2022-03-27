/**
 * @file altitude_handler.c
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
/*                           altitude_handler.c                         */
/*                                                                      */
/************************************************************************/

// RTOS Library includes
#include <FreeRTOS.h>
#include <task.h>

// Application Library includes
#include "config/target.h"
#include "altitude_handler.h"
#include "adcd.h"

static adc_t *alt;

// defines a circular buffer used to store
//static circBuf_t g_potBuffer;

// Defines volatile variable to store most recent altitude reading
static volatile uint32_t current_raw_altitude = 0;

// Defines variable to store initial voltage for the landed state
static volatile uint32_t landed_raw_altitude = 0;

// Defines variable to store ADC range based on which landed voltage was selected
static volatile uint32_t raw_altitude_range = 0;


/**
 * @brief Get() method to read the latest raw altitude 
 * reading as an unprocessed ADC conversion value (0-4096)
 * @return uint32_t current_raw_altitude 
 */
uint32_t altitude_raw_get(void)
{
    // Method to convert value
    return landed_raw_altitude;
}

/**
 * @brief Get() method to read the latest altitude 
 * as a processed 0 to 100 value
 * @return uint32_t current_altitude 
 */
uint32_t altitude_get(void)
{
    // Method to convert value
    uint32_t processed_current_altitude = ((100 * (landed_raw_altitude - current_raw_altitude)) / ANALOGUE_ADC_MULTIPLIER);

    return processed_current_altitude;
}

/**
 * @brief Handler that is called when the ADC conversion complete
 * interrupt occurs. This method writes the returned result
 * from the ADC conversion to a circular buffer
 */
void altitude_adc_isr(void)
{
    adc_isr(alt);
    current_raw_altitude = adc_read(alt);
}

// ADC is 12-bit
// range: 0-4095 (0-3v3 range, 0.8mV resolution)

void altitude_initialise(void)
{
    adc_cfg_t cfg = {
        .periph = SYSCTL_PERIPH_ADC0,
        .priority = 0,
        .base = ALT_ADC_BASE,
        .sequencer = ALT_SEQUENCE_NO,
        .channel = ALT_SENS_CHANNEL,
        .buffer_size = 5,
        .adc_isr = altitude_adc_isr};

    alt = adc_init(&cfg);
}

// Task to initiate new analogue conversion every ALTITUDE_TASK_PERIOD ms
// If a landed voltage has not been set, this task will automatically set the landed
// voltage to the first (smoothed) reading obtained from the altitude analogue pin
void altitude_sample_task(void *args)
{
    UNUSED(args);

    static bool init_landed_alt = false;

    TickType_t wake_time = xTaskGetTickCount();
    while (1)
    {

        // Trigger ADC read
        adc_read_trigger(alt);

        // Check if landed voltage has been set
        if ((!init_landed_alt) && (current_raw_altitude != 0))
        {
            landed_raw_altitude = current_raw_altitude;
            init_landed_alt = true;
        }

        // Delay loop
        vTaskDelayUntil(&wake_time, ALTITUDE_TASK_PERIOD);
    }
}
