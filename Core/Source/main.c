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
/* Private macro **********************************************************/
#define FLASH_USER_ADDR         ((uint32_t)0x08040000U)
/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/**
 * @brief   Main program
 *
 * @param   None
 *
 * @retval  None
 */
int main(void)
{
    uint32_t sectorError = 0U;
    
    FLASH_EraseInitTypeDef Erase_InitStruct = {0};
    /* Device configuration */
    DAL_DeviceConfig();
    DAL_EnableCompensationCell();
    /* Unlock the Flash */
    DAL_FLASH_Unlock();
    /* Erase sector */
    Erase_InitStruct.Sector         = FLASH_SECTOR_6;
    Erase_InitStruct.NbSectors      = 1U;
    Erase_InitStruct.TypeErase      = FLASH_TYPEERASE_SECTORS;
    Erase_InitStruct.VoltageRange   = FLASH_VOLTAGE_RANGE_3;
    if(DAL_FLASHEx_Erase(&Erase_InitStruct, &sectorError) != DAL_OK)
    {
        Error_Handler();
    }
    
    if (DAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_USER_ADDR, 0x12345678U) != DAL_OK)
    {
        Error_Handler();
    }

    /* Lock the Flash to */
    DAL_FLASH_Lock();

    /* Infinite loop */
    extern void msc_ram_init(uint8_t busid, uintptr_t reg_base);
    msc_ram_init(1, USB_OTG_HS_PERIPH_BASE);
    while (1) {
    
    }
}
