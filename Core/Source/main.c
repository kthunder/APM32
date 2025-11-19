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
        printf("1 %08X %d\r\n", sector_addr[i], addr>=sector_addr[i]);
        printf("2 %08X %d\r\n", sector_addr[i+1], addr<sector_addr[i+1]);
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
    printf("Erase %08X\r\n", addr);
    printf("sectorNum %d\r\n", sectorNum);
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
        printf("data addr %08X\r\n", &page[i]);
        printf("Program %08X\r\n", addr+i*4);
        if (DAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr+i*4, page[i]) != DAL_OK)
        {
            Error_Handler();
        }
        printf("Program OK\r\n");
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
    uint32_t jump_addr;
    pFunction jump_to_app;
    
    /* Check if valid application exists */
    if (((*(__IO uint32_t*)app_addr) & 0x20000000) == 0x20000000) {
        /* Get jump address from vector table */
        jump_addr = *(__IO uint32_t*)(app_addr + 4);
        jump_to_app = (pFunction)jump_addr;
        
        /* Disable all interrupts */
        __disable_irq();
        
        /* Reset all peripherals */
        DAL_DeInit();
        
        /* Set main stack pointer */
        __set_MSP(*(__IO uint32_t*)app_addr);
        
        /* Jump to application */
        jump_to_app();
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

    printf("init s\r\n");
    
    GPIO_InitTypeDef  GPIO_InitStruct = {0U};
    /* Configure the LED pin */
    GPIO_InitStruct.Pin     = GPIO_PIN_10;
    GPIO_InitStruct.Mode    = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull    = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FAST;

    DAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    for (int i = 0; i<=10; i++) {
        printf("pin %d\r\n", DAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10));
    }

    if(DAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == 0)
    {
        printf("jump \r\n");
        boot_jump_to_app(0x08010000);
    }

    /* Infinite loop */
    extern void msc_ram_init(uint8_t busid, uintptr_t reg_base);
    msc_ram_init(1, USB_OTG_HS_PERIPH_BASE);
    while (1) {
        extern bool flash_start;
        extern uint32_t flash_timer;
        if (flash_start) {
            printf("TIMER %d ms\r\n", DAL_GetTick());
            printf("flash_timer %d ms\r\n", flash_timer);
            if (DAL_GetTick() - flash_timer > 3000)
                NVIC_SystemReset();
        }
    }
}
