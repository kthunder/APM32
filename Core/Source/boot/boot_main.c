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
    pFunction JumpToApplication;
    uint32_t JumpAddress;
    
    /* Check vector table */
    if (((*(__IO uint32_t*)app_addr) & 0x20000000 ) == 0x20000000)
    {
        /* Jump to user application */
        JumpAddress = *(__IO uint32_t*) (app_addr + 4);
        JumpToApplication = (pFunction) JumpAddress;

        /* Initialize user application's Stack Pointer */
        __set_MSP(*(__IO uint32_t*) app_addr);
        SCB->VTOR = app_addr;
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
    
    if (RTC->BAKP19 != 0x5AFE)
    {
        boot_jump_to_app(0x08010000);
    }
    RTC->BAKP19 = 0x0;

    __enable_irq();  /* SystemInit() disables all interrupts, re-enable before USB */
void dfu_flash_init(uint8_t busid, uintptr_t reg_base);
    dfu_flash_init(1, USB_OTG_HS_PERIPH_BASE);
    while (1) {
        
    }
    // GPIO_InitTypeDef  GPIO_InitStruct = {0U};
    // /* Configure the user KEY pin */
    // GPIO_InitStruct.Pin     = GPIO_PIN_3;
    // GPIO_InitStruct.Mode    = GPIO_MODE_INPUT;
    // GPIO_InitStruct.Pull    = GPIO_PULLUP;
    // GPIO_InitStruct.Speed   = GPIO_SPEED_FAST;

    // DAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    // DAL_Delay(5);

    // if(DAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == 1)
    // {
    //     boot_jump_to_app(0x08010000);
    // }

    // /* Infinite loop */
    // extern void msc_ram_init(uint8_t busid, uintptr_t reg_base);
    // msc_ram_init(1, USB_OTG_HS_PERIPH_BASE);
    // while (1) {
    //     extern bool flash_start;
    //     extern uint32_t flash_timer;
    //     if (flash_start) {
    //         DAL_Delay(1000);
    //         if (DAL_GetTick() - flash_timer > 2000)
    //             boot_jump_to_app(0x08010000);
    //     }
    // }
}

uint8_t *dfu_read_flash(uint8_t *src, uint8_t *dest, uint32_t len)
{
  uint32_t i = 0;
  uint8_t *psrc = src;

  for (i = 0; i < len; i++)
  {
    dest[i] = *psrc++;
  }
  /* Return a valid address to avoid HardFault */
  return (uint8_t *)(dest);
}

uint16_t dfu_write_flash(uint8_t *src, uint8_t *dest, uint32_t len)
{
  DAL_FLASH_Unlock();
  uint32_t i = 0;

  for (i = 0; i < len; i += 4)
  {
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
     * be done by byte */
    if (DAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)(dest + i),
                          *(uint32_t *)(src + i)) == DAL_OK)
    {
      /* Check the written value */
      if (*(uint32_t *)(src + i) != *(uint32_t *)(dest + i))
      {
        /* Flash content doesn't match SRAM content */
        return (1);
      }
    }
    else
    {
      /* Error occurred while writing data in Flash memory */
      return (2);
    }
  }
  return 0;
}

uint16_t dfu_erase_flash(uint32_t add)
{
  flash_erase(add);
  return 0;
}

void dfu_leave(void)
{
    NVIC_SystemReset();
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
