/*******************************************************************************
 *      文    件: SysClockConfig.c
 *      说    明: 适用于STM32F4(具体为 STM32F411CEU6) 的 时钟配置文件
 *      版    本: V1.0
 *      作    者: Rinya101(http://github.com/rinya101) 学号：220802040137
 *      版权说明：本程序代码仅用于 2026 本科毕业设计，不得用于其他商业用途，
 *               可以作为个人参考学习使用。
*******************************************************************************/
#include "SysClockConfig.h"

void SysClockConfig_HSE25MHZ(void)
{
    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);
    while (RCC_WaitForHSEStartUp() != SUCCESS);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_HCLKConfig(RCC_SYSCLK_Div1);    // HCLK = 100MHz
    RCC_PCLK2Config(RCC_HCLK_Div1);     // APB2 = 100MHz
    RCC_PCLK1Config(RCC_HCLK_Div2);     // APB1 = 50MHz
    RCC_PLLConfig(RCC_PLLSource_HSE, 25, 400, 4, 7);
    RCC_PLLCmd(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
    FLASH_SetLatency(FLASH_Latency_3);
    FLASH_PrefetchBufferCmd(ENABLE);
    FLASH_InstructionCacheCmd(ENABLE);
    FLASH_DataCacheCmd(ENABLE);
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while (RCC_GetSYSCLKSource() != 0x08);
}
