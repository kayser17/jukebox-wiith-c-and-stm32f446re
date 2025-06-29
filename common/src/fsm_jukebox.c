/**
 * @file fsm_jukebox.c
 * @brief Jukebox FSM main file.
 * @author Alejandro Gómez Ruiz
 * @author Mariano Lorenzo Kayser
 * @date 18/04/2024
 */

/* Includes ------------------------------------------------------------------*/
// Standard C includes
#include <string.h> // strcmp
#include <stdio.h>  // sprintf

// Other includes
#include "fsm.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fsm.h>
#include "fsm_jukebox.h"
#include "fsm_button.h"
#include "fsm_usart.h"
#include "fsm_buzzer.h"
#include "port_system.h"
#include "port_usart.h"
#include "melodies.h"

/* Defines ------------------------------------------------------------------*/
#define MAX(a, b) ((a) > (b) ? (a) : (b)) /*!< Macro to get the maximum of two values. */

/* Private functions */
/**
 * @brief Parse the message received by the USART.
 *
 * Given data received by the USART, this function parses the message and extracts the command and the parameter (if available).
 *
 * > 1. Split the message by space using function strtok() \n
 * > 2. If there's a token (command), copy it to the p_command variable. Otherwise, return false \n
 * > 3. Extract the parameter (if available). To do so, get the next token using function strtok(). If there's a token, copy it to the p_param variable. Otherwise, copy an empty string to the p_param variable \n
 * > 4. Return true indicating that the message has been parsed correctly \n
 *
 * @param p_message Pointer to the message received by the USART.
 * @param p_command Pointer to store the command extracted from the message.
 * @param p_param Pointer to store the parameter extracted from the message.
 * @return true if the message has been parsed correctly
 * @return false if the message has not been parsed correctly
 */

/**
 * @brief Parsea un mensaje para extraer el comando y el parámetro asociado.
 * 
 * @param p_message Mensaje a parsear.
 * @param p_command Puntero al buffer donde se almacenará el comando extraído.
 * @param p_param Puntero al buffer donde se almacenará el parámetro extraído.
 * @return true Si se pudo parsear el mensaje correctamente.
 * @return false Si no se encontró un comando válido en el mensaje.
 */
bool _parse_message(char *p_message, char *p_command, char *p_param)
{
    char *p_token = strtok(p_message, " "); // Divide el mensaje por espacios

    if (p_token != NULL)
    {
        strcpy(p_command, p_token);
    }
    else
    {

        return false;
    }

    // Extraer el parámetro (si está disponible)
    p_token = strtok(NULL, " "); // Obtener el siguiente token

    if (p_token != NULL)
    {
        strcpy(p_param, p_token);
    }
    else
    {
        strcpy(p_param, " "); // NO se encontró ningún parámetro
    }
    return true;
}

/**
 * @brief Establece la siguiente canción en la lista de reproducción.
 * 
 * @param p_fsm_jukebox Puntero a la estructura de la máquina de estados del jukebox.
 */
void _set_next_song(fsm_jukebox_t *p_fsm_jukebox)
{
    // Detiene la reproducción actual
    fsm_buzzer_set_action(p_fsm_jukebox->p_fsm_buzzer, STOP);

    // Incrementa el índice de la canción actual
    if (MELODIES_MEMORY_SIZE <= p_fsm_jukebox->melody_idx)
    {
        p_fsm_jukebox->melody_idx = 0;
    }
    else
    {
        p_fsm_jukebox->melody_idx++;
    }

    // Si la longitud de la melodía es menor o igual a cero, reinicia al principio de la lista
    if (p_fsm_jukebox->melodies[p_fsm_jukebox->melody_idx].melody_length <= 0)
    {
        p_fsm_jukebox->melody_idx = 0;
    }

    // Actualiza la melodía actual
    p_fsm_jukebox->p_melody = p_fsm_jukebox->melodies[p_fsm_jukebox->melody_idx].p_name;

    // Imprime el nombre de la melodía que se está reproduciendo
    printf("Reproduciendo: %s\n", p_fsm_jukebox->p_melody);

    // Establece la melodía y comienza la reproducción
    fsm_buzzer_set_melody(p_fsm_jukebox->p_fsm_buzzer, &p_fsm_jukebox->melodies[p_fsm_jukebox->melody_idx]);
    fsm_buzzer_set_action(p_fsm_jukebox->p_fsm_buzzer, PLAY);
}
/**
 * @brief Ejecuta un comando recibido por el jukebox.
 * 
 * @param p_fsm_jukebox Puntero a la estructura de la máquina de estados del jukebox.
 * @param p_command Comando recibido.
 * @param p_param Parámetro del comando (si lo hay).
 */
