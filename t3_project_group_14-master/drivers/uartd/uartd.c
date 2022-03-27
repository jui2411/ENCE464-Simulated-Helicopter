/**
 * @file uart_handler.c
 * @author Zach Preston         81883956
 * @author Hank Wu              34748017
 * @author Jason UI             27674749
 * @author Randipa Gunathilake  66941908
 * @brief Module to handle serial printing
 * @version 0.1
 * @date 2021-07-14
 *
 */
//        >(')____,  >(')____,  >(')____,  >(')____,  >(') ___,
//         (` =~~/    (` =~~/    (` =~~/    (` =~~/    (` =~~/
//    ~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~
/************************************************************************/
/*                                                                      */
/*                         uartd.c                               */
/*                                                                      */
/************************************************************************/
// Uart driver with mutex protection.

// Standard library includes
#include <stdbool.h>
#include <stdint.h>

// Tiva library includes
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/uart.h>
#include <inc/hw_memmap.h>

// RTOS Library includes
#include <FreeRTOS.h>
#include <semphr.h>

#include "uartd.h"
#include <stdbool.h>
// Global defines
#define SERIAL_BAUD_RATE 9600

// Define mutex for uart protection
typedef struct uart_s {
    bool is_init;
    SemaphoreHandle_t xMutex;
} uart_t;

uart_t uart0 = {.is_init = false, .xMutex = NULL};
// uart_t uart1;

/**
 * @brief Initialises uart system peripherals.
 *
 */
uart_t *uart0_init(void) {
    if (uart0.is_init == false) {
        SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
        GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
        GPIOPinConfigure(GPIO_PA0_U0RX);
        GPIOPinConfigure(GPIO_PA1_U0TX);
        UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), SERIAL_BAUD_RATE,
                            UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                                UART_CONFIG_PAR_NONE);
        UARTFIFOEnable(UART0_BASE);
        UARTEnable(UART0_BASE);
        // Create mutex
        uart0.xMutex = xSemaphoreCreateMutex();
        uart0.is_init = true;
    }
    return &uart0;
}

/**
 * @brief put string to uart port.
 *
 * @param str
 */
void uart_puts(uart_t *uart, char *str) {
    xSemaphoreTake(uart->xMutex, portMAX_DELAY);
    while (*str) {
        UARTCharPut(UART0_BASE, *str++);
    }
    xSemaphoreGive(uart->xMutex);
}