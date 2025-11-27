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
DMA_HandleTypeDef hdma_usart1Tx;
DMA_HandleTypeDef hdma_usart1Rx;

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
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (huart->Instance == USART1)
    {
        /* Enable USART1 GPIO clock */
        __DAL_RCM_GPIOA_CLK_ENABLE();
        
        /* Enable USART1 clock */
        __DAL_RCM_USART1_CLK_ENABLE();
        
        /* Enable DMA2 clock */
        __DAL_RCM_DMA2_CLK_ENABLE();

        /* Configure the UART TX and RX pin */
        GPIO_InitStruct.Pin         = GPIO_PIN_9 | GPIO_PIN_10;
        GPIO_InitStruct.Mode        = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull        = GPIO_NOPULL;
        GPIO_InitStruct.Speed       = GPIO_SPEED_FAST;
        GPIO_InitStruct.Alternate   = GPIO_AF7_USART1;

        DAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* Configure USART1 TX DMA */
        hdma_usart1Tx.Instance                  = DMA2_Stream7;
        hdma_usart1Tx.Init.Channel              = DMA_CHANNEL_4;
        hdma_usart1Tx.Init.Direction            = DMA_MEMORY_TO_PERIPH;
        hdma_usart1Tx.Init.PeriphInc            = DMA_PINC_DISABLE;
        hdma_usart1Tx.Init.MemInc               = DMA_MINC_ENABLE;
        hdma_usart1Tx.Init.PeriphDataAlignment  = DMA_PDATAALIGN_BYTE;
        hdma_usart1Tx.Init.MemDataAlignment     = DMA_MDATAALIGN_BYTE;
        hdma_usart1Tx.Init.Mode                 = DMA_NORMAL;
        hdma_usart1Tx.Init.Priority             = DMA_PRIORITY_LOW;
        hdma_usart1Tx.Init.FIFOMode             = DMA_FIFOMODE_DISABLE;
        hdma_usart1Tx.Init.FIFOThreshold        = DMA_FIFO_THRESHOLD_FULL;
        hdma_usart1Tx.Init.PeriphBurst          = DMA_PBURST_INC4;
        hdma_usart1Tx.Init.MemBurst             = DMA_MBURST_INC4;
        if (DAL_DMA_Init(&hdma_usart1Tx) != DAL_OK)
        {
            Error_Handler();
        }

        /* Link DMA handle */
        __DAL_LINKDMA(huart, hdmatx, hdma_usart1Tx);

        /* Configure USART1 RX DMA */
        hdma_usart1Rx.Instance                  = DMA2_Stream5;
        hdma_usart1Rx.Init.Channel              = DMA_CHANNEL_4;
        hdma_usart1Rx.Init.Direction            = DMA_PERIPH_TO_MEMORY;
        hdma_usart1Rx.Init.PeriphInc            = DMA_PINC_DISABLE;
        hdma_usart1Rx.Init.MemInc               = DMA_MINC_ENABLE;
        hdma_usart1Rx.Init.PeriphDataAlignment  = DMA_PDATAALIGN_BYTE;
        hdma_usart1Rx.Init.MemDataAlignment     = DMA_MDATAALIGN_BYTE;
        hdma_usart1Rx.Init.Mode                 = DMA_CIRCULAR;
        hdma_usart1Rx.Init.Priority             = DMA_PRIORITY_HIGH;
        hdma_usart1Rx.Init.FIFOMode             = DMA_FIFOMODE_DISABLE;
        hdma_usart1Rx.Init.FIFOThreshold        = DMA_FIFO_THRESHOLD_FULL;
        hdma_usart1Rx.Init.PeriphBurst          = DMA_PBURST_INC4;
        hdma_usart1Rx.Init.MemBurst             = DMA_MBURST_INC4;
        if (DAL_DMA_Init(&hdma_usart1Rx) != DAL_OK)
        {
            Error_Handler();
        }

        /* Link DMA handle */
        __DAL_LINKDMA(huart, hdmarx, hdma_usart1Rx);
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

        /* USART1 DMA de-init */
        DAL_DMA_DeInit(huart->hdmatx);
        DAL_DMA_DeInit(huart->hdmarx);
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
