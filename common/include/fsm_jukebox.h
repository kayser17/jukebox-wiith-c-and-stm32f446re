/**
 * @file fsm_jukebox.h
 * @brief Encabezado para el archivo fsm_jukebox.c. Define la estructura y las funciones para la FSM del Jukebox.
 * 
 * Este archivo de encabezado contiene los includes necesarios, defines, enumeraciones, typedefs y prototipos de funciones
 * para implementar una máquina de estados finita (FSM) para un sistema de jukebox. La FSM maneja diferentes estados como
 * APAGADO, INICIO, ESPERA_COMANDO, DORMIR_MIENTRAS_APAGADO y DORMIR_MIENTRAS_ENCENDIDO. También gestiona transiciones basadas
 * en pulsaciones de botones y otras entradas.
 * 
 * @autores
 * Alejandro Gómez Ruiz
 * Mariano Lorenzo Kayser
 * 
 * @date 18/04/2024
 */

#ifndef FSM_JUKEBOX_H_
#define FSM_JUKEBOX_H_

/* Includes ------------------------------------------------------------------*/
/* Incluye estándar de C */
#include <stdint.h>
#include <fsm.h>
#include "melodies.h"

/* Otros includes */

/* Defines y enumeraciones --------------------------------------------------*/
/* Defines */
#define MELODIES_MEMORY_SIZE 10  /**< Define el número máximo de melodías que el jukebox puede almacenar */

/* Enumeraciones */
/**
 * @enum FSM_JUKEBOX
 * @brief Enumeración de los posibles estados para la FSM del jukebox.
 */
enum FSM_JUKEBOX
{
    OFF = 0,             /**< El jukebox está apagado */
    START_UP,            /**< El jukebox está iniciando */
    WAIT_COMMAND,        /**< El jukebox está esperando un comando */
    SLEEP_WHILE_OFF,     /**< El jukebox está en modo de sueño mientras está apagado */
    SLEEP_WHILE_ON       /**< El jukebox está en modo de sueño mientras está encendido */
};

/* Tipos definidos --------------------------------------------------*/
/**
 * @struct fsm_jukebox_t
 * @brief Estructura que representa la FSM del jukebox.
 * 
 * Esta estructura contiene todos los datos necesarios para gestionar la FSM del jukebox, incluyendo punteros a otras FSMs,
 * el índice de la melodía actual, los datos de las melodías y la información de temporización.
 */
typedef struct {
    fsm_t f;                            /**< Estructura base de la FSM */
    melody_t melodies[MELODIES_MEMORY_SIZE]; /**< Array de melodías */
    uint8_t melody_idx;                 /**< Índice de la melodía actual */
    char *p_melody;                     /**< Puntero a los datos de la melodía actual */
    fsm_t *p_fsm_button;                /**< Puntero a la FSM del botón */
    uint32_t on_off_press_time_ms;      /**< Tiempo en milisegundos para la pulsación del botón de encendido/apagado */
    fsm_t *p_fsm_usart;                 /**< Puntero a la FSM de USART */
    fsm_t *p_fsm_buzzer;                /**< Puntero a la FSM del buzzer */
    uint32_t next_song_press_time_ms;   /**< Tiempo en milisegundos para la pulsación del botón de la siguiente canción */
    double speed;                       /**< Velocidad de reproducción */
} fsm_jukebox_t;

/* Prototipos de funciones y explicación -------------------------------------*/
/**
 * @brief Crea una nueva instancia de la FSM del jukebox.
 * 
 * Esta función asigna memoria e inicializa una nueva FSM del jukebox con los parámetros dados.
 * 
 * @param p_fsm_button 
 * @param on_off_press_time_ms 
 * @param p_fsm_usart 
 * @param p_fsm_buzzer 
 * @param next_song_press_time_ms 
 * @return fsm_t* Puntero a la instancia de la FSM recién creada
 */
fsm_t * fsm_jukebox_new(fsm_t *p_fsm_button, uint32_t on_off_press_time_ms, fsm_t *p_fsm_usart, fsm_t *p_fsm_buzzer, uint32_t next_song_press_time_ms);

/**
 * @brief Inicializa una instancia existente de la FSM del jukebox.
 * 
 * Esta función configura una FSM del jukebox ya asignada con los parámetros dados.
 * 
 * @param p_this 
 * @param p_fsm_button 
 * @param on_off_press_time_ms 
 * @param p_fsm_usart 
 * @param p_fsm_buzzer 
 * @param next_song_press_time_ms 
 */
void fsm_jukebox_init(fsm_t *p_this, fsm_t *p_fsm_button, uint32_t on_off_press_time_ms, fsm_t *p_fsm_usart, fsm_t *p_fsm_buzzer, uint32_t next_song_press_time_ms);

#endif /* FSM_JUKEBOX_H_ */