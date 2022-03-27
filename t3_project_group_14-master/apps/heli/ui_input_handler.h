/**
 * @file ui_input_handler.c
 * @author Zach Preston         81883956
 * @author Hank Wu              34748017
 * @author Jason UI             27674749
 * @author Randipa Gunathilake  66941908
 * @brief This Module monitors the user input and sent the data via a queue.
 *
 *  The module would only sent out data when the input status (i.g. button
 * status) changes. For the input element that does not change, a
 * <ELEMENT_TYPE>_NO_CHANGE would be sent instead.
 *  For example, if only button_up is pressed:
 *  The value in packet.button_up would equals to BUTTON_PRESSED, with the
 * packet.button_down equals to BUTTON_NO_CHANGE.
 *  NOTE: The initilise function i.e. ui_initialise() needed to be called
 *  before passing ui_task to RTOS. Otherwise, ui_task will return.
 * @version 0.1
 * @date 2021-07-14
 *
 */
//        >(')____,  >(')____,  >(')____,  >(')____,  >(') ___,
//         (` =~~/    (` =~~/    (` =~~/    (` =~~/    (` =~~/
//    ~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~
/************************************************************************/
/*                                                                      */
/*                      ui_input_handler.h                              */
/*                                                                      */
/************************************************************************/

#ifndef _UI_INPUT_HANDLER_H_
#define _UI_INPUT_HANDLER_H_

#ifdef __cplusplus
extern "C"
{
#endif

// Application Library includes
#include "button.h"
#include "switch.h"
#include "config/target.h"
#include "queue.h"

//The length of the queue.
#define UI_QUEUE_LENGTH 20
#define UI_TASK_PERIOD 10

// struct sent by the module via queue.
typedef struct ui_input_packet_s {
  button_state_t button_up;
  button_state_t button_down;
  button_state_t button_left;
  button_state_t button_right;
  switch_state_t sw_right;
  switch_state_t sw_left;
  int32_t potentiometer;
} ui_input_packet_t;

/**
 * @brief initilise user interface module. This function must be called before
 * passing ui_task to RTOS.
 *
 */
void ui_initialise(void);

/**
 * @brief RTOS task for monitoring user input.
 *
 * @param args
 */
void ui_task(void *args);

/**
 * @brief Get the queue of the user input data.
 *
 * @return QueueHandle_t
 */
QueueHandle_t ui_get_queue(void);

#ifdef __cplusplus
}
#endif

#endif /* _UI_INPUT_HANDLER_H_ */