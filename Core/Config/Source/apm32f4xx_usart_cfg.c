/**
 * @file        apm32f4xx_usart_cfg.c
 *
 * @brief       This file provides configuration support for USART
 *
 * @version     V1.0.0
 *
 * @date        2023-12-01
 *
 * @attention
 *
 *  Copyright (C) 2023 Geehy Semiconductor
 *
 *  You may not use this file except in compliance with the
 *  GEEHY COPYRIGHT NOTICE (GEEHY SOFTWARE PACKAGE LICENSE).
 *
 *  The program is only for reference, which is distributed in the hope
 *  that it will be useful and instructional for customers to develop
 *  their software. Unless required by applicable law or agreed to in
 *  writing, the program is distributed on an "AS IS" BASIS, WITHOUT
 *  ANY WARRANTY OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the GEEHY SOFTWARE PACKAGE LICENSE for the governing permissions
 *  and limitations under the License.
 */

/* Includes ***************************************************************/
#include "apm32f4xx_usart_cfg.h"

/* Private includes *******************************************************/
#include <stdio.h>

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
UART_HandleTypeDef huart1;

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/**
 * @brief   USART1 configuration
 *
 * @param   None
 *
 * @retval  None
 */
void DAL_USART1_Config(void)
{
    huart1.Instance             = USART1;
    huart1.Init.BaudRate        = 115200U;
    huart1.Init.WordLength      = UART_WORDLENGTH_8B;
    huart1.Init.StopBits        = UART_STOPBITS_1;
    huart1.Init.Parity          = UART_PARITY_NONE;
    huart1.Init.Mode            = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl       = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling    = UART_OVERSAMPLING_16;
    
    if (DAL_UART_Init(&huart1) != DAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief   UART MSP Init
 *
 * @param   huart  Pointer to a UART_HandleTypeDef structure that contains
 *                the configuration information for the specified UART module
 *
 * @retval  None
 */
void DAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0U};

    if (huart->Instance == USART1)
    {
        /* Enable USART1 GPIO clock */
        __DAL_RCM_GPIOA_CLK_ENABLE();
        
        /* Enable USART1 clock */
        __DAL_RCM_USART1_CLK_ENABLE();

        /* Configure the UART TX and RX pin */
        GPIO_InitStruct.Pin         = GPIO_PIN_9 | GPIO_PIN_10;
        GPIO_InitStruct.Mode        = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull        = GPIO_NOPULL;
        GPIO_InitStruct.Speed       = GPIO_SPEED_FAST;
        GPIO_InitStruct.Alternate   = GPIO_AF7_USART1;

        DAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
}

/**
  * @brief  UART MSP DeInit
  *
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module
  *
  * @retval None
  */
void DAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        /* Reset USART */
        __DAL_RCM_USART1_FORCE_RESET();
        __DAL_RCM_USART1_RELEASE_RESET();
        
        /* Disable USART and GPIO clocks */
        DAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);
    }
}

#if defined (__CC_ARM) || defined (__ICCARM__) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))

/*!
* @brief       Redirect C Library function printf to serial port.
*              After Redirection, you can use printf function.
*
* @param       ch:  The characters that need to be send.
*
* @param       *f:  pointer to a FILE that can recording all information
*              needed to control a stream
*
* @retval      The characters that need to be send.
*
* @note
*/
int fputc(int ch, FILE* f)
{
    /* send a byte of data to the serial port */
    DAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1U, 1000U);

    return (ch);
}

#elif defined (__GNUC__)

/*!
* @brief       Redirect C Library function printf to serial port.
*              After Redirection, you can use printf function.
*
* @param       ch:  The characters that need to be send.
*
* @retval      The characters that need to be send.
*
* @note
*/
int __io_putchar(int ch)
{
    /* send a byte of data to the serial port */
    DAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1U, 1000U);

    return ch;
}

/*!
* @brief       Redirect C Library function printf to serial port.
*              After Redirection, you can use printf function.
*
* @param       file:  Meaningless in this function.
*
* @param       *ptr:  Buffer pointer for data to be sent.
*
* @param       len:  Length of data to be sent.
*
* @retval      The characters that need to be send.
*
* @note
*/
int _write(int file, char* ptr, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        __io_putchar(*ptr++);
    }

    return len;
}

#else
#warning Not supported compiler type
#endif