void _execute_command(fsm_jukebox_t *p_fsm_jukebox, char *p_command, char *p_param)
{
    // Comprueba el comando recibido y realiza la acción correspondiente
    if (strcmp(p_command, "play") == 0)
    {
        fsm_buzzer_set_action(p_fsm_jukebox->p_fsm_buzzer, PLAY);
    }
    else if (strcmp(p_command, "stop") == 0)
    {
        fsm_buzzer_set_action(p_fsm_jukebox->p_fsm_buzzer, STOP);
    }
    else if (strcmp(p_command, "pause") == 0)
    {
        fsm_buzzer_set_action(p_fsm_jukebox->p_fsm_buzzer, PAUSE);
    }
    else if (strcmp(p_command, "speed") == 0)
    {
        double param = atof(p_param);
        fsm_buzzer_set_speed(p_fsm_jukebox->p_fsm_buzzer, MAX(param, 0.1));
    }
    else if (strcmp(p_command, "next") == 0)
    {
        // Establece la siguiente canción en la lista de reproducción
        _set_next_song(p_fsm_jukebox);
    }
    else if (strcmp(p_command, "select") == 0)
    {
        // Selecciona una melodía específica en la lista de reproducción
        uint32_t melody_selected = atoi(p_param);
        if (p_fsm_jukebox->melodies[melody_selected].melody_length != 0)
        {
            fsm_buzzer_set_action(p_fsm_jukebox->p_fsm_buzzer, STOP);
            p_fsm_jukebox->melody_idx = melody_selected;
            fsm_buzzer_set_melody(p_fsm_jukebox->p_fsm_buzzer, &p_fsm_jukebox->melodies[melody_selected]);
            p_fsm_jukebox->p_melody = p_fsm_jukebox->melodies[melody_selected].p_name;
            printf("Reproduciendo: %s\n", p_fsm_jukebox->melodies[melody_selected].p_name);
            fsm_buzzer_set_action(p_fsm_jukebox->p_fsm_buzzer, PLAY);
        }
        else if (strcmp(p_command, "lista"))
        {
            // Imprime la lista de melodías disponibles
            printf("Lista de melodías: \n");
            for (int i = 0; i < MELODIES_MEMORY_SIZE; i++)
            {
                if (p_fsm_jukebox->melodies[i].melody_length != 0)
                {
                    printf("%d. %s\n", i, p_fsm_jukebox->melodies[i].p_name);
                }
            }
        }
        else
        {
            // Si no se encuentra la melodía especificada, envía un mensaje de error
            fsm_usart_set_out_data(p_fsm_jukebox->p_fsm_usart, "Error:Melody not found\n");
        }
    }
    else if (strcmp(p_command, "info") == 0)
    {
        // Envía información sobre la melodía actualmente en reproducción
        char msg[USART_OUTPUT_BUFFER_LENGTH];
        sprintf(msg, "Reproduciendo: %s\n", p_fsm_jukebox->p_melody);
        fsm_usart_set_out_data(p_fsm_jukebox->p_fsm_usart, msg);
    }
    else
    {
        // Si el comando no se reconoce, envía un mensaje de error
        fsm_usart_set_out_data(p_fsm_jukebox->p_fsm_usart, "Error:Command not found\n");
    }
}

