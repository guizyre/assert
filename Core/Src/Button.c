/******************************************************************************/
/**
 * @file    Button.c
 * @addtogroup BUTTON
 * @brief   Implementacao do comportamento de alternancia do botao.
 * @{
 ******************************************************************************/

#include "Button.h"

/*******************************************************************************
 * ESTRUTURAS DE DADOS LOCAIS
 ******************************************************************************/

static struct
{
    bool isFrozen; ///< Estado atual do sistema (dTRUE = travado, dFALSE = atualizando)
} btnData;

/*******************************************************************************
 * FUNCOES PUBLICAS
 ******************************************************************************/

/******************************************************************************/
/** @brief  Inicializa a logica do botao (sistema comeca ativo).
 ******************************************************************************/
void Button_Init(void)
{
    btnData.isFrozen = dFALSE;
}

/******************************************************************************/
/** @brief  Tarefa continua que checa a flag fisica do botao e inverte o estado.
 ******************************************************************************/
void Button_Task(void)
{
    if (Bsp_GetButtonFlag() == dTRUE)
    {
        Bsp_ClearButtonFlag(); // Consome a flag gerada pela interrupcao

        // Alterna o estado logico
        btnData.isFrozen = !btnData.isFrozen;
    }
}

/******************************************************************************/
/** @brief  Retorna se o sistema esta congelado no momento.
 * @retval dTRUE (travado) ou dFALSE (ativo).
 ******************************************************************************/
bool Button_IsFrozen(void)
{
    return btnData.isFrozen;
}

/** @} DOXYGEN GROUP TAG END OF FILE */
