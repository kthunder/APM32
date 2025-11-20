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
#include <stdint.h>
#include <stdbool.h>
/* Private macro **********************************************************/
#define FLASH_USER_ADDR         ((uint32_t)0x08040000U)
/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/
static const uint32_t sector_addr[12] = {
    0x08000000, // 16K
    0x08004000, // 16K
    0x08008000, // 16K
    0x0800C000, // 16K
    0x08010000, // 64K
    0x08020000, // 128K
    0x08040000, // 128K
    0x08060000, // 128K
    0x08080000, // 128K
    0x080A0000, // 128K
    0x080C0000, // 128K
    0x080E0000  // 128K
};

void flash_erase(uint32_t addr)
{
    uint32_t sectorNum = 0xffU;
    uint32_t sectorError = 0xffU;
    FLASH_EraseInitTypeDef Erase_InitStruct = {0};
    static bool erase_flag[12] = {0};
    
    for (int i = 0; i < (11); i++) 
    {
        if ((addr>=sector_addr[i]) && (addr<sector_addr[i+1]) )
        {
            sectorNum = i;
            break;
        }
    }

    if (sectorNum==0xFF) {
        return;
    }

    if (erase_flag[sectorNum] == 1) {
        return;
    }

    /* Unlock the Flash */
    DAL_FLASH_Unlock();

    /* Erase sector */
    Erase_InitStruct.Sector         = sectorNum;
    Erase_InitStruct.NbSectors      = 1U;
    Erase_InitStruct.TypeErase      = FLASH_TYPEERASE_SECTORS;
    Erase_InitStruct.VoltageRange   = FLASH_VOLTAGE_RANGE_3;
    if(DAL_FLASHEx_Erase(&Erase_InitStruct, &sectorError) != DAL_OK)
    {
        Error_Handler();
    }
    erase_flag[sectorNum] = 1;

    /* Lock the Flash to */
    DAL_FLASH_Lock();
}
void flash_program(uint32_t addr, uint8_t *data)
{
    /* Unlock the Flash */
    DAL_FLASH_Unlock();

    uint32_t *page = (uint32_t *)data;

    for (int i = 0; i < (512/4); i++) {
        if (DAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr+i*4, page[i]) != DAL_OK)
        {
            Error_Handler();
        }
    }
    
    /* Lock the Flash to */
    DAL_FLASH_Lock();
}

typedef void (*pFunction)(void);
/**
 * @brief   Jump to application
 * @param   app_addr: Application start address
 * @retval  None
 */
void boot_jump_to_app(uint32_t app_addr)
{
    #define APPLICATION_ADDRESS 0x08010000
    pFunction JumpToApplication;
    uint32_t JumpAddress;

    RCM->AHB1RST = 0xFFFFFFFFU;
    RCM->APB1RST = 0xFFFFFFFFU;
    RCM->APB2RST = 0xFFFFFFFFU;
    RCM->AHB2RST = 0xFFFFFFFFU;
    
    RCM->AHB1RST = 0x00000000U;
    RCM->APB1RST = 0x00000000U;
    RCM->APB2RST = 0x00000000U;
    RCM->AHB2RST = 0x00000000U;
    
    RCM->AHB1CLKEN = 0x00000000U;
    RCM->AHB2CLKEN = 0x00000000U;
    RCM->APB1CLKEN = 0x00000000U;
    RCM->APB2CLKEN = 0x00000000U;
    
    /* Check vector table */
    if (((*(__IO uint32_t*)APPLICATION_ADDRESS) & 0x2FFE0000 ) == 0x20000000)
    {
        /* Jump to user application */
        JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
        JumpToApplication = (pFunction) JumpAddress;

        /* Initialize user application's Stack Pointer */
        __set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
        JumpToApplication();
    }
}

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
    
    GPIO_InitTypeDef  GPIO_InitStruct = {0U};
    /* Configure the LED pin */
    GPIO_InitStruct.Pin     = GPIO_PIN_10;
    GPIO_InitStruct.Mode    = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull    = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FAST;

    DAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    DAL_Delay(10);

    if(DAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == 0)
    {
        boot_jump_to_app(0x08010000);
    }

    /* Infinite loop */
    extern void msc_ram_init(uint8_t busid, uintptr_t reg_base);
    msc_ram_init(1, USB_OTG_HS_PERIPH_BASE);
    while (1) {
        extern bool flash_start;
        extern uint32_t flash_timer;
        if (flash_start) {
            DAL_Delay(1000);
            if (DAL_GetTick() - flash_timer > 2000)
                boot_jump_to_app(0x08010000);
        }
    }
}
