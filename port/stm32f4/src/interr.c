/**
 * @file interr.c
 * @brief Interrupt service routines for the STM32F4 platform.
 * @author Sistemas Digitales II
 * @date 2024-01-01
 */
// Include HW dependencies:
#include "port_system.h"
#include "port_button.h"
#include "port_usart.h"
#include "port_buzzer.h"
// Include headers of different port elements:

//------------------------------------------------------
// INTERRUPT SERVICE ROUTINES
//------------------------------------------------------
/**
 * @brief Interrupt service routine for the System tick timer (SysTick).
 *
 * @note This ISR is called when the SysTick timer generates an interrupt.
 * The program flow jumps to this ISR and increments the tick counter by one millisecond.
 *
 * > **TO-DO alumnos:**
 * >
 * > ✅ 1. **Increment the System tick counter `msTicks` in 1 count.** To do so, use the function `port_system_get_millis()` and `port_system_get_millis()`.
 *
 * @warning **The variable `msTicks` must be declared volatile!** Just because it is modified by a call of an ISR, in order to avoid [*race conditions*](https://en.wikipedia.org/wiki/Race_condition). **Added to the definition** after *static*.
 *
 */

/**
 * @brief Incrementa el contador de milisegundos del sistema en 1 cada vez que se produce una interrupción de SysTick.
*/
void SysTick_Handler(void)
{
    uint32_t millis = port_system_get_millis();
    port_system_set_millis(millis+1);
}
/**
 * @brief Detecta y maneja la interrupción generada por el botón de usuario.
 * 
 */
void EXTI15_10_IRQHandler(void) {
    /* ISR user button */
    port_system_systick_resume();
    if (EXTI->PR & BIT_POS_TO_MASK(buttons_arr[BUTTON_0_ID].pin)){
        uint8_t pin = buttons_arr[BUTTON_0_ID].pin;
        GPIO_TypeDef *p_port = buttons_arr[BUTTON_0_ID].p_port;
        bool button = (p_port -> IDR & (1 << pin)) != 0;
        if(button){
            buttons_arr[BUTTON_0_ID].flag_pressed = false;
        } else {
            buttons_arr[BUTTON_0_ID].flag_pressed = true;
        }
        EXTI -> PR |= BIT_POS_TO_MASK(pin);
    }
}
/**
 * @brief  Gestiona las interrupciones de recepción y transmisión del USART3.
 * 
 */
void USART3_IRQHandler (void) {
    port_system_systick_resume();
if ((USART3 ->CR1 & USART_CR1_RXNEIE) && (USART3->SR & USART_SR_RXNE)){
    port_usart_store_data(USART_0_ID);
}
if ((USART3->CR1 & USART_CR1_TXEIE) && (USART3->SR & USART_SR_TXE)){
    port_usart_write_data(USART_0_ID);
}
}
void TIM2_IRQHandler(void){
    TIM2->SR &= ~TIM_SR_UIF;
    port_buzzer_hw_t *p_buzzer = &buzzers_arr[BUZZER_0_ID];
    p_buzzer->note_end = true;
}