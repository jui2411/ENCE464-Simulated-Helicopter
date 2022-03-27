/**
 * @file pilot.h
 * @author Hank Wu              34748017
 * @brief Pilot stream data via a queue from user interface module and
 * interpret it into a heading goal and altitude goal
 *
 * @version 0.1
 * @date 2021-07-18
 *
 */
//        >(')____,  >(')____,  >(')____,  >(')____,  >(') ___,
//         (` =~~/    (` =~~/    (` =~~/    (` =~~/    (` =~~/
//    ~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~
/************************************************************************/
/*                                                                      */
/*                               pilot.h                                */
/*                                                                      */
/************************************************************************/

#ifndef _PILOT_H_
#define _PILOT_H_

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief get the heading goal.
 *
 * @return uint16_t
 */
uint16_t pilot_get_heading_goal(void);

/**
 * @brief get the altitude goal.
 *
 * @return uint16_t
 */
uint16_t pilot_get_altitude_goal(void);

/**
 * @brief Pilot task for RTOS. pilot_initialise() is required to be called
 * before passing this task to RTOS.
 *
 * @param args
 */
void pilot_task(void *args);

/**
 * @brief initialise package. Required to be called before passing pilot_task()
 * to RTOS.
 *
 */
void pilot_initialise(void);

#ifdef __cplusplus
}
#endif

#endif /* _PILOT_H_ */