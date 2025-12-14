/*
 * Startup code for STM32WB55xx
 * Cortex-M4 core initialization
 */

#include <stdint.h>

/* Linker script symbols */
extern uint32_t _estack;
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

/* Main function prototype */
extern int main(void);

/* System initialization */
extern void SystemInit(void);

/* Default handler for unused interrupts */
void Default_Handler(void)
{
    while (1) {
        /* Infinite loop for unhandled interrupts */
    }
}

/* Reset handler - entry point */
void Reset_Handler(void)
{
    uint32_t *src, *dst;

    /* Copy .data section from Flash to RAM */
    src = &_sidata;
    dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    /* Zero fill .bss section */
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }

    /* Call system initialization */
    SystemInit();

    /* Call main */
    main();

    /* Should never reach here */
    while (1);
}

/* Weak aliases for interrupt handlers */
void NMI_Handler(void)                    __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void)              __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void)              __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void)               __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void)             __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void)                    __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void)               __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)                 __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)                __attribute__((weak, alias("Default_Handler")));

/* STM32WB5x specific interrupt handlers */
void WWDG_IRQHandler(void)                __attribute__((weak, alias("Default_Handler")));
void PVD_PVM_IRQHandler(void)             __attribute__((weak, alias("Default_Handler")));
void TAMP_STAMP_LSECSS_IRQHandler(void)   __attribute__((weak, alias("Default_Handler")));
void RTC_WKUP_IRQHandler(void)            __attribute__((weak, alias("Default_Handler")));
void FLASH_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void RCC_IRQHandler(void)                 __attribute__((weak, alias("Default_Handler")));
void EXTI0_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void EXTI1_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void EXTI2_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void EXTI3_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void EXTI4_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel1_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel2_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel3_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel4_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel5_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel6_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel7_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void ADC1_IRQHandler(void)                __attribute__((weak, alias("Default_Handler")));
void USB_HP_IRQHandler(void)              __attribute__((weak, alias("Default_Handler")));
void USB_LP_IRQHandler(void)              __attribute__((weak, alias("Default_Handler")));
void C2SEV_PWR_C2H_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void COMP_IRQHandler(void)                __attribute__((weak, alias("Default_Handler")));
void EXTI9_5_IRQHandler(void)             __attribute__((weak, alias("Default_Handler")));
void TIM1_BRK_IRQHandler(void)            __attribute__((weak, alias("Default_Handler")));
void TIM1_UP_TIM16_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void TIM1_TRG_COM_TIM17_IRQHandler(void)  __attribute__((weak, alias("Default_Handler")));
void TIM1_CC_IRQHandler(void)             __attribute__((weak, alias("Default_Handler")));
void TIM2_IRQHandler(void)                __attribute__((weak, alias("Default_Handler")));
void PKA_IRQHandler(void)                 __attribute__((weak, alias("Default_Handler")));
void I2C1_EV_IRQHandler(void)             __attribute__((weak, alias("Default_Handler")));
void I2C1_ER_IRQHandler(void)             __attribute__((weak, alias("Default_Handler")));
void I2C3_EV_IRQHandler(void)             __attribute__((weak, alias("Default_Handler")));
void I2C3_ER_IRQHandler(void)             __attribute__((weak, alias("Default_Handler")));
void SPI1_IRQHandler(void)                __attribute__((weak, alias("Default_Handler")));
void SPI2_IRQHandler(void)                __attribute__((weak, alias("Default_Handler")));
void USART1_IRQHandler(void)              __attribute__((weak, alias("Default_Handler")));
void LPUART1_IRQHandler(void)             __attribute__((weak, alias("Default_Handler")));
void SAI1_IRQHandler(void)                __attribute__((weak, alias("Default_Handler")));
void TSC_IRQHandler(void)                 __attribute__((weak, alias("Default_Handler")));
void EXTI15_10_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void RTC_Alarm_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void CRS_IRQHandler(void)                 __attribute__((weak, alias("Default_Handler")));
void PWR_SOTF_BLEACT_802ACT_RFPHASE_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void IPCC_C1_RX_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void IPCC_C1_TX_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void HSEM_IRQHandler(void)                __attribute__((weak, alias("Default_Handler")));
void LPTIM1_IRQHandler(void)              __attribute__((weak, alias("Default_Handler")));
void LPTIM2_IRQHandler(void)              __attribute__((weak, alias("Default_Handler")));
void LCD_IRQHandler(void)                 __attribute__((weak, alias("Default_Handler")));
void QUADSPI_IRQHandler(void)             __attribute__((weak, alias("Default_Handler")));
void AES1_IRQHandler(void)                __attribute__((weak, alias("Default_Handler")));
void AES2_IRQHandler(void)                __attribute__((weak, alias("Default_Handler")));
void RNG_IRQHandler(void)                 __attribute__((weak, alias("Default_Handler")));
void FPU_IRQHandler(void)                 __attribute__((weak, alias("Default_Handler")));
void DMA2_Channel1_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void DMA2_Channel2_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void DMA2_Channel3_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void DMA2_Channel4_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void DMA2_Channel5_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void DMA2_Channel6_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void DMA2_Channel7_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void DMAMUX1_OVR_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));

