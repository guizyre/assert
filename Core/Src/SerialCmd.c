/******************************************************************************/
/**
 * @file    SerialCmd.c
 * @addtogroup SERIAL_CMD
 * @brief   Implementacao da maquina de estados para recebimento de texto.
 * @{
 ******************************************************************************/

#include "SerialCmd.h"

extern UART_HandleTypeDef huart3;

/*******************************************************************************
 * ESTRUTURAS DE DADOS LOCAIS
 ******************************************************************************/

static struct
{
    u8 rxState; ///< Estado atual da deteccao da palavra
} serialData;

/*******************************************************************************
 * FUNCOES PUBLICAS
 ******************************************************************************/

/******************************************************************************/
/** @brief  Inicializa a maquina de estados da serial.
 ******************************************************************************/
void SerialCmd_Init(void)
{
    serialData.rxState = 0;
}

/******************************************************************************/
/** @brief  Tarefa continua que le a USART de forma nao bloqueante.
 * @details Busca pelas strings "LED1", "LED2" e "LED3".
 ******************************************************************************/
void SerialCmd_Task(void)
{
    u8 rxByte = 0;

    // Le 1 byte com Timeout 0 (Nao Bloqueante)
    if (HAL_UART_Receive(&huart3, &rxByte, 1, 0) == HAL_OK)
    {
        switch (serialData.rxState)
        {
            case 0:
                if (rxByte == 'L' || rxByte == 'l') serialData.rxState = 1;
                else serialData.rxState = 0;
                break;
            case 1:
                if (rxByte == 'E' || rxByte == 'e') serialData.rxState = 2;
                else serialData.rxState = 0;
                break;
            case 2:
                if (rxByte == 'D' || rxByte == 'd') serialData.rxState = 3;
                else serialData.rxState = 0;
                break;
            case 3:
                if (rxByte == '1') LedPwm_SelectLed(eLED_1);
                else if (rxByte == '2') LedPwm_SelectLed(eLED_2);
                else if (rxByte == '3') LedPwm_SelectLed(eLED_3);

                // Retorna ao estado inicial independentemente do caractere
                serialData.rxState = 0;
                break;
            default:
                serialData.rxState = 0;
                break;
        }
    }
}

/** @} DOXYGEN GROUP TAG END OF FILE */
