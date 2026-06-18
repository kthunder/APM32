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

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

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
    SCB->VTOR = 0x08010000;
    /* Device configuration */
    DAL_DeviceConfig();
    DAL_EnableCompensationCell();

    /* Infinite loop */
    chry_dap_init(0,USB_OTG_HS_PERIPH_BASE);

    while (!usb_device_is_configured(0)) {
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
