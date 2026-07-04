/******************************************************************************/
/**
 * @file    SerialCmd.h
 * @addtogroup SERIAL_CMD
 * @brief   Modulo de interpretacao de comandos via USART3.
 * @{
 ******************************************************************************/

#ifndef _SERIALCMD_H_
#define _SERIALCMD_H_

#include "Bsp.h"
#include "LedPwm.h"

/*******************************************************************************
 * PROTOTIPOS PUBLICOS
 ******************************************************************************/

void SerialCmd_Init(void);
void SerialCmd_Task(void);

#endif /* _SERIALCMD_H_ */

/** @} DOXYGEN GROUP TAG END OF FILE */
