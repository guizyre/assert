/******************************************************************************/
/**
 * @file    Orquestro.h
 * @addtogroup ORQUESTRO
 * @brief   Orquestrador principal: fornece as etapas de execucao para a main.
 * @{
 ******************************************************************************/

#ifndef _ORQUESTRO_H_
#define _ORQUESTRO_H_

/*******************************************************************************
 * PROTOTIPOS PUBLICOS (Etapas do Laco Principal)
 ******************************************************************************/

void App_InitModules(void);

void App_CheckSerialCommands(void);
void App_CheckButtonState(void);
void App_ProcessSamplingAndPWM(void);
void App_PrintStatusEverySecond(void);

#endif /* _APP_H_ */

/** @} DOXYGEN GROUP TAG END OF FILE */
