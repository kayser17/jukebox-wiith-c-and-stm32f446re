/**
 * @file fsm_buzzer.c
 * @brief Buzzer melody player FSM main file.
 * @author Mariano Lorenzo Kayser
 * @author Alejandro Gomez Ruiz
 * @date 4/4/2024
 */

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include "port_buzzer.h"
#include "fsm_buzzer.h"
#include "melodies.h"
#include "port_buzzer.h"

/* Standard C libraries */

/* Other libraries */

/* State machine input or transition functions */

/* State machine output or action functions */

/* Public functions */
/**
 * @brief  Inicia la reproducción de una nota en el buzzer
 *
 * @param p_this
 * @param freq
 * @param duration
 */
static void _start_note(fsm_t *p_this, double freq, uint32_t duration)
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    double duration_double=(double)duration;
    duration_double /= p_fsm->player_speed;
    port_buzzer_set_note_frequency(p_fsm->buzzer_id,freq);
    port_buzzer_set_note_duration(p_fsm->buzzer_id,duration_double);
}
/**
 * @brief  Comprueba si la melodía debe comenzar.
 *
 * @param p_this
 * @return true
 * @return false
 */
static bool check_melody_start(fsm_t *p_this)
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    if (p_fsm->p_melody != NULL && p_fsm->user_action == PLAY)
    {
        return true;
    }
    return false;
}
/**
 * @brief Comprueba si el reproductor debe comenzar.
 *
 * @param p_this
 * @return true
 * @return false
 */
static bool check_player_start(fsm_t *p_this)
{
    return check_melody_start(p_this);
}
/**
 * @brief Comprueba si la melodía ha terminado.
 *
 * @param p_this
 * @return true
 * @return false
 */
static bool check_end_melody(fsm_t *p_this)
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    if (p_fsm->note_index >= p_fsm->p_melody->melody_length)
    {
        return true;
    }
    return false;
}
/**
 * @brief Comprueba si la reproducción debe pausarse.
 *
 * @param p_this
 * @return true
 * @return false
 */
static bool check_pause(fsm_t *p_this)
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    if (p_fsm->user_action == PAUSE)
    {
        return true;
    }
    return false;
}
/**
 * @brief Comprueba si la reproducción debe reanudarse.
 *
 * @param p_this
 * @return true
 * @return false
 */
static bool check_resume(fsm_t *p_this)
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    if (p_fsm->user_action == PLAY)
    {
        return true;
    }
    return false;
}
/**
 * @brief Comprueba si el reproductor debe detenerse.
 *
 * @param p_this
 * @return true
 * @return false
 */
static bool check_player_stop(fsm_t *p_this)
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    if (p_fsm->user_action == STOP)
    {
        return true;
    }
    return false;
}
/**
 * @brief  Comprueba si debe reproducirse una nota.
 *
 * @param p_this
 * @return true
 * @return false
 */
static bool check_play_note(fsm_t *p_this)
{
    if (check_resume(p_this) && !check_end_melody(p_this))
    {
        return true;
    }
    return false;
}
/**
 * @brief Comprueba si la nota actual ha terminado.
 *
 * @param p_this
 * @return true
 * @return false
 */
static bool check_note_end(fsm_t *p_this)
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    return port_buzzer_get_note_timeout(p_fsm->buzzer_id);
}
/**
 * @brief Inicia la melodía.
 * 
 *
 * @param p_this
 */
static void do_melody_start(fsm_t *p_this)
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    _start_note(p_this, p_fsm->p_melody->p_notes[p_fsm->note_index], p_fsm->p_melody->p_durations[p_fsm->note_index]);
    p_fsm->note_index++;
}
/**
 * @brief Inicia el reproductor.
 *
 * @param p_this
 */
static void do_player_start(fsm_t *p_this)
{
    do_melody_start(p_this);
}
/**
 * @brief Finaliza la melodía.
 *
 * @param p_this
 */
static void do_end_melody(fsm_t *p_this)
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    port_buzzer_stop(p_fsm->buzzer_id);
    p_fsm->note_index=0;
    p_fsm->user_action=STOP;
    
}
/**
 * @brief Pausa la reproducción.
 *
 * @param p_this
 */
static void do_pause(fsm_t *p_this)
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
   port_buzzer_stop(p_fsm->buzzer_id);
}
/**
 * @brief Detiene el reproductor.
 *
 * @param p_this
 */
static void do_player_stop(fsm_t *p_this)
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    port_buzzer_stop(p_fsm->buzzer_id);
    p_fsm->note_index=0;
    
}
/**
 * @brief Termina la reproducción de la nota actual.
 *
 * @param p_this
 */
static void do_play_note(fsm_t *p_this)
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    _start_note(p_this,p_fsm->p_melody->p_notes[p_fsm->note_index],p_fsm->p_melody->p_durations[p_fsm->note_index]);
    p_fsm->note_index++;
}
/**
 * @brief
 *
 * @param p_this
 */
static void do_note_end(fsm_t *p_this)
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    port_buzzer_stop(p_fsm->buzzer_id);
}

static fsm_trans_t fsm_trans_buzzer[] = {
    {WAIT_START, check_player_start, WAIT_NOTE, do_player_start},
    {WAIT_NOTE, check_note_end, PLAY_NOTE, do_note_end},
    {PLAY_NOTE, check_play_note, WAIT_NOTE, do_play_note},
    {PLAY_NOTE, check_player_stop, WAIT_START, do_player_stop},
    {PLAY_NOTE, check_pause, PAUSE_NOTE, do_pause},
    {PAUSE_NOTE, check_resume, PLAY_NOTE, NULL},
    {PLAY_NOTE, check_end_melody, WAIT_MELODY, do_end_melody},
    {WAIT_MELODY, check_melody_start, WAIT_NOTE, do_melody_start},
    {-1, NULL, -1, NULL}

};

bool fsm_buzzer_check_activity(fsm_t *p_this)
{
     fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
     if(p_fsm->user_action==PLAY){
        return true;
     }
     return false;
}

void fsm_buzzer_set_melody(fsm_t *p_this, const melody_t *p_melody)
{
     fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
     melody_t *p= (melody_t *)p_melody;
     p_fsm->p_melody=p;
}

void fsm_buzzer_set_speed(fsm_t *p_this, double speed)
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    p_fsm->player_speed=speed;
}

void fsm_buzzer_set_action(fsm_t *p_this, uint8_t action)
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    p_fsm->user_action=action;
    if(action==STOP){
        p_fsm->note_index=0;
    }
}
uint8_t fsm_buzzer_get_action(fsm_t *p_this)
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    return p_fsm->user_action;
}
fsm_t *fsm_buzzer_new(uint32_t buzzer_id)
{
    fsm_t *p_fsm = malloc(sizeof(fsm_buzzer_t));
    fsm_buzzer_init(p_fsm, buzzer_id);
    return p_fsm;
}

void fsm_buzzer_init(fsm_t *p_this, uint32_t buzzer_id)
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    fsm_init(p_this, fsm_trans_buzzer);

    /* TO-DO alumnos */
    p_fsm->buzzer_id=buzzer_id;
    p_fsm->p_melody=NULL;
    p_fsm->note_index=0;
    p_fsm->user_action=STOP;
    p_fsm->player_speed=1.0;
    port_buzzer_init(p_fsm->buzzer_id);
}