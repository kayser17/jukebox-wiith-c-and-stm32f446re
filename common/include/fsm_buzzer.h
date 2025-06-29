/**
 * @file fsm_buzzer.h
 * @brief Header for fsm_buzzer.c file.
 * @author Mariano Lorenzo Kayser
 * @author Alejandro Gomez Ruiz
 * @date 4/4/2024
 */

#ifndef FSM_BUZZER_H_
#define FSM_BUZZER_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>
#include <fsm.h>
#include "melodies.h"

/* Other includes */

/* HW dependent includes */


/* Defines and enums ----------------------------------------------------------*/
/* Enums */
enum  	FSM_BUZZER {
  WAIT_START = 0,
  PLAY_NOTE,
  PAUSE_NOTE,
  WAIT_NOTE,
  WAIT_MELODY
};
enum  	USER_ACTIONS {
  STOP = 0,
  PLAY,
  PAUSE
};

/* Typedefs --------------------------------------------------------------------*/
typedef struct{
fsm_t f;
melody_t *p_melody;
uint32_t note_index;
uint8_t buzzer_id;
uint8_t user_action;
double player_speed; 
} fsm_buzzer_t;



/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Verifica la actividad del buzzer.

 * 
 * @param p_this 
 * @return true 
 * @return false 
 */
bool 	fsm_buzzer_check_activity (fsm_t *p_this);
/**
 * @brief Establece la melodía que debe reproducir el buzzer.

 * 
 * @param p_this 
 * @param p_melody 
 */
void 	fsm_buzzer_set_melody (fsm_t *p_this, const melody_t *p_melody);
/**
 * @brief Establece la velocidad de reproducción del buzzer.
 * 
 * @param p_this 
 * @param speed 
 */
void 	fsm_buzzer_set_speed (fsm_t *p_this, double speed);
/**
 * @brief Establece la acción del usuario.
 * 
 * @param p_this 
 * @param action 
 */
void 	fsm_buzzer_set_action (fsm_t *p_this, uint8_t action);
/**
 * @brief Obtiene la acción actual del usuario.
 * 
 * @param p_this 
 * @return uint8_t 
 */
uint8_t 	fsm_buzzer_get_action (fsm_t *p_this);
/**
 * @brief Crea una nueva instancia de la máquina de estados finita del buzzer.
 * 
 * @param buzzer_id 
 * @return fsm_t* 
 */
fsm_t * 	fsm_buzzer_new (uint32_t buzzer_id);
/**
 * @brief Inicializa la máquina de estados finita del buzzer
 * 
 * @param p_this 
 * @param buzzer_id 
 */
void 	fsm_buzzer_init (fsm_t *p_this, uint32_t buzzer_id);

#endif /* FSM_BUZZER_H_ */