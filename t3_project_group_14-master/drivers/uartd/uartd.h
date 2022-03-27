/**
 * @file switch.h
 * @author Zach Preston         81883956
 * @author Hank Wu              34748017
 * @author Jason UI             27674749
 * @author Randipa Gunathilake  66941908
 * @brief Driver for mutex protected UART
 * @version 0.1
 * @date 2021-08-08
 *
 */
//        >(')____,  >(')____,  >(')____,  >(')____,  >(') ___,
//         (` =~~/    (` =~~/    (` =~~/    (` =~~/    (` =~~/
//    ~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~
/************************************************************************/
/*                                                                      */
/*                              uartd.h                                */
/*                                                                      */
/************************************************************************/
#ifndef _UARTD_H_
#define _UARTD_H_

#ifdef __cplusplus
extern "C" {
#endif

struct uart_s;
typedef struct uart_s uart_t;


/**
 * @brief Initialises uart system peripherals. A pointer to uart0 object would be return by this function.
 *
 */
uart_t* uart0_init(void);

/**
 * @brief put string to uart port.
 *
 * @param str
 */
void uart_puts(uart_t* uart, char* str);

#ifdef __cplusplus
}
#endif

#endif /* _UARTD_H_ */