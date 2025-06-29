/**
 * @file port_buzzer.c
 * @brief Funciones portables para interactuar con la biblioteca FSM del reproductor de melodías del zumbador.
 * @author Alejandro Gómez Ruiz
 * @author Mariano Lorenzo Kayser
 * @date 04/04/2024
 */

/* Inclusiones ------------------------------------------------------------------*/
#include <math.h>
#include "port_buzzer.h"
#include "port_system.h"

/* Bibliotecas estándar de C */

/* Bibliotecas dependientes del hardware */

/* Variables globales */
#define ALT_FUNC2_TIM3 2
#define TIM_AS_PWM1_MASK 0x0060
port_buzzer_hw_t buzzers_arr[] = {
    [BUZZER_0_ID] = {.p_port = BUZZER_0_GPIO, .pin = BUZZER_0_PIN, .alt_func = ALT_FUNC2_TIM3, .note_end = false}
};

/* Funciones privadas */

/**
 * @brief Configura el temporizador para el cálculo de la duración.
 * 
 * @param buzzer_id Identificador del zumbador.
 */
static void _timer_duration_setup(uint32_t buzzer_id)
{
    if (buzzer_id == BUZZER_0_ID)
    {
        RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
        TIM2->CR1 &= ~TIM_CR1_CEN;
        TIM2->CR1 |= TIM_CR1_ARPE;
        TIM2->SR = ~TIM_SR_UIF;
        TIM2->DIER |= TIM_DIER_UIE;

        /* Configurar interrupciones */
        NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 3, 0));
        NVIC_EnableIRQ(TIM2_IRQn);
    }
}

/**
 * @brief Configura el temporizador para la generación de PWM.
 * 
 * @param buzzer_id Identificador del zumbador.
 */
static void _timer_pwm_setup(uint32_t buzzer_id)
{
    if (buzzer_id == BUZZER_0_ID)
    {
        RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
        TIM3->CR1 &= ~TIM_CR1_CEN;
        TIM3->CR1 |= TIM_CR1_ARPE;
        TIM3->CNT = 0;
        TIM3->ARR = 0;
        TIM3->PSC = 0;
        TIM3->EGR |= TIM_EGR_UG;
        TIM3->CCER &= ~TIM_CCER_CC1E;
        TIM3->CCMR1 |= TIM_AS_PWM1_MASK;
        TIM3->CCMR1 |= TIM_CCMR1_OC1PE;
        TIM3->CR1 &= ~TIM_CR1_CEN;
    }
}

/* Funciones públicas */

/**
 * @brief Establece la duración de una nota en milisegundos.
 * 
 * @param buzzer_id Identificador del zumbador.
 * @param duration_ms Duración de la nota en milisegundos.
 */
void port_buzzer_set_note_duration(uint32_t buzzer_id, uint32_t duration_ms)
{
    if (buzzer_id == BUZZER_0_ID)
    {
        TIM2->CR1 &= ~TIM_CR1_CEN;
        TIM2->CNT = 0;
        double duration_as_double = (double)duration_ms;
        double sysclk_as_double = (double)SystemCoreClock;
        double PSC = 0;
        double ARR = ((sysclk_as_double * (duration_as_double / 1000)) / (PSC + 1)) - 1.0;
        ARR = round(ARR);
        while (ARR > 65535.0)
        {
            PSC += 1.0;
            ARR = ((sysclk_as_double * (duration_as_double / 1000)) / (PSC + 1)) - 1.0;
            ARR = round(ARR);
        }
        TIM2->ARR = (uint32_t)(round(ARR));
        TIM2->PSC = (uint32_t)(round(PSC));
        TIM2->EGR = TIM_EGR_UG;
        buzzers_arr[buzzer_id].note_end = false;
        TIM2->CR1 |= TIM_CR1_CEN;
    }
}

/**
 * @brief Establece la frecuencia de una nota en Hertzios.
 * 
 * @param buzzer_id Identificador del zumbador.
 * @param frequency_hz Frecuencia de la nota en Hertzios.
 */
void port_buzzer_set_note_frequency(uint32_t buzzer_id, double frequency_hz)
{
    if (buzzer_id == BUZZER_0_ID)
    {
        if (frequency_hz == 0)
        {
            TIM3->CR1 &= ~TIM_CR1_CEN;
            return;
        }
        else
        {
            TIM3->CR1 &= ~TIM_CR1_CEN;
            TIM3->CNT = 0;
            double sysclk_as_double = (double)SystemCoreClock;
            double PSC = 0;
            double ARR = ((sysclk_as_double * (1.0 / frequency_hz)) / (PSC + 1)) - 1.0;

            while (ARR > 65535)
            {
                PSC++;
                ARR = ((sysclk_as_double * (1.0 / frequency_hz)) / (PSC + 1)) - 1.0;
                ARR = round(ARR);
            }
            TIM3->ARR = (uint32_t)(round(ARR));
            TIM3->PSC = (uint32_t)(round(PSC));
            TIM3->CCR1 = (ARR + 1) * BUZZER_PWM_DC;
            TIM3->EGR = TIM_EGR_UG;
            TIM3->CCER |= TIM_CCER_CC1E;
            TIM3->CR1 |= TIM_CR1_CEN;
        }
    }
}
/**
 * @brief Devuelve si ha transcurrido el tiempo de duración de la nota.
 * 
 * @param buzzer_id Identificador del zumbador.
 * @return true si ha transcurrido el tiempo de duración de la nota, false de lo contrario.
 */
bool port_buzzer_get_note_timeout(uint32_t buzzer_id)
{
    port_buzzer_hw_t *p_buzzer = &buzzers_arr[buzzer_id];
    if (buzzer_id == BUZZER_0_ID)
    {
        return p_buzzer->note_end;
    }
    else
    {
        return false;
    }
}

/**
 * @brief Detiene la reproducción del zumbador.
 * 
 * @param buzzer_id Identificador del zumbador.
 */
void port_buzzer_stop(uint32_t buzzer_id)
{
    if (buzzer_id == BUZZER_0_ID)
    {
        TIM3->CCER &= ~TIM_CCER_CC1E;
        TIM3->CR1 &= ~TIM_CR1_CEN;
        TIM2->CR1 &= ~TIM_CR1_CEN;
    }
}

/**
 * @brief Inicializa el zumbador.
 * 
 * @param buzzer_id Identificador del zumbador.
 */
void port_buzzer_init(uint32_t buzzer_id)
{
    port_buzzer_hw_t buzzer = buzzers_arr[buzzer_id];

    /* Configuración de GPIO */
    port_system_gpio_config(buzzer.p_port, buzzer.pin, GPIO_MODE_ALTERNATE, GPIO_PUPDR_NOPULL);
    port_system_gpio_config_alternate(buzzer.p_port, buzzer.pin, buzzer.alt_func);
    _timer_duration_setup(buzzer_id);
    _timer_pwm_setup(buzzer_id);
}
