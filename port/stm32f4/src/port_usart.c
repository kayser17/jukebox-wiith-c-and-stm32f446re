/**
 * @file port_usart.c
 * @brief Portable functions to interact with the USART FSM library.
 * @author MAriano Lorenzo Kayser
 * @author Alejandro Gomez Ruiz
 * @date fecha
 */
/* Includes ------------------------------------------------------------------*/
/* Standard C libraries */
#include <string.h>
#include <stdlib.h>
#include "port_system.h"
#include "port_usart.h"
/* HW dependent libraries */

/* Global variables */
/**
 * @brief Array de estructuras que contiene las especificaciones de hardware de los USART.
 */
port_usart_hw_t usart_arr[] = {
[USART_0_ID] = {.p_usart = USART_0, .p_port_tx =USART_0_GPIO_TX, .p_port_rx = USART_0_GPIO_RX,
 .pin_tx = USART_0_PIN_TX, .pin_rx = USART_0_PIN_RX,.alt_func_tx = USART_0_AF_TX, .alt_func_rx = USART_0_AF_RX,
 .i_idx =0, .read_complete = false, .o_idx =0 , .write_complete = false}
};

/* Private functions */



/* Public functions */
/**
 * @brief Resetea un buffer dado a un valor específico.
 *
 * @param buffer Puntero al buffer a resetear.
 * @param length Longitud del buffer.
 */

void 	_reset_buffer (char *buffer, uint32_t length){
memset(buffer,EMPTY_BUFFER_CONSTANT,length);
}
/**
 * @brief Almacena los datos recibidos en el buffer de entrada del USART especificado.
 *
 * @param usart_id Identificador del USART.
 */

void port_usart_store_data (uint32_t usart_id){
    USART_TypeDef *p_usart = usart_arr[usart_id].p_usart;
    if(p_usart->DR != END_CHAR_CONSTANT){
        if(usart_arr[usart_id].i_idx >= USART_INPUT_BUFFER_LENGTH){
            usart_arr[usart_id].i_idx=0;
        }    
            usart_arr[usart_id].input_buffer[usart_arr[usart_id].i_idx]=p_usart->DR;
            usart_arr[usart_id].i_idx=usart_arr[usart_id].i_idx + 1;
            return;
    }
    else{
        usart_arr[usart_id].read_complete=true;
        usart_arr[usart_id].i_idx=0;
        return;
    }
}
/**
 * @brief Escribe los datos del buffer de salida en el USART especificado.
 *
 * @param usart_id Identificador del USART.
 */
void port_usart_write_data (uint32_t usart_id){
   if((usart_arr[usart_id].o_idx == USART_OUTPUT_BUFFER_LENGTH-1) || (usart_arr[usart_id].output_buffer[usart_arr[usart_id].o_idx] == END_CHAR_CONSTANT)){
        char data=usart_arr[usart_id].output_buffer[usart_arr[usart_id].o_idx];
        usart_arr[usart_id].p_usart->DR = data;
        usart_arr[usart_id].p_usart->CR1 &= ~(USART_CR1_TXEIE | USART_CR1_TCIE);
        usart_arr[usart_id].o_idx =0;
        usart_arr[usart_id].write_complete=true;
        }else{
        char data=usart_arr[usart_id].output_buffer[usart_arr[usart_id].o_idx];
        if(data != EMPTY_BUFFER_CONSTANT){
            usart_arr[usart_id].p_usart->DR = data;
            usart_arr[usart_id].o_idx++;  
        }
        }
}

/**
 * @brief Verifica si la transmisión del USART especificado se ha completado.
 *
 * @param usart_id Identificador del USART.
 * @return true si la transmisión está completa, false de lo contrario.
 */

bool     port_usart_tx_done (uint32_t usart_id){
    bool write_complete=usart_arr[usart_id].write_complete;
    return write_complete;
}
/**
 * @brief Verifica si la recepción del USART especificado se ha completado.
 *
 * @param usart_id Identificador del USART.
 * @return true si la recepción está completa, false de lo contrario.
 */

bool     port_usart_rx_done (uint32_t usart_id){
    bool read_complete=usart_arr[usart_id].read_complete;
    return read_complete;
}
/**
 * @brief Obtiene los datos del buffer de entrada del USART especificado.
 *
 * @param usart_id Identificador del USART.
 * @param p_input_data Puntero al buffer donde se almacenarán los datos.
 */

void     port_usart_get_from_input_buffer (uint32_t usart_id, char *p_input_data){
    memcpy(p_input_data,usart_arr[usart_id].input_buffer,USART_INPUT_BUFFER_LENGTH);
}
/**
 * @brief Obtiene el estado de transmisión del USART especificado.
 *
 * @param usart_id Identificador del USART.
 * @return true si la transmisión está en progreso, false de lo contrario.
 */

