#include "apm32f4xx.h"
#include <stdio.h>
#define SECTION_DATA(sec) __attribute__((section(sec)))

__WEAK void Default_Handler(void)
{
    __ASM("b .");
}

#define FUNC_ALIAS(func_from, func_to) void func_to(void) __attribute__((alias(#func_from), weak))

FUNC_ALIAS(Default_Handler, NMI_Handler);
FUNC_ALIAS(Default_Handler, HardFault_Handler);
FUNC_ALIAS(Default_Handler, MemManage_Handler);
FUNC_ALIAS(Default_Handler, BusFault_Handler);
FUNC_ALIAS(Default_Handler, UsageFault_Handler);
FUNC_ALIAS(Default_Handler, SVC_Handler);
FUNC_ALIAS(Default_Handler, DebugMon_Handler);
FUNC_ALIAS(Default_Handler, PendSV_Handler);
FUNC_ALIAS(Default_Handler, SysTick_Handler);
FUNC_ALIAS(Default_Handler, WWDT_IRQHandler);
FUNC_ALIAS(Default_Handler, PVD_IRQHandler);
FUNC_ALIAS(Default_Handler, TAMP_STAMP_IRQHandler);
FUNC_ALIAS(Default_Handler, RTC_WKUP_IRQHandler);
FUNC_ALIAS(Default_Handler, FLASH_IRQHandler);
FUNC_ALIAS(Default_Handler, RCM_IRQHandler);
FUNC_ALIAS(Default_Handler, EINT0_IRQHandler);
FUNC_ALIAS(Default_Handler, EINT1_IRQHandler);
FUNC_ALIAS(Default_Handler, EINT2_IRQHandler);
FUNC_ALIAS(Default_Handler, EINT3_IRQHandler);
FUNC_ALIAS(Default_Handler, EINT4_IRQHandler);
FUNC_ALIAS(Default_Handler, DMA1_STR0_IRQHandler);
FUNC_ALIAS(Default_Handler, DMA1_STR1_IRQHandler);
FUNC_ALIAS(Default_Handler, DMA1_STR2_IRQHandler);
FUNC_ALIAS(Default_Handler, DMA1_STR3_IRQHandler);
FUNC_ALIAS(Default_Handler, DMA1_STR4_IRQHandler);
FUNC_ALIAS(Default_Handler, DMA1_STR5_IRQHandler);
FUNC_ALIAS(Default_Handler, DMA1_STR6_IRQHandler);
FUNC_ALIAS(Default_Handler, ADC_IRQHandler);
FUNC_ALIAS(Default_Handler, CAN1_TX_IRQHandler);
FUNC_ALIAS(Default_Handler, CAN1_RX0_IRQHandler);
FUNC_ALIAS(Default_Handler, CAN1_RX1_IRQHandler);
FUNC_ALIAS(Default_Handler, CAN1_SCE_IRQHandler);
FUNC_ALIAS(Default_Handler, EINT9_5_IRQHandler);
FUNC_ALIAS(Default_Handler, TMR1_BRK_TMR9_IRQHandler);
FUNC_ALIAS(Default_Handler, TMR1_UP_TMR10_IRQHandler);
FUNC_ALIAS(Default_Handler, TMR1_TRG_COM_TMR11_IRQHandler);
FUNC_ALIAS(Default_Handler, TMR1_CC_IRQHandler);
FUNC_ALIAS(Default_Handler, TMR2_IRQHandler);
FUNC_ALIAS(Default_Handler, TMR3_IRQHandler);
FUNC_ALIAS(Default_Handler, TMR4_IRQHandler);
FUNC_ALIAS(Default_Handler, I2C1_EV_IRQHandler);
FUNC_ALIAS(Default_Handler, I2C1_ER_IRQHandler);
FUNC_ALIAS(Default_Handler, I2C2_EV_IRQHandler);
FUNC_ALIAS(Default_Handler, I2C2_ER_IRQHandler);
FUNC_ALIAS(Default_Handler, SPI1_IRQHandler);
FUNC_ALIAS(Default_Handler, SPI2_IRQHandler);
FUNC_ALIAS(Default_Handler, USART1_IRQHandler);
FUNC_ALIAS(Default_Handler, USART2_IRQHandler);
FUNC_ALIAS(Default_Handler, USART3_IRQHandler);
FUNC_ALIAS(Default_Handler, EINT15_10_IRQHandler);
FUNC_ALIAS(Default_Handler, RTC_Alarm_IRQHandler);
FUNC_ALIAS(Default_Handler, OTG_FS_WKUP_IRQHandler);
FUNC_ALIAS(Default_Handler, TMR8_BRK_TMR12_IRQHandler);
FUNC_ALIAS(Default_Handler, TMR8_UP_TMR13_IRQHandler);
FUNC_ALIAS(Default_Handler, TMR8_TRG_COM_TMR14_IRQHandler);
FUNC_ALIAS(Default_Handler, TMR8_CC_IRQHandler);
FUNC_ALIAS(Default_Handler, DMA1_STR7_IRQHandler);
FUNC_ALIAS(Default_Handler, EMMC_IRQHandler);
FUNC_ALIAS(Default_Handler, SDIO_IRQHandler);
FUNC_ALIAS(Default_Handler, TMR5_IRQHandler);
FUNC_ALIAS(Default_Handler, SPI3_IRQHandler);
FUNC_ALIAS(Default_Handler, UART4_IRQHandler);
FUNC_ALIAS(Default_Handler, UART5_IRQHandler);
FUNC_ALIAS(Default_Handler, TMR6_DAC_IRQHandler);
FUNC_ALIAS(Default_Handler, TMR7_IRQHandler);
FUNC_ALIAS(Default_Handler, DMA2_STR0_IRQHandler);
FUNC_ALIAS(Default_Handler, DMA2_STR1_IRQHandler);
FUNC_ALIAS(Default_Handler, DMA2_STR2_IRQHandler);
FUNC_ALIAS(Default_Handler, DMA2_STR3_IRQHandler);
FUNC_ALIAS(Default_Handler, DMA2_STR4_IRQHandler);
FUNC_ALIAS(Default_Handler, ETH_IRQHandler);
FUNC_ALIAS(Default_Handler, ETH_WKUP_IRQHandler);
FUNC_ALIAS(Default_Handler, CAN2_TX_IRQHandler);
FUNC_ALIAS(Default_Handler, CAN2_RX0_IRQHandler);
FUNC_ALIAS(Default_Handler, CAN2_RX1_IRQHandler);
FUNC_ALIAS(Default_Handler, CAN2_SCE_IRQHandler);
FUNC_ALIAS(Default_Handler, OTG_FS_IRQHandler);
FUNC_ALIAS(Default_Handler, DMA2_STR5_IRQHandler);
FUNC_ALIAS(Default_Handler, DMA2_STR6_IRQHandler);
FUNC_ALIAS(Default_Handler, DMA2_STR7_IRQHandler);
FUNC_ALIAS(Default_Handler, USART6_IRQHandler);
FUNC_ALIAS(Default_Handler, I2C3_EV_IRQHandler);
FUNC_ALIAS(Default_Handler, I2C3_ER_IRQHandler);
FUNC_ALIAS(Default_Handler, OTG_HS1_EP1_OUT_IRQHandler);
FUNC_ALIAS(Default_Handler, OTG_HS1_EP1_IN_IRQHandler);
FUNC_ALIAS(Default_Handler, OTG_HS1_WKUP_IRQHandler);
FUNC_ALIAS(Default_Handler, OTG_HS1_IRQHandler);
FUNC_ALIAS(Default_Handler, DCI_IRQHandler);
FUNC_ALIAS(Default_Handler, HASH_RNG_IRQHandler);
FUNC_ALIAS(Default_Handler, FPU_IRQHandler);
FUNC_ALIAS(Default_Handler, SM3_IRQHandler);
FUNC_ALIAS(Default_Handler, SM4_IRQHandler);
FUNC_ALIAS(Default_Handler, BN_IRQHandler);

