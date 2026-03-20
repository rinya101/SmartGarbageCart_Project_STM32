#include "SysBspInit.h"
#include "FreeRTOS.h"
#include "task.h"
#include "string.h"
#include "bsp_encoder.h"
#include "bsp_usart.h"

/**
 * @brief  系统 BSP 层初始化
 * 
 */
void SysBspInit(void)
{
    bsp_usart_global_init();
    bsp_encoder_global_init();
}