/******************************************************************************/
/**
 * @file    Sampler.h
 * @addtogroup SAMPLER
 * @brief   Modulo responsavel pela filtragem (media) das leituras do ADC.
 * @{
 ******************************************************************************/

#ifndef _SAMPLER_H_
#define _SAMPLER_H_

#include "Bsp.h"

/*******************************************************************************
 * CONFIGURACOES E DEFINES PUBLICOS
 ******************************************************************************/

/// Numero de amostras para o calculo da media (100 amostras * 5ms = 500ms)
#define dSAMPLER_MAX_SAMPLES    100

/// Valor maximo do ADC de 12 bits (usado para converter para 0 a 100%)
#define dSAMPLER_ADC_MAX_VALUE  4095

/*******************************************************************************
 * PROTOTIPOS PUBLICOS
 ******************************************************************************/

void Sampler_Init(void);
void Sampler_ProcessSample(u32 rawAdcValue);
bool Sampler_IsAverageReady(void);
u8 Sampler_GetAveragePercent(void);

#endif /* _SAMPLER_H_ */

/** @} DOXYGEN GROUP TAG END OF FILE */
