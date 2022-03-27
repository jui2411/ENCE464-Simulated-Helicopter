/**
 * @file quad_enc.h
 * @author Zach Preston         81883956
 * @author Hank Wu              34748017
 * @author Jason UI             27674749
 * @author Randipa Gunathilake  66941908
 * @brief Quadrature encoder driver
 * @version 0.1
 * @date 2021-08-20
 *
 */
//        >(')____,  >(')____,  >(')____,  >(')____,  >(') ___,
//         (` =~~/    (` =~~/    (` =~~/    (` =~~/    (` =~~/
//    ~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~
/************************************************************************/
/*                                                                      */
/*                            quad_enc.h                                */
/*                                                                      */
/************************************************************************/
#ifndef QUAD_ENC_H_
#define QUAD_ENC_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Quad encoder configuarion structure
 * 
 */
typedef struct quad_enc_cfg_s
{
  uint32_t enc_periph;
  uint32_t enc_port;
  uint32_t enc_a_pin;
  uint32_t enc_b_pin;

  uint32_t ref_periph;
  uint32_t ref_port;
  uint32_t ref_pin;
  uint32_t ticks_per_rev;
} quad_enc_cfg_t;

// Defines for OOD
struct quad_enc_dev_s;
typedef struct quad_enc_dev_s quad_enc_dev_t;
typedef quad_enc_dev_t *quad_enc_t;

/**
 * @brief Initialise and return a quad encoder object using configuration struct passed
 * 
 * @param cfg : Configuration struct used to init
 * @return quad_enc_t : Handler
 */
quad_enc_t quad_enc_init(quad_enc_cfg_t *cfg);

/**
 * @brief Get the current heading
 * 
 * @param enc : Encoder hander
 * @return uint32_t : Heading in degrees
 */
uint32_t quad_enc_heading_get(quad_enc_t enc);

/**
 * @brief Calculate the heading
 * 
 * @param enc : Encoder hander
 */
void quad_enc_heading_calc(quad_enc_t enc);

#ifdef __cplusplus
}
#endif

#endif