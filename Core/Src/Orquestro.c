/******************************************************************************/
/**
 * @file    Orquestro.c
 * @addtogroup ORQUESTRO
 * @brief   Implementacao das etapas de orquestracao do Projeto Final.
 * @{
 ******************************************************************************/

#include "orquestro.h"
#include "Bsp.h"
#include "Sampler.h"
#include "LedPwm.h"
#include "Button.h"
#include "SerialCmd.h"
#include <stdio.h>

/*******************************************************************************
 * ESTRUTURAS DE DADOS LOCAIS
 ******************************************************************************/

static struct
{
    u16 tickCounter;     ///< Contador de tempo (incrementado a cada 5 ms)
    char txBuffer[128];  ///< Buffer para montagem da string de saida
} appData;

/*******************************************************************************
 * FUNCOES PUBLICAS
 ******************************************************************************/

/******************************************************************************/
/** @brief  Passo 0: Inicializa todos os submodulos do projeto.
 ******************************************************************************/
void App_InitModules(void)
{
    appData.tickCounter = 0;

    Bsp_Init();
    Sampler_Init();
    LedPwm_Init();
    Button_Init();
    SerialCmd_Init();
}

/******************************************************************************/
/** @brief  Passo 1: Verifica se chegou algum comando via USART3.
 ******************************************************************************/
void App_CheckSerialCommands(void)
{
    SerialCmd_Task();
}

/******************************************************************************/
/** @brief  Passo 2: Verifica o debouncing e atualiza o estado de congelamento.
 ******************************************************************************/
void App_CheckButtonState(void)
{
    Button_Task();
}

/******************************************************************************/
/** @brief  Passo 3: Se der 5ms, le o ADC, filtra e atualiza o PWM (se ativo).
 ******************************************************************************/
void App_ProcessSamplingAndPWM(void)
{
    // Verifica a flag do Timer 3 (Base de tempo de 5 ms)
    if (Bsp_GetSampleFlag() == dTRUE)
    {
        Bsp_ClearSampleFlag(); // Consome a flag
        appData.tickCounter++; // Avanca o tempo para a impressao do terminal

        // Executa a amostragem apenas se o sistema nao estiver travado pelo botao
        if (Button_IsFrozen() == dFALSE)
        {
            u32 adcVal = Bsp_AdcRead();
            Sampler_ProcessSample(adcVal);

            // Se acumulou 100 amostras (500ms), aplica ao LED
            if (Sampler_IsAverageReady() == dTRUE)
            {
                u8 percent = Sampler_GetAveragePercent();
                LedPwm_UpdateSelected(percent);
            }
        }
    }
}

/******************************************************************************/
/** @brief  Passo 4: Imprime o relatorio na tela a cada 1 segundo.
 ******************************************************************************/
void App_PrintStatusEverySecond(void)
{
    // 200 ticks de 5ms = 1000ms
    if (appData.tickCounter >= 200)
    {
        appData.tickCounter = 0; // Reinicia a contagem

        sprintf(appData.txBuffer,
                "VALUE: %d%% || LED1: %d%% aceso || LED2: %d%% aceso || LED3: %d%% aceso || STATE: %s\r\n",
                Sampler_GetAveragePercent(),
                LedPwm_GetDuty(eLED_1),
                LedPwm_GetDuty(eLED_2),
                LedPwm_GetDuty(eLED_3),
                Button_IsFrozen() ? "OFF" : "ON");

        Bsp_UartSendString(appData.txBuffer);
    }
}

/** @} DOXYGEN GROUP TAG END OF FILE */
