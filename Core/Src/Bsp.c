/******************************************************************************/
/**
 * @file    Bsp.c
 * @addtogroup BSP
 * @brief   Camada de abstracao do hardware (Board Support Package).
 * @details
 * \n <b>Ferramentas:</b>
 * - STM32CubeIDE, STM32CubeMX.
 *
 * \n <b>Dependencias:</b>
 * - STM32 HAL Driver.
 *
 * \n <b>Observacoes:</b>
 * - Este arquivo e o unico que acessa diretamente as funcoes HAL_*.
 * - Toda configuracao de pinos e perifericos de baixo nivel esta aqui.
 * - Hardware: STM32F407VETx (ADC1, TIM3, TIM4, TIM2, USART3, EXTI).
 *
 * @{
 ******************************************************************************/

/*******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "Bsp.h"
#include <string.h>


/*******************************************************************************
 * VARIAVEIS EXTERNAS (Geradas pelo STM32CubeMX em main.c)
 ******************************************************************************/
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim2;  // Timer de Debounce
extern TIM_HandleTypeDef htim3;  // Timer de Amostragem (5 ms)
extern TIM_HandleTypeDef htim4;  // Timer do PWM (LEDs)
extern UART_HandleTypeDef huart3;

/*******************************************************************************
 * ESTRUTURAS DE DADOS LOCAIS
 ******************************************************************************/

/// Variaveis internas da BSP contendo as flags de controle
static struct
{
    bool sampleFlag;       // Flag acionada a cada 5 ms pelo TIM3
    bool buttonFlag;       //Flag acionada apos o debounce do botao no TIM2
    bool isDebouncing;     // Flag indicadora de debounce em andamento
} bspFlags;

/*******************************************************************************
 * PROTOTIPOS LOCAIS
 ******************************************************************************/

static bool Bsp_IsValidLedId(ledId_t id);

/*******************************************************************************
 * FUNCOES PUBLICAS
 ******************************************************************************/

/******************************************************************************/
/** @brief  Inicializacao da BSP.
 * @details Inicializa as variaveis internas, liga os temporizadores e o PWM.
 ******************************************************************************/
void Bsp_Init(void)
{
    // Limpa todas as flags
    bspFlags.sampleFlag   = dFALSE;
    bspFlags.buttonFlag   = dFALSE;
    bspFlags.isDebouncing = dFALSE;

    // Inicia os canais de PWM para todos os LEDs com duty cycle em 0%
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);

    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 0);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 0);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 0);

    // Inicia o Timer de amostragem (5 ms) com interrupcao
    HAL_TIM_Base_Start_IT(&htim3);
}

/******************************************************************************/
/** @brief  Ajusta a intensidade (PWM) de um LED.
 * @param  id: identificador do LED (eLED_1, eLED_2, eLED_3).
 * @param  percent: intensidade de 0 a 100%.
 ******************************************************************************/
void Bsp_SetLedPwm(ledId_t id, u8 percent)
{
    if(Bsp_IsValidLedId(id) == dTRUE)
    {
        // Trava o valor maximo em 100%
        if(percent > dBSP_PWM_MAX_VALUE)
        {
            percent = dBSP_PWM_MAX_VALUE;
        }

        switch(id)
        {
            case eLED_1:
                __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, percent);
                break;
            case eLED_2:
                __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, percent);
                break;
            case eLED_3:
                __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, percent);
                break;
            default:
                break;
        }
    }
}

/******************************************************************************/
/** @brief  Efetua a leitura do ADC por polling.
 * @retval Valor digitalizado de 12 bits (0 a 4095).
 ******************************************************************************/
u32 Bsp_AdcRead(void)
{
    u32 adcValue = 0;

    // Dispara a leitura por software e aguarda (polling rapido)
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)    {
        adcValue = HAL_ADC_GetValue(&hadc1);
    }
    HAL_ADC_Stop(&hadc1);

    return adcValue;
}

/******************************************************************************/
/** @brief  Retorna o estado da flag de amostragem.
 * @retval dTRUE se o timer estourou (5 ms), dFALSE caso contrario.
 ******************************************************************************/
bool Bsp_GetSampleFlag(void)
{
    return bspFlags.sampleFlag;
}

/******************************************************************************/
/** @brief  Limpa a flag de amostragem.
 ******************************************************************************/
void Bsp_ClearSampleFlag(void)
{
    bspFlags.sampleFlag = dFALSE;
}

/******************************************************************************/
/** @brief  Retorna o estado da flag do botao (apos debounce).
 * @retval dTRUE se o botao foi pressionado e validado, dFALSE caso contrario.
 ******************************************************************************/
bool Bsp_GetButtonFlag(void)
{
    return bspFlags.buttonFlag;
}

/******************************************************************************/
/** @brief  Limpa a flag do botao.
 ******************************************************************************/
void Bsp_ClearButtonFlag(void)
{
    bspFlags.buttonFlag = dFALSE;
}

/******************************************************************************/
/** @brief  Envia uma string de texto pela USART3 (TX).
 * @param  str: Ponteiro para a string terminada em nulo.
 ******************************************************************************/
void Bsp_UartSendString(const char *str)
{
    HAL_UART_Transmit(&huart3, (u8*)str, strlen(str), HAL_MAX_DELAY);
}

/*******************************************************************************
 * FUNCOES LOCAIS E CALLBACKS (INTERRUPCOES)
 ******************************************************************************/

/******************************************************************************/
/** @brief  Verifica se o identificador de LED e valido.
 * @param  id: identificador do LED a ser verificado.
 * @retval dTRUE se o id e valido, dFALSE caso contrario.
 ******************************************************************************/
static bool Bsp_IsValidLedId(ledId_t id)
{
    return (id < eNUMBER_OF_LEDS) ? dTRUE : dFALSE;
}

/******************************************************************************/
/** @brief  Callback de estouro dos Timers (TIM3 = Amostragem, TIM2 = Debounce).
 ******************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    // Timer de Amostragem (5 ms)
    if (htim->Instance == TIM3)
    {
        bspFlags.sampleFlag = dTRUE;
    }

    // Timer de Debounce (Validacao do botao apos os milissegundos programados)
    if (htim->Instance == TIM2)
    {
        // Para o timer de debounce para ele nao rodar infinitamente
        HAL_TIM_Base_Stop_IT(&htim2);

        // Verifica fisicamente se o botao continua pressionado (ajuste GPIO_PIN_SET/RESET conforme hardware)
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_SET)
        {
            bspFlags.buttonFlag = dTRUE;
        }

        // Limpa a interrupcao pendente do pino e libera o sistema para um novo clique
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
        NVIC_EnableIRQ(EXTI1_IRQn);

        bspFlags.isDebouncing = dFALSE;
    }
}

/******************************************************************************/
/** @brief  Callback da Interrupcao Externa (EXTI) do Botao.
 ******************************************************************************/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    // Tratamento para o pino do botao (Exemplo: PA1)
    if (GPIO_Pin == GPIO_PIN_1)
    {
        if (bspFlags.isDebouncing == dFALSE)
        {
            bspFlags.isDebouncing = dTRUE;

            // Desativa a interrupcao do botao temporariamente para evitar ruido (bouncing)
            NVIC_DisableIRQ(EXTI1_IRQn);

            // Inicia o Timer 2 para fazer a checagem segura (ex: apos 20ms a 50ms)
            // O tratamento real ocorrera na funcao HAL_TIM_PeriodElapsedCallback do TIM2
            HAL_TIM_Base_Start_IT(&htim2);
        }
    }
}

/** @} DOXYGEN GROUP TAG END OF FILE */
