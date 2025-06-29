/**
 * @file port_button.c
 * @brief File containing functions related to the HW of the button.
 *
 * This file defines an internal struct which contains the HW information of the button.
 *
 * @author alejandro gomez ruiz
 * @author mariano lorenzo kayser
 * @date 29/02/2024
 */


/* Includes ------------------------------------------------------------------*/
#include "port_button.h"

/* Global variables ------------------------------------------------------------*/
/**
 * @brief  Array de estructuras que contiene las especificaciones de hardware de los botones.
 * 
 */
port_button_hw_t buttons_arr[] = {
    [BUTTON_0_ID] = {.p_port = BUTTON_0_GPIO, .pin= BUTTON_0_PIN,.debounce_time = BUTTON_0_DEBOUNCE_TIME_MS, .flag_pressed = false}
};
/**
 * @brief Configura las especificaciones de hardware de un botón dado.
 *
 * @param button_id Identificador del botón.
 */
void port_button_init(uint32_t button_id)
{
    GPIO_TypeDef *p_port = buttons_arr[button_id].p_port;
    uint8_t pin = buttons_arr[button_id].pin;
    /* TO-DO alumnos */
    port_system_gpio_config(p_port,pin,GPIO_MODE_IN,GPIO_PUPDR_NOPULL);
    port_system_gpio_config_exti(p_port,pin,(TRIGGER_FALLING_EDGE | TRIGGER_ENABLE_INTERR_REQ | TRIGGER_RISING_EDGE));
    port_system_gpio_exti_enable(pin,1,0);
}
uint32_t port_button_get_debouncetime(uint32_t button_id){
    return buttons_arr[button_id].debounce_time;
}
/**
 * @brief Retorna el estado del botón (presionado o no).
 *
 * @param button_id Identificador del botón.
 * @return true si el botón está presionado, false de lo contrario.
 */
bool port_button_is_pressed (uint32_t button_id) {
    return buttons_arr[button_id].flag_pressed;
}
/**
 * @brief Retorna el contador del tick del sistema en milisegundos.
 *
 * @return Contador del tick del sistema en milisegundos.
 */

uint32_t port_button_get_tick (){
    return port_system_get_millis();
}