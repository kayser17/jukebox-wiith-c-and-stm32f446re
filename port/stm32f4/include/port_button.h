/**
 * @file port_button.h
 * @brief Header for port_button.c file.
 * @author Alejandro Gómez Ruiz
 * @author Mariano Lorenzo Kayser
 * @date 08/02/2024
 */

#ifndef PORT_BUTTON_H_
#define PORT_BUTTON_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>
#include "port_system.h"

/* HW dependent includes */


/* Defines and enums ----------------------------------------------------------*/
#define 	BUTTON_0_ID 0
 
#define 	BUTTON_0_GPIO GPIOC
 
#define 	BUTTON_0_PIN 13
 
#define 	BUTTON_0_DEBOUNCE_TIME_MS 150
/* Defines */


/* Typedefs --------------------------------------------------------------------*/
typedef struct
{
    GPIO_TypeDef *p_port;
    uint32_t debounce_time;
    uint8_t pin;
    bool flag_pressed;
} port_button_hw_t;

/* Global variables */
extern port_button_hw_t buttons_arr[];
/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief cabecera de la funcion port_button_init
 * 
 * @param button_id 
 */
void port_button_init (uint32_t button_id);
/**
 * @brief  cabecera de la funcion port_button_is_pressed
 *
 * @param button_id 
 * @return true 
 * @return false 
 */
bool port_button_is_pressed (uint32_t button_id);
/**
 * @brief 
 * 
 * @param button_id 
 * @return uint32_t 
 */
uint32_t port_button_get_debouncetime(uint32_t button_id);
/**
 * @brief cabecera de la funcion port_button_get_tick
 * 
 * @return uint32_t 
 */
uint32_t port_button_get_tick ();
#endif