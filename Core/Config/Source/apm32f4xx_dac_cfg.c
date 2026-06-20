/**
 * @file        apm32f4xx_dac_cfg.c
 *
 * @brief       This file provides configuration support for DAC
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
#include "apm32f4xx_dac_cfg.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
DAC_HandleTypeDef hdac;

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/**
 * @brief  Initialize the DAC MSP
 *
 * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
 *         the configuration information for the specified DAC
 *
 * @retval None
 */
void DAL_DAC_MspInit(DAC_HandleTypeDef *hdac)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable peripheral clock */
    __DAL_RCM_DAC_CLK_ENABLE();

    /* Enable peripheral GPIO clock */
    __DAL_RCM_GPIOA_CLK_ENABLE();

    /* DAC pin configuration */
    GPIO_InitStruct.Pin     = GPIO_PIN_4;
    GPIO_InitStruct.Mode    = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull    = GPIO_NOPULL;
    DAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
 * @brief  DeInitialize the DAC MSP
 *
 * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
 *         the configuration information for the specified DAC
 *
 * @retval None
 */
void DAL_DAC_MspDeInit(DAC_HandleTypeDef *hdac)
{
    /* Disable peripheral clock */
    __DAL_RCM_DAC_CLK_DISABLE();

    /* DAC GPIO configuration */
    DAL_GPIO_DeInit(GPIOA, GPIO_PIN_4);
}

/**
 * @brief   DAC configuration
 *
 * @param   None
 *
 * @retval  None
 */
void DAL_DAC_Config(void)
{
    DAC_ChannelConfTypeDef Channel_ConfigStruct = {0};

    hdac.Instance = DAC;
    if (DAL_DAC_Init(&hdac) != DAL_OK)
    {
        Error_Handler();
    }

    Channel_ConfigStruct.DAC_OutputBuffer   = DAC_OUTPUTBUFFER_ENABLE;
    Channel_ConfigStruct.DAC_Trigger        = DAC_TRIGGER_NONE;
    if (DAL_DAC_ConfigChannel(&hdac, &Channel_ConfigStruct, DAC_CHANNEL_1) != DAL_OK)
    {
        Error_Handler();
    }
}
