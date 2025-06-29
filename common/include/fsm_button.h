/**
 * @file fsm_button.h
 * @brief Header for fsm_button.c file.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */

#ifndef FSM_BUTTON_H_
#define FSM_BUTTON_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* Other includes */
#include "fsm.h"

/* Defines and enums ----------------------------------------------------------*/
/* Enums */
enum  FSM_BUTTON {
  BUTTON_RELEASED = 0,
  BUTTON_RELEASED_WAIT,
  BUTTON_PRESSED,
  BUTTON_PRESSED_WAIT
};

/* Typedefs --------------------------------------------------------------------*/
typedef struct 
{
    fsm_t f; 
    uint32_t next_timeout; 
    uint32_t tick_pressed; 
    uint32_t duration; 
    uint32_t button_id;
    
} fsm_button_t;

/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Create a new button FSM.
 * 
 * @param debounce_time 
 * @param button_id 
 * @return fsm_t* 
 */
fsm_t * fsm_button_new ( uint32_t button_id);
/**
 * @brief Initialize a button FSM.
 * 
 * @param p_this 
 * @param debounce_time 
 * @param button_id 
 */
void fsm_button_init (fsm_t *p_this, uint32_t button_id);
/**
 * @brief Return the duration of the last button press.
 * 
 * @param p_this 
 * @return uint32_t 
 */
uint32_t fsm_button_get_duration (fsm_t *p_this);
/**
 * @brief Reset the duration of the last button press.
 * 
 * @param p_this 
 */
void fsm_button_reset_duration (fsm_t *p_this);
/**
 * @brief Check if the button FSM is active, or not.
 * 
 * @param p_this 
 * @return true 
 * @return false 
 */
bool fsm_button_check_activity (fsm_t *p_this);

#endif