/**
 * @brief Comprueba si el jukebox está encendido.
 * 
 * @param p_this Puntero a la estructura de la máquina de estados.
 * @return true Si el jukebox está encendido.
 * @return false Si el jukebox no está encendido.
 */
static bool check_on(fsm_t *p_this)
{
    fsm_jukebox_t *p_fsm_jukebox = (fsm_jukebox_t *)(p_this);
    uint32_t duration = fsm_button_get_duration(p_fsm_jukebox->p_fsm_button);

    if ((duration > 0) && (duration > p_fsm_jukebox->on_off_press_time_ms))
    {
        return true;
    }
    return false;
}

/**
 * @brief Comprueba si el jukebox está apagado.
 * 
 * @param p_this Puntero a la estructura de la máquina de estados.
 * @return true Si el jukebox está apagado.
 * @return false Si el jukebox no está apagado.
 */
static bool check_off(fsm_t *p_this)
{
    return check_on(p_this);
}

/**
 * @brief Comprueba si la melodía actual ha finalizado.
 * 
 * @param p_this Puntero a la estructura de la máquina de estados.
 * @return true Si la melodía ha finalizado.
 * @return false Si la melodía no ha finalizado.
 */
static bool check_melody_finished(fsm_t *p_this)
{
    fsm_jukebox_t *p_fsm_jukebox = (fsm_jukebox_t *)(p_this);
    if (fsm_buzzer_get_action(p_fsm_jukebox->p_fsm_buzzer) == STOP)
    {
        return true;
    }
    return false;
}

/**
 * @brief Comprueba si se ha presionado el botón de próxima canción.
 * 
 * @param p_this Puntero a la estructura de la máquina de estados.
 * @return true Si se ha presionado el botón de próxima canción.
 * @return false Si no se ha presionado el botón de próxima canción.
 */
static bool check_next_song_button(fsm_t *p_this)
{
    fsm_jukebox_t *p_fsm_jukebox = (fsm_jukebox_t *)(p_this);
    uint32_t duration = fsm_button_get_duration(p_fsm_jukebox->p_fsm_button);
    if ((duration > 0) && (duration >= p_fsm_jukebox->next_song_press_time_ms) && (duration < p_fsm_jukebox->on_off_press_time_ms))
    {
        return true;
    }
    return false;
}

/**
 * @brief Comprueba si se ha recibido un comando por USART.
 * 
 * @param p_this Puntero a la estructura de la máquina de estados.
 * @return true Si se ha recibido un comando por USART.
 * @return false Si no se ha recibido un comando por USART.
 */
static bool check_command_received(fsm_t *p_this)
{
    fsm_jukebox_t *p_fsm_jukebox = (fsm_jukebox_t *)(p_this);
    return fsm_usart_check_data_received(p_fsm_jukebox->p_fsm_usart);
}

/**
 * @brief Comprueba si hay actividad en el jukebox.
 * 
 * @param p_this Puntero a la estructura de la máquina de estados.
 * @return true Si hay actividad en el jukebox.
 * @return false Si no hay actividad en el jukebox.
 */
static bool check_activity(fsm_t *p_this)
{
    fsm_jukebox_t *p_fsm_jukebox = (fsm_jukebox_t *)(p_this);
    if (fsm_buzzer_check_activity(p_fsm_jukebox->p_fsm_buzzer) || fsm_button_check_activity(p_fsm_jukebox->p_fsm_button) || fsm_usart_check_activity(p_fsm_jukebox->p_fsm_usart))
    {
        return true;
    }
    return false;
}

/**
 * @brief Comprueba si no hay actividad en el jukebox.
 * 
 * @param p_this Puntero a la estructura de la máquina de estados.
 * @return true Si no hay actividad en el jukebox.
 * @return false Si hay actividad en el jukebox.
 */
static bool check_no_activity(fsm_t *p_this)
{
    return !check_activity(p_this);
}

/**
 * @brief Realiza las acciones necesarias cuando se enciende el jukebox.
 * 
 * @param p_this Puntero a la estructura de la máquina de estados.
 */
