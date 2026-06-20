/**
 * @file        main.c
 *
 * @brief       Main program body
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
#include "main.h"

/* Private includes *******************************************************/
#include "apm32f4xx_device_cfg.h"
#include <string.h>
#include "dap_main.h"
/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
#define mv_to_value(v) (4095*(v)/3300)
__IO uint16_t dacValue = mv_to_value(3300);
/* Private function prototypes ********************************************/

/* External variables *****************************************************/
extern DAC_HandleTypeDef hdac;
/* External functions *****************************************************/

extern UART_HandleTypeDef huart1;
/**
 * @brief   Main program
 *
 * @param   None
 *
 * @retval  None
 */
int main(void)
{
    /* Device configuration */
    DAL_DeviceConfig();
    DAL_EnableCompensationCell();

    // DAL_Delay(3000);
    // RTC->BAKP19 = 0x5afe;
    // NVIC_SystemReset();

    /* Start DAC channel 1 output */
    DAL_DAC_Start(&hdac, DAC_CHANNEL_1);

    /* Generate analog signal */
    DAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dacValue);

    /* Infinite loop */
    chry_dap_init(1,USB_OTG_HS_PERIPH_BASE);

    while (!usb_device_is_configured(1)) {
    }

    while (1) {
        chry_dap_handle();
        chry_dap_usb2uart_handle();
    }
}

/*
    PA8 : LED
    PA9 : TX
    PA10: RX
    PA13: MCU_SWDIO
    PA14: MCU_SWCLK
    PC0 : DAP_SWCLK
    PC1 : DAP_SWDIO
*/
