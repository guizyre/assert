/******************************************************************************/
/**
 * @file    Bsp.h
 * @addtogroup BSP
 * @brief   Camada de abstracao do hardware para o Projeto Final.
 * @{
 ******************************************************************************/

#ifndef _BSP_H_
#define _BSP_H_

/*******************************************************************************
 * INCLUDES NECESSARIOS
 ******************************************************************************/
#include "main.h"
#include <stdbool.h>

/*******************************************************************************
 * TIPOS DE DADOS PADRONIZADOS (Substituindo AssertTypes.h se necessario)
 ******************************************************************************/
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;

#ifndef dTRUE
#define dTRUE  true
#define dFALSE false
#endif

/*******************************************************************************
 * CONFIGURACOES E DEFINES PUBLICOS
 ******************************************************************************/

/// Numero total de LEDs disponiveis no painel
#define dBSP_NUMBER_OF_LEDS             3

/// Valor maximo do Timer do PWM (ARR configurado no CubeMX, ex: 100 para 100%)
#define dBSP_PWM_MAX_VALUE              100

/*******************************************************************************
 * TIPOS DE DADOS PUBLICOS
 ******************************************************************************/

/// Identificadores dos LEDs do painel
typedef enum
{
    eLED_1 = 0,
    eLED_2,
    eLED_3,
    eNUMBER_OF_LEDS
} ledId_t;

/*******************************************************************************
 * PROTOTIPOS PUBLICOS
 ******************************************************************************/

void Bsp_Init(void);

/* Controle dos LEDs (PWM) */
void Bsp_SetLedPwm(ledId_t id, u8 percent);

/* Leitura do Analógico */
u32 Bsp_AdcRead(void);

/* Controle de Flags (Temporizador e Botão) */
bool Bsp_GetSampleFlag(void);
void Bsp_ClearSampleFlag(void);
bool Bsp_GetButtonFlag(void);
void Bsp_ClearButtonFlag(void);

/* Comunicação Serial */
void Bsp_UartSendString(const char *str);

#endif /* _BSP_H_ */

/** @} DOXYGEN GROUP TAG END OF FILE */