bool     port_usart_get_txr_status (uint32_t usart_id){
    USART_TypeDef *p_usart = usart_arr[usart_id].p_usart;
    return p_usart->SR & USART_SR_TXE;           
}
/**
 * @brief Copia datos al buffer de salida del USART especificado.
 *
 * @param usart_id Identificador del USART.
 * @param p_out_data Puntero a los datos que se copiarán.
 * @param nBytes Número de bytes a copiar.
 */
void port_usart_copy_to_output_buffer (uint32_t usart_id, char *p_out_data, uint32_t nBytes){
    memcpy(usart_arr[usart_id].output_buffer,p_out_data,nBytes);
}
/**
 * @brief Reinicia el buffer de entrada del USART especificado.
 *
 * @param usart_id Identificador del USART.
 */

void     port_usart_reset_input_buffer (uint32_t usart_id){
    _reset_buffer(usart_arr[usart_id].input_buffer,USART_INPUT_BUFFER_LENGTH);
    usart_arr[usart_id].read_complete=false;
}
/**
 * @brief Reinicia el buffer de salida del USART especificado.
 *
 * @param usart_id Identificador del USART.
 */

void     port_usart_reset_output_buffer (uint32_t usart_id){
    _reset_buffer(usart_arr[usart_id].output_buffer,USART_OUTPUT_BUFFER_LENGTH);
    usart_arr[usart_id].write_complete =false;
}
/**
 * @brief Habilita la interrupción de recepción del USART especificado.
 *
 * @param usart_id Identificador del USART.
 */
void 	port_usart_enable_rx_interrupt (uint32_t usart_id){
    USART_TypeDef *p_usart = usart_arr[usart_id].p_usart;
    p_usart->CR1 |= USART_CR1_RXNEIE;
}
/**
 * @brief Habilita la interrupción de transmisión del USART especificado.
 *
 * @param usart_id Identificador del USART.
 */
void 	port_usart_enable_tx_interrupt (uint32_t usart_id){
     usart_arr[usart_id].p_usart->CR1 |= USART_CR1_TXEIE;
}
/**
 * @brief Deshabilita la interrupción de recepción del USART especificado.
 *
 * @param usart_id Identificador del USART.
 */
void 	port_usart_disable_rx_interrupt (uint32_t usart_id){
    USART_TypeDef *p_usart = usart_arr[usart_id].p_usart;
    p_usart->CR1 &= ~USART_CR1_RXNEIE;
}
/**
 * @brief Deshabilita la interrupción de transmisión del USART.
 *
 * @param usart_id Identificador del USART.
 */
void 	port_usart_disable_tx_interrupt (uint32_t usart_id){
    USART_TypeDef *p_usart = usart_arr[usart_id].p_usart;
     p_usart->CR1 &= ~USART_CR1_TXEIE;
}
/**
 * @brief Inicializa el USART con las especificaciones de hardware y configuraciones proporcionadas.
 *
 * @param usart_id Identificador del USART.
 */

void port_usart_init(uint32_t usart_id)
{
    USART_TypeDef *p_usart = usart_arr[usart_id].p_usart;
    GPIO_TypeDef *p_port_tx = usart_arr[usart_id].p_port_tx;
    GPIO_TypeDef *p_port_rx = usart_arr[usart_id].p_port_rx;
    uint8_t pin_tx = usart_arr[usart_id].pin_tx;
    uint8_t pin_rx = usart_arr[usart_id].pin_rx;
    uint8_t alt_func_tx = usart_arr[usart_id].alt_func_tx;
    uint8_t alt_func_rx = usart_arr[usart_id].alt_func_rx;

    /* TO-DO alumnos: */

    port_system_gpio_config(p_port_tx, pin_tx, GPIO_MODE_ALTERNATE, GPIO_PUPDR_PUP);
    port_system_gpio_config(p_port_rx, pin_rx, GPIO_MODE_ALTERNATE, GPIO_PUPDR_PUP);
    port_system_gpio_config_alternate(p_port_tx, pin_tx, alt_func_tx);
    port_system_gpio_config_alternate(p_port_rx, pin_rx, alt_func_rx);

    
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
    p_usart->CR1 &= ~USART_CR1_UE;
    p_usart->CR1 &= ~USART_CR1_M;
    p_usart->CR2 &= ~USART_CR2_STOP;
    p_usart->CR1 &= ~(USART_CR1_PCE | USART_CR1_OVER8);
    p_usart->BRR = 0x683;
    p_usart->CR1 |= (USART_CR1_TE | USART_CR1_RE);

    port_usart_disable_rx_interrupt(usart_id);
    port_usart_disable_tx_interrupt(usart_id);
    p_usart->SR &= ~(USART_SR_RXNE | USART_SR_TC);

    // Enable USART interrupts globally
    if (p_usart == USART3)
    {
        NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 2, 0));
        NVIC_EnableIRQ(USART3_IRQn);
    }

    /* TO-DO alumnos: */
    p_usart->CR1 |= USART_CR1_UE;
    _reset_buffer(usart_arr[usart_id].input_buffer, USART_INPUT_BUFFER_LENGTH);
    _reset_buffer(usart_arr[usart_id].output_buffer, USART_OUTPUT_BUFFER_LENGTH);
}