/* Vector table - placed at beginning of Flash */
__attribute__((section(".isr_vector")))
const void *g_pfnVectors[] = {
    &_estack,                                   /* Initial Stack Pointer */
    Reset_Handler,                              /* Reset Handler */
    NMI_Handler,                                /* NMI Handler */
    HardFault_Handler,                          /* Hard Fault Handler */
    MemManage_Handler,                          /* MPU Fault Handler */
    BusFault_Handler,                           /* Bus Fault Handler */
    UsageFault_Handler,                         /* Usage Fault Handler */
    0, 0, 0, 0,                                 /* Reserved */
    SVC_Handler,                                /* SVCall Handler */
    DebugMon_Handler,                           /* Debug Monitor Handler */
    0,                                          /* Reserved */
    PendSV_Handler,                             /* PendSV Handler */
    SysTick_Handler,                            /* SysTick Handler */
    
    /* External Interrupts */
    WWDG_IRQHandler,
    PVD_PVM_IRQHandler,
    TAMP_STAMP_LSECSS_IRQHandler,
    RTC_WKUP_IRQHandler,
    FLASH_IRQHandler,
    RCC_IRQHandler,
    EXTI0_IRQHandler,
    EXTI1_IRQHandler,
    EXTI2_IRQHandler,
    EXTI3_IRQHandler,
    EXTI4_IRQHandler,
    DMA1_Channel1_IRQHandler,
    DMA1_Channel2_IRQHandler,
    DMA1_Channel3_IRQHandler,
    DMA1_Channel4_IRQHandler,
    DMA1_Channel5_IRQHandler,
    DMA1_Channel6_IRQHandler,
    DMA1_Channel7_IRQHandler,
    ADC1_IRQHandler,
    USB_HP_IRQHandler,
    USB_LP_IRQHandler,
    C2SEV_PWR_C2H_IRQHandler,
    COMP_IRQHandler,
    EXTI9_5_IRQHandler,
    TIM1_BRK_IRQHandler,
    TIM1_UP_TIM16_IRQHandler,
    TIM1_TRG_COM_TIM17_IRQHandler,
    TIM1_CC_IRQHandler,
    TIM2_IRQHandler,
    PKA_IRQHandler,
    I2C1_EV_IRQHandler,
    I2C1_ER_IRQHandler,
    I2C3_EV_IRQHandler,
    I2C3_ER_IRQHandler,
    SPI1_IRQHandler,
    SPI2_IRQHandler,
    USART1_IRQHandler,
    LPUART1_IRQHandler,
    SAI1_IRQHandler,
    TSC_IRQHandler,
    EXTI15_10_IRQHandler,
    RTC_Alarm_IRQHandler,
    CRS_IRQHandler,
    PWR_SOTF_BLEACT_802ACT_RFPHASE_IRQHandler,
    IPCC_C1_RX_IRQHandler,
    IPCC_C1_TX_IRQHandler,
    HSEM_IRQHandler,
    LPTIM1_IRQHandler,
    LPTIM2_IRQHandler,
    LCD_IRQHandler,
    QUADSPI_IRQHandler,
    AES1_IRQHandler,
    AES2_IRQHandler,
    RNG_IRQHandler,
    FPU_IRQHandler,
    DMA2_Channel1_IRQHandler,
    DMA2_Channel2_IRQHandler,
    DMA2_Channel3_IRQHandler,
    DMA2_Channel4_IRQHandler,
    DMA2_Channel5_IRQHandler,
    DMA2_Channel6_IRQHandler,
    DMA2_Channel7_IRQHandler,
    DMAMUX1_OVR_IRQHandler
};
