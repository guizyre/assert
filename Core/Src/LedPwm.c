/******************************************************************************/
/**
 * @file    LedPwm.c
 * @addtogroup LED_PWM
 * @brief   Implementacao do gerenciamento logico dos LEDs.
 * @{
 ******************************************************************************/

#include "LedPwm.h"

/*******************************************************************************
 * ESTRUTURAS DE DADOS LOCAIS
 ******************************************************************************/

static struct
{
    ledId_t selectedLed;                     ///< O LED atualmente selecionado pela UART
    u8 dutyCache[dBSP_NUMBER_OF_LEDS];       ///< Memoria do ultimo valor (0 a 100%) de cada LED
} ledPwmData;

/*******************************************************************************
 * FUNCOES PUBLICAS
 ******************************************************************************/

/******************************************************************************/
/** @brief  Inicializa o gerenciador de LEDs.
 ******************************************************************************/
void LedPwm_Init(void)
{
    ledPwmData.selectedLed = eLED_1; // LED1 e o padrao ao ligar o sistema

    u8 i;
    for(i = 0; i < dBSP_NUMBER_OF_LEDS; i++)
    {
        ledPwmData.dutyCache[i] = 0;
    }
}

/******************************************************************************/
/** @brief  Altera qual LED esta selecionado para seguir o potenciometro.
 * @param  id: Identificador do LED (eLED_1, eLED_2, eLED_3).
 ******************************************************************************/
void LedPwm_SelectLed(ledId_t id)
{
    if(id < eNUMBER_OF_LEDS)
    {
        ledPwmData.selectedLed = id;
    }
}

/******************************************************************************/
/** @brief  Aplica o valor do ADC apenas ao LED selecionado no momento.
 * @param  percent: Intensidade de 0 a 100%.
 ******************************************************************************/
void LedPwm_UpdateSelected(u8 percent)
{
    // Atualiza a memoria local
    ledPwmData.dutyCache[ledPwmData.selectedLed] = percent;

    // Envia o comando para o hardware aplicar o novo PWM
    Bsp_SetLedPwm(ledPwmData.selectedLed, percent);
}

/******************************************************************************/
/** @brief  Retorna o duty cycle atual de um LED especifico (usado para o printf).
 * @param  id: Identificador do LED.
 * @retval Valor de 0 a 100%.
 ******************************************************************************/
u8 LedPwm_GetDuty(ledId_t id)
{
    if(id < eNUMBER_OF_LEDS)
    {
        return ledPwmData.dutyCache[id];
    }
    return 0;
}

/** @} DOXYGEN GROUP TAG END OF FILE */