static void do_start_up(fsm_t *p_this)
{
    fsm_jukebox_t *p_fsm_jukebox = (fsm_jukebox_t *)(p_this);
    fsm_usart_t *p_fsm_usart = (fsm_usart_t *)p_fsm_jukebox->p_fsm_usart;
    fsm_button_reset_duration(p_fsm_jukebox->p_fsm_button);
    port_usart_enable_rx_interrupt(p_fsm_usart->usart_id);
    printf("Jukebox ON\n");
    fsm_buzzer_set_speed(p_fsm_jukebox->p_fsm_buzzer, 1.0);
    fsm_buzzer_set_melody(p_fsm_jukebox->p_fsm_buzzer, p_fsm_jukebox->melodies);
    fsm_buzzer_set_action(p_fsm_jukebox->p_fsm_buzzer, PLAY);
}

/**
 * @brief Inicia la reproducción de la jukebox.
 * 
 * @param p_this Puntero a la instancia de la máquina de estados.
 */
static void do_start_jukebox(fsm_t *p_this)
{
    fsm_jukebox_t *p_fsm_jukebox = (fsm_jukebox_t *)(p_this);
    p_fsm_jukebox->melody_idx = 1;
    p_fsm_jukebox->p_melody = happy_birthday_melody.p_name;
    fsm_buzzer_set_melody(p_fsm_jukebox->p_fsm_buzzer, &happy_birthday_melody);
}

/**
 * @brief Detiene la reproducción de la jukebox.
 * 
 * @param p_this Puntero a la instancia de la máquina de estados.
 */
static void do_stop_jukebox(fsm_t *p_this)
{
    fsm_jukebox_t *p_fsm_jukebox = (fsm_jukebox_t *)(p_this);
    fsm_buzzer_set_action(p_fsm_jukebox->p_fsm_buzzer, STOP);
    fsm_button_reset_duration(p_fsm_jukebox->p_fsm_button);
    fsm_usart_disable_rx_interrupt(p_fsm_jukebox->p_fsm_usart);
    fsm_usart_disable_tx_interrupt(p_fsm_jukebox->p_fsm_usart);
    printf("Jukebox OFF\n");
    fsm_buzzer_set_melody(p_fsm_jukebox->p_fsm_buzzer, &p_fsm_jukebox->melodies[4]);
    fsm_buzzer_set_action(p_fsm_jukebox->p_fsm_buzzer, PLAY);
}

/**
 * @brief Carga la siguiente canción en la jukebox.
 * 
 * @param p_this Puntero a la instancia de la máquina de estados.
 */
static void do_load_next_song(fsm_t *p_this)
{
    fsm_jukebox_t *p_fsm_jukebox = (fsm_jukebox_t *)(p_this);
    _set_next_song(p_fsm_jukebox);
    fsm_button_reset_duration(p_fsm_jukebox->p_fsm_button);
}

/**
 * @brief Lee el comando recibido por la jukebox.
 * 
 * @param p_this Puntero a la instancia de la máquina de estados.
 */
static void do_read_command(fsm_t *p_this)
{
    fsm_jukebox_t *p_fsm_jukebox = (fsm_jukebox_t *)(p_this);
    char p_message[USART_INPUT_BUFFER_LENGTH];
    char p_command[USART_INPUT_BUFFER_LENGTH];
    char p_param[USART_INPUT_BUFFER_LENGTH];
    fsm_usart_get_in_data(p_fsm_jukebox->p_fsm_usart, p_message);
    bool valid = _parse_message(p_message, p_command, p_param);
    if (valid) {
        _execute_command(p_fsm_jukebox, p_command, p_param);
    }
    fsm_usart_reset_input_data(p_fsm_jukebox->p_fsm_usart);
    p_message[0] = '\0';
}
/**
 * @brief Pone la máquina de estados en modo de suspensión mientras la jukebox está apagada.
 * 
 * @param p_this Puntero a la instancia de la máquina de estados.
 */
