/**
 * @file main.c
 * @brief Main file.
 * @author Sistemas Digitales II
 * @date 2023-10-01
 */


/* Includes ------------------------------------------------------------------*/
/* Standard C libraries */
#include <stdio.h> // printf

/* HW libraries */
#include "port_system.h"
#include "fsm_button.h"
#include "port_button.h"
#include "fsm_usart.h"
#include "port_usart.h"
#include "fsm_buzzer.h"
#include "port_buzzer.h"
#include "melodies.h"
#include <string.h>
#include "fsm_jukebox.h"

/* Defines ------------------------------------------------------------------*/
#define 	ON_OFF_PRESS_TIME_MS 1500
#define 	NEXT_SONG_BUTTON_TIME_MS 300

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* Init board */
    port_system_init(); //Inicializa el sitema
    //Creamos las maquinas de estados
    fsm_t * p_fsm_user_button = fsm_button_new(BUTTON_0_ID); 
    fsm_t *p_fsm_usart = fsm_usart_new(USART_0_ID);
    fsm_t *p_fsm_buzzer = fsm_buzzer_new(BUZZER_0_ID);
    fsm_t *p_fsm_jukebox = fsm_jukebox_new(p_fsm_user_button,ON_OFF_PRESS_TIME_MS,p_fsm_usart,p_fsm_buzzer,NEXT_SONG_BUTTON_TIME_MS);

    /* Infinite loop */
    while (1)
    {
        //Utilizamos las maquinass de estados
        fsm_fire(p_fsm_user_button);
        fsm_fire(p_fsm_usart);
        fsm_fire(p_fsm_buzzer);
        fsm_fire(p_fsm_jukebox);

    } // End of while(1)
    //Destruimos las maquinas de estados
    fsm_destroy(p_fsm_user_button);
    fsm_destroy(p_fsm_usart);
    fsm_destroy(p_fsm_buzzer);
    fsm_destroy(p_fsm_jukebox);

    
    return 0;
}
