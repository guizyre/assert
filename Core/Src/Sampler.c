/******************************************************************************/
/**
 * @file    Sampler.c
 * @addtogroup SAMPLER
 * @brief   Implementacao do modulo de amostragem e filtragem.
 * @{
 ******************************************************************************/

#include "Sampler.h"

/*******************************************************************************
 * ESTRUTURAS DE DADOS LOCAIS
 ******************************************************************************/

static struct
{
    u32 accumulator;       ///< Soma das leituras brutas do ADC
    u8  sampleCount;       ///< Contador de amostras atuais (0 a 100)
    u8  averagePercent;    ///< Ultima media calculada (0 a 100%)
    bool isReady;          ///< Flag que avisa se a media de 500ms esta pronta
} samplerData;

/*******************************************************************************
 * FUNCOES PUBLICAS
 ******************************************************************************/

/******************************************************************************/
/** @brief  Inicializa o modulo de amostragem.
 ******************************************************************************/
void Sampler_Init(void)
{
    samplerData.accumulator    = 0;
    samplerData.sampleCount    = 0;
    samplerData.averagePercent = 0;
    samplerData.isReady        = dFALSE;
}

/******************************************************************************/
/** @brief  Acumula uma nova amostra e calcula a media se atingir o limite.
 * @param  rawAdcValue: Valor bruto lido pelo ADC (0 a 4095).
 ******************************************************************************/
void Sampler_ProcessSample(u32 rawAdcValue)
{
    samplerData.accumulator += rawAdcValue;
    samplerData.sampleCount++;

    if (samplerData.sampleCount >= dSAMPLER_MAX_SAMPLES)
    {
        // Calcula a media aritmetica dos valores brutos
        u32 averageRaw = samplerData.accumulator / dSAMPLER_MAX_SAMPLES;

        // Converte o valor medio (0 a 4095) para a escala de 0 a 100%
        samplerData.averagePercent = (u8)((averageRaw * 100) / dSAMPLER_ADC_MAX_VALUE);

        // Reseta as variaveis para a proxima janela de amostragem
        samplerData.accumulator = 0;
        samplerData.sampleCount = 0;

        // Sinaliza que uma nova media esta disponivel
        samplerData.isReady = dTRUE;
    }
}

/******************************************************************************/
/** @brief  Verifica se uma nova media foi calculada e limpa a flag.
 * @retval dTRUE se a media estiver pronta, dFALSE caso contrario.
 ******************************************************************************/
bool Sampler_IsAverageReady(void)
{
    if (samplerData.isReady == dTRUE)
    {
        samplerData.isReady = dFALSE;
        return dTRUE;
    }
    return dFALSE;
}

/******************************************************************************/
/** @brief  Retorna o ultimo valor de porcentagem calculado.
 * @retval Valor de 0 a 100%.
 ******************************************************************************/
u8 Sampler_GetAveragePercent(void)
{
    return samplerData.averagePercent;
}

/** @} DOXYGEN GROUP TAG END OF FILE */