extern uint8_t _estack[];
extern uint8_t _sidata[];
extern uint8_t _sdata[];
extern uint8_t _edata[];
extern uint8_t _sbss[];
extern uint8_t _ebss[];

static void __memset(uint8_t *dist, uint8_t val, uint32_t len)
{
    while (len--)
        *dist++ = val;
}
static void __memcpy(uint8_t *dist, uint8_t *src, uint32_t len)
{
    while (len--)
        *dist++ = *src++;
}

extern int main();

void Reset_Handler(void)
{
    __set_MSP((uint32_t)_estack);
    __memset(_sbss, 0, _ebss - _sbss);
    __memcpy(_sdata, _sidata, _edata - _sdata);
    SystemInit();
    main();
}

SECTION_DATA(".isr_vector")
// typedef void (*IRQ_HandlerType)(void);
__USED static void (*vector[256])(void) = {
    (void (*)(void))_estack,
    Reset_Handler                      ,// Reset Handler
    NMI_Handler                        ,// NMI Handler
    HardFault_Handler                  ,// Hard Fault Handler
    MemManage_Handler                  ,// MPU Fault Handler
    BusFault_Handler                   ,// Bus Fault Handler
    UsageFault_Handler                 ,// Usage Fault Handler
    0                                  ,// Reserved
    0                                  ,// Reserved
    0                                  ,// Reserved
    0                                  ,// Reserved
    SVC_Handler                        ,// SVCall Handler
    DebugMon_Handler                   ,// Debug Monitor Handler
    0                                  ,// Reserved
    PendSV_Handler                     ,// PendSV Handler
    SysTick_Handler                    ,// SysTick Handler

    /* External Interrupts */
    WWDT_IRQHandler                    ,// Window WatchDog
    PVD_IRQHandler                     ,// PVD through EINT Line detection
    TAMP_STAMP_IRQHandler              ,// Tamper and TimeStamps through the EINT line
    RTC_WKUP_IRQHandler                ,// RTC Wakeup through the EINT line
    FLASH_IRQHandler                   ,// FLASH
    RCM_IRQHandler                     ,// RCM
    EINT0_IRQHandler                   ,// EINT Line0
    EINT1_IRQHandler                   ,// EINT Line1
    EINT2_IRQHandler                   ,// EINT Line2
    EINT3_IRQHandler                   ,// EINT Line3
    EINT4_IRQHandler                   ,// EINT Line4
    DMA1_STR0_IRQHandler               ,// DMA1 Stream 0
    DMA1_STR1_IRQHandler               ,// DMA1 Stream 1
    DMA1_STR2_IRQHandler               ,// DMA1 Stream 2
    DMA1_STR3_IRQHandler               ,// DMA1 Stream 3
    DMA1_STR4_IRQHandler               ,// DMA1 Stream 4
    DMA1_STR5_IRQHandler               ,// DMA1 Stream 5
    DMA1_STR6_IRQHandler               ,// DMA1 Stream 6
    ADC_IRQHandler                     ,// ADC1, ADC2 and ADC3s
    CAN1_TX_IRQHandler                 ,// CAN1 TX
    CAN1_RX0_IRQHandler                ,// CAN1 RX0
    CAN1_RX1_IRQHandler                ,// CAN1 RX1
    CAN1_SCE_IRQHandler                ,// CAN1 SCE
    EINT9_5_IRQHandler                 ,// External Line[9:5]s
    TMR1_BRK_TMR9_IRQHandler           ,// TMR1 Break and TMR9
    TMR1_UP_TMR10_IRQHandler           ,// TMR1 Update and TMR10
    TMR1_TRG_COM_TMR11_IRQHandler      ,// TMR1 Trigger and Commutation and TMR11
    TMR1_CC_IRQHandler                 ,// TMR1 Capture Compare
    TMR2_IRQHandler                    ,// TMR2
    TMR3_IRQHandler                    ,// TMR3
    TMR4_IRQHandler                    ,// TMR4
    I2C1_EV_IRQHandler                 ,// I2C1 Event
    I2C1_ER_IRQHandler                 ,// I2C1 Error
    I2C2_EV_IRQHandler                 ,// I2C2 Event
    I2C2_ER_IRQHandler                 ,// I2C2 Error
    SPI1_IRQHandler                    ,// SPI1
    SPI2_IRQHandler                    ,// SPI2
    USART1_IRQHandler                  ,// USART1
    USART2_IRQHandler                  ,// USART2
    USART3_IRQHandler                  ,// USART3
    EINT15_10_IRQHandler               ,// External Line[15:10]s
    RTC_Alarm_IRQHandler               ,// RTC Alarm (A and B) through EINT Line
    OTG_FS_WKUP_IRQHandler             ,// USB OTG FS Wakeup through EINT line
    TMR8_BRK_TMR12_IRQHandler          ,// TMR8 Break and TMR12
    TMR8_UP_TMR13_IRQHandler           ,// TMR8 Update and TMR13
    TMR8_TRG_COM_TMR14_IRQHandler      ,// TMR8 Trigger and Commutation and TMR14
    TMR8_CC_IRQHandler                 ,// TMR8 Capture Compare
    DMA1_STR7_IRQHandler               ,// DMA1 Stream7
    EMMC_IRQHandler                    ,// EMMC
    SDIO_IRQHandler                    ,// SDIO
    TMR5_IRQHandler                    ,// TMR5
    SPI3_IRQHandler                    ,// SPI3
    UART4_IRQHandler                   ,// UART4
    UART5_IRQHandler                   ,// UART5
    TMR6_DAC_IRQHandler                ,// TMR6 and DAC1&2 underrun errors
    TMR7_IRQHandler                    ,// TMR7
    DMA2_STR0_IRQHandler               ,// DMA2 Stream 0
    DMA2_STR1_IRQHandler               ,// DMA2 Stream 1
    DMA2_STR2_IRQHandler               ,// DMA2 Stream 2
    DMA2_STR3_IRQHandler               ,// DMA2 Stream 3
    DMA2_STR4_IRQHandler               ,// DMA2 Stream 4
    ETH_IRQHandler                     ,// Ethernet
    ETH_WKUP_IRQHandler                ,// Ethernet Wakeup through EINT line
    CAN2_TX_IRQHandler                 ,// CAN2 TX
    CAN2_RX0_IRQHandler                ,// CAN2 RX0
    CAN2_RX1_IRQHandler                ,// CAN2 RX1
    CAN2_SCE_IRQHandler                ,// CAN2 SCE
    OTG_FS_IRQHandler                  ,// USB OTG FS
    DMA2_STR5_IRQHandler               ,// DMA2 Stream 5
    DMA2_STR6_IRQHandler               ,// DMA2 Stream 6
    DMA2_STR7_IRQHandler               ,// DMA2 Stream 7
    USART6_IRQHandler                  ,// USART6
    I2C3_EV_IRQHandler                 ,// I2C3 event
    I2C3_ER_IRQHandler                 ,// I2C3 error
    OTG_HS1_EP1_OUT_IRQHandler         ,// USB OTG HS End Point 1 Out
    OTG_HS1_EP1_IN_IRQHandler          ,// USB OTG HS End Point 1 In
    OTG_HS1_WKUP_IRQHandler            ,// USB OTG HS Wakeup through EINT
    OTG_HS1_IRQHandler                 ,// USB OTG HS
    DCI_IRQHandler                     ,// DCI
    0                                  ,// Reserved
    HASH_RNG_IRQHandler                ,// Hash and Rng
    FPU_IRQHandler                     ,// FPU
    SM3_IRQHandler                     ,// SM3
    SM4_IRQHandler                     ,// SM4
    BN_IRQHandler                      ,// BN
};
