/******************************************************************************/
/**
 * @file    Button.h
 * @addtogroup BUTTON
 * @brief   Modulo responsavel pela logica de congelamento do sistema.
 * @{
 ******************************************************************************/

#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "Bsp.h"

/*******************************************************************************
 * PROTOTIPOS PUBLICOS
 ******************************************************************************/

void Button_Init(void);
void Button_Task(void);
bool Button_IsFrozen(void);

#endif /* _BUTTON_H_ */

/** @} DOXYGEN GROUP TAG END OF FILE */
