/******************************************************************************/
/**
 * @file    LedPwm.h
 * @addtogroup LED_PWM
 * @brief   Modulo responsavel pela logica de selecao e duty cycle dos LEDs.
 * @{
 ******************************************************************************/

#ifndef _LEDPWM_H_
#define _LEDPWM_H_

#include "Bsp.h"

/*******************************************************************************
 * PROTOTIPOS PUBLICOS
 ******************************************************************************/

void LedPwm_Init(void);
void LedPwm_SelectLed(ledId_t id);
void LedPwm_UpdateSelected(u8 percent);
u8 LedPwm_GetDuty(ledId_t id);

#endif /* _LEDPWM_H_ */

/** @} DOXYGEN GROUP TAG END OF FILE */