static void do_sleep_while_off(fsm_t *p_this)
{
    port_system_sleep();
}

/**
 * @brief Pone la máquina de estados en modo de suspensión mientras la jukebox está encendida.
 * 
 * @param p_this Puntero a la instancia de la máquina de estados.
 */
static void do_sleep_while_on(fsm_t *p_this)
{
    port_system_sleep();
}

/**
 * @brief Pone la máquina de estados en modo de suspensión mientras espera un comando.
 * 
 * @param p_this Puntero a la instancia de la máquina de estados.
 */
static void do_sleep_wait_command(fsm_t *p_this)
{
    port_system_sleep();
}

/**
 * @brief Pone la máquina de estados en modo de suspensión cuando la jukebox está apagada.
 * 
 * @param p_this Puntero a la instancia de la máquina de estados.
 */
static void do_sleep_off(fsm_t *p_this)
{
    port_system_sleep();
}


/* State machine input or transition functions */
fsm_trans_t fsm_trans_jukebox[] = {
    {OFF,check_on,START_UP,do_start_up},
    {OFF,check_no_activity,SLEEP_WHILE_OFF,do_sleep_off},
    {SLEEP_WHILE_OFF,check_no_activity,SLEEP_WHILE_OFF,do_sleep_while_off},
    {SLEEP_WHILE_OFF,check_activity,OFF,NULL},
    {START_UP,check_melody_finished,WAIT_COMMAND,do_start_jukebox},
    {WAIT_COMMAND,check_next_song_button,WAIT_COMMAND,do_load_next_song},
    {WAIT_COMMAND,check_command_received,WAIT_COMMAND,do_read_command},
    {WAIT_COMMAND,check_no_activity,SLEEP_WHILE_ON,do_sleep_wait_command},
    {WAIT_COMMAND,check_off,OFF,do_stop_jukebox},
    {SLEEP_WHILE_ON,check_no_activity,SLEEP_WHILE_ON,do_sleep_while_on},
    {SLEEP_WHILE_ON,check_activity,WAIT_COMMAND,NULL},
    {-1, NULL,-1,NULL}
};
/* Public functions */
fsm_t *fsm_jukebox_new(fsm_t *p_fsm_button, uint32_t on_off_press_time_ms, fsm_t *p_fsm_usart, fsm_t *p_fsm_buzzer, uint32_t next_song_press_time_ms)
{
    fsm_t *p_fsm = malloc(sizeof(fsm_jukebox_t));

    fsm_jukebox_init(p_fsm, p_fsm_button, on_off_press_time_ms, p_fsm_usart, p_fsm_buzzer, next_song_press_time_ms);

    return p_fsm;
}

void fsm_jukebox_init(fsm_t *p_this, fsm_t *p_fsm_button, uint32_t on_off_press_time_ms, fsm_t *p_fsm_usart, fsm_t *p_fsm_buzzer, uint32_t next_song_press_time_ms)
{
    fsm_jukebox_t *p_fsm_jukebox = (fsm_jukebox_t *)(p_this);
    fsm_init(p_this,fsm_trans_jukebox);
    p_fsm_jukebox -> p_fsm_button = p_fsm_button;
    p_fsm_jukebox-> on_off_press_time_ms = on_off_press_time_ms;
    p_fsm_jukebox -> p_fsm_usart = p_fsm_usart;
    p_fsm_jukebox -> p_fsm_buzzer = p_fsm_buzzer;
    p_fsm_jukebox -> next_song_press_time_ms=next_song_press_time_ms;
    p_fsm_jukebox -> melody_idx = 0;
    memset(p_fsm_jukebox->melodies,0,sizeof(p_fsm_jukebox->melodies));
    p_fsm_jukebox->melodies[0] = scale_melody;
    p_fsm_jukebox->melodies[1] = happy_birthday_melody;
    p_fsm_jukebox->melodies[2] = tetris_melody;
    p_fsm_jukebox->melodies[3] = himno_madrid_melody;
    p_fsm_jukebox->melodies[4] = windows_shutdown_melody;
}