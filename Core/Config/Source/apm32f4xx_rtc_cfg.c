/**
 * @file        apm32f4xx_rtc_cfg.c
 *
 * @brief       This file provides configuration support for RTC
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
#include "apm32f4xx_rtc_cfg.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
RTC_HandleTypeDef hrtc;

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/**
 * @brief  Initializes the RTC MSP
 *
 * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
 *                the configuration information for RTC
 *
 * @retval None
 */
void DAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
    RCM_OscInitTypeDef        RCM_OscInitStruct;
    RCM_PeriphCLKInitTypeDef  PeriphClk_InitStruct;

    /* Configure the LSI */
    RCM_OscInitStruct.OscillatorType    = RCM_OSCILLATORTYPE_LSI;
    RCM_OscInitStruct.PLL.PLLState      = RCM_PLL_NONE;
    RCM_OscInitStruct.LSIState          = RCM_LSI_ON;
    RCM_OscInitStruct.LSEState          = RCM_LSE_OFF;
    if(DAL_RCM_OscConfig(&RCM_OscInitStruct) != DAL_OK)
    {
        Error_Handler();
    }

    /* Configure the RTC clock source */
    PeriphClk_InitStruct.PeriphClockSelection   = RCM_PERIPHCLK_RTC;
    PeriphClk_InitStruct.RTCClockSelection      = RCM_RTCCLKSOURCE_LSI;
    if(DAL_RCMEx_PeriphCLKConfig(&PeriphClk_InitStruct) != DAL_OK)
    { 
        Error_Handler();
    }

    /* Enable RTC clock */
    __DAL_RCM_RTC_ENABLE();
}

/**
 * @brief  DeInitializes the RTC MSP
 *
 * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
 *                the configuration information for RTC
 *
 * @retval None
 */
void DAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
    /* Disable RTC clock */
    __DAL_RCM_RTC_ENABLE();
}

/**
 * @brief   RTC configuration
 *
 * @param   None
 *
 * @retval  None
 */
void DAL_RTC_Config(void)
{
    hrtc.Instance               = RTC;
    hrtc.Init.HourFormat        = RTC_HOURFORMAT_24;
    hrtc.Init.AsynchPrediv      = 0x7FU;
    hrtc.Init.SynchPrediv       = 0x130U;
    hrtc.Init.OutPut            = RTC_OUTPUT_DISABLE;
    hrtc.Init.OutPutPolarity    = RTC_OUTPUT_POLARITY_HIGH;
    hrtc.Init.OutPutType        = RTC_OUTPUT_TYPE_OPENDRAIN;
    if (DAL_RTC_Init(&hrtc) != DAL_OK)
    {
        Error_Handler();
    }
}
