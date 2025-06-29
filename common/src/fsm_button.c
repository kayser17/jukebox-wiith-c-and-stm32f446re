/**
 * @file fsm_button.c
 * @brief Button FSM main file.
 * @author alejandro gomez ruiz
 * @author mariano lorenzo kayser
 * @date 29/02/2024
 */

/* Includes ------------------------------------------------------------------*/
#include "fsm_button.h"
#include "port_button.h"


/* State machine input or transition functions */

/* State machine output or action functions */


/* Other auxiliary functions */

/**
 * @brief Comprueba si el botón está presionado.
 * 
 * @param p_this 
 * @return true 
 * @return false 
 */
static bool check_button_pressed (fsm_t *p_this){
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    return port_button_is_pressed(p_fsm->button_id);
}
/**
 * @brief Comprueba si el botón ha sido soltado.
 * 
 * @param p_this 
 * @return true 
 * @return false 
 */
static bool check_button_released (fsm_t *p_this){
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    return !port_button_is_pressed(p_fsm -> button_id);
}
/**
 * @brief Comprueba si ha pasado el tiempo de rebote.
 * 
 * @param p_this 
 * @return true 
 * @return false 
 */
static bool check_timeout (fsm_t *p_this){
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    uint32_t tick = port_button_get_tick();
    if(tick > p_fsm -> next_timeout){
        return true;
    } else {
        return false;
    }
}

/**
 * @brief Almacena la duración de la pulsación del botón.
 * 
 * @param p_this 
 */
static void do_set_duration (fsm_t *p_this){
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    uint32_t tick = port_button_get_tick();
    //p_fsm->duration = p_fsm -> duration + p_fsm -> tick_pressed;
    p_fsm->duration = tick - p_fsm -> tick_pressed;
    p_fsm -> next_timeout = tick + port_button_get_debouncetime(p_fsm -> button_id);
}
/**
 * @brief Almacena el tiempo del sistema cuando se presionó el botón.
 * 
 * @param p_this 
 */
static void do_store_tick_pressed (fsm_t *p_this){
     fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
     uint32_t tick = port_button_get_tick();

     p_fsm -> tick_pressed = tick;
     p_fsm -> next_timeout = tick + port_button_get_debouncetime(p_fsm -> button_id);

}
/**
 * @brief Array representando la tabla de transiciones de la FSM del botón.
 * 
 */
static fsm_trans_t 	fsm_trans_button [] = {
    {BUTTON_RELEASED, check_button_pressed,BUTTON_PRESSED_WAIT,do_store_tick_pressed},
    {BUTTON_PRESSED_WAIT, check_timeout,BUTTON_PRESSED,NULL},
    {BUTTON_PRESSED,check_button_released,BUTTON_RELEASED_WAIT,do_set_duration},
    {BUTTON_RELEASED_WAIT,check_timeout,BUTTON_RELEASED,NULL},
    {-1, NULL,-1,NULL}

};
/**
 * @brief Crea una nueva instancia de la FSM para el botón.

 * 
 * @param debounce_time 
 * @param button_id 
 * @return fsm_t* 
 */
fsm_t *fsm_button_new( uint32_t button_id)
{
    fsm_t *p_fsm = malloc(sizeof(fsm_button_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    fsm_button_init(p_fsm, button_id);
    return p_fsm;
}
/**
 * @brief Inicializa una instancia de la FSM para el botón.
 *
 * @param p_this Puntero a la instancia de la FSM.
 * @param debounce_time Tiempo de rebote.
 * @param button_id Identificación del botón.
 */
void fsm_button_init(fsm_t *p_this, uint32_t button_id)
{
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    fsm_init(p_this, fsm_trans_button);
    /* TO-DO alumnos: */
    p_fsm -> button_id = button_id;
    p_fsm -> tick_pressed = 0;
    p_fsm -> duration = 0;
    port_button_init(button_id);
}
/**
 * @brief Retorna la duración de la última pulsación del botón.
 *
 * @param p_this Puntero a la instancia de la FSM.
 * @return Duración de la última pulsación del botón.
 */

uint32_t 	fsm_button_get_duration (fsm_t *p_this){
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    return p_fsm -> duration;
}
/**
 * @brief Reinicia la duración de la última pulsación del botón.
 *
 * @param p_this Puntero a la instancia de la FSM.
 */

void 	fsm_button_reset_duration (fsm_t *p_this){
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    p_fsm -> duration = 0; 
}

bool fsm_button_check_activity(fsm_t *p_this)
{
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    if (p_fsm->f.current_state == BUTTON_RELEASED)
    {
        return false;
    }
     return true;
}

