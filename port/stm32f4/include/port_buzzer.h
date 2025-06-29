/**
 * @file port_buzzer.h
 * @brief Header for port_buzzer.c file.
 * @author Alejandro Gómez Ruiz
 * @author Mariano Lorenzo Kayser
 * @date 04/04/2023
 */
#ifndef PORT_BUZZER_H_
#define PORT_BUZZER_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "port_system.h"
/* Standard C includes */
/**
 * @brief 
 * 
 */
struct port_buzzer_hw_t;

/* HW dependent includes */

/* Defines and enums ----------------------------------------------------------*/
/* Defines */

/**
 * @brief 
 * 
 */
#define BUZZER_0_ID 0

#define BUZZER_0_GPIO GPIOA

#define BUZZER_0_PIN 6

#define BUZZER_PWM_DC 0.5

    /* Typedefs --------------------------------------------------------------------*/
    /**
     * @brief Estructura que representa el hardware del buzzer.
     * 
     */
    typedef struct{
        GPIO_TypeDef *p_port;
        uint8_t pin;
        uint8_t alt_func;
        bool note_end;
    }port_buzzer_hw_t;

    /* Global variables */

    /**
     * @brief Matriz que contiene las configuraciones de hardware para los buzzers.

     * 
     */
    extern port_buzzer_hw_t buzzers_arr[];

/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Inicializa el hardware del buzzer.
 * 
 * @param buzzer_id 
 */
void port_buzzer_init(uint32_t buzzer_id);
/**
 * @brief  Configura la duración de la nota del buzzer especificado.
 * 
 * @param buzzer_id 
 * @param duration_ms 
 */
void port_buzzer_set_note_duration(uint32_t buzzer_id, uint32_t duration_ms);
/**
 * @brief Configura la frecuencia de la nota del buzzer especificado.
 * 
 * @param buzzer_id 
 * @param frequency_hz 
 */
void port_buzzer_set_note_frequency(uint32_t buzzer_id, double frequency_hz);
/**
 * @brief  Obtiene el estado de finalización de la nota del buzzer especificado.
 * 
 * @param buzzer_id 
 * @return true 
 * @return false 
 */
bool port_buzzer_get_note_timeout(uint32_t buzzer_id);
/**
 * @brief Detiene la reproducción de la nota del buzzer especificado.
 * 
 * @param buzzer_id 
 */
void port_buzzer_stop(uint32_t buzzer_id);

#endif