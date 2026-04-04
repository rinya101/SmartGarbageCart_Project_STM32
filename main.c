
#include "stm32f4xx.h"
#include "SysClockConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "bsp_usart.h"
#include <string.h>
#include "bsp_encoder.h"
#include "PeripheralParamConfig.h"
#include "bsp_ultrasonic.h"
#include "bsp_oled.h"
#include "bsp_tracking.h"
#include "bsp_compass.h"
void app(void *pvParameters);
void led_init(void);
oled_handle_t oled;
compass_handle_t compass;
int main(void)
{
    /* 时钟配置 */
    SysClockConfig_HSE25MHZ();
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));
    g_usart1_handle = pvPortMalloc(sizeof(usart_handle_t));
    if (g_usart1_handle == NULL)
    {
        while(1); // 内存不足
    }
    memset(g_usart1_handle, 0, sizeof(usart_handle_t));
    usart_cfg_t *cfg = pvPortMalloc(sizeof(usart_cfg_t));
    if (cfg == NULL)
    {
        while(1);// 内存不足
    }
    memset(cfg, 0, sizeof(cfg));
    *cfg = USART1_DEFAULT_CONFIG();
    bsp_usart_init(g_usart1_handle, *cfg);
    vPortFree(cfg);/* 释放内存 */
    printf("[info] FreeRTOS start!\r\n");
    /* 创建任务 */
    if(xTaskCreate(app, "app", 1024, NULL, 5, NULL) != pdPASS)
    {
        printf("create task failed\r\n");
    }
    vTaskStartScheduler();
    while(1)
    {
        printf("[Error]: FreeRTOS scheduler error!\r\n");
    }
}

/**
 * @brief 用户任务
 */
void app(void *pvParameters)
{
    compass_cfg_t* compass_cfg = &COMPASS_DEFAULT_CONFIG();
    bsp_compass_init(&compass, compass_cfg);
    uint16_t count = 0;
    while(1)
    {
        count++;
        bsp_compass_read(&compass);
        printf("angle: %.2f x:%d\ty:%d\tz:%d\tcount:%d\n",compass.data->angle,
               compass.data->x, compass.data->y, compass.data->z,
               count);
        vTaskDelay(100);
        if (g_usart1_handle != NULL)
        {
            if (g_usart1_handle->new_msg_flag)
            {
                g_usart1_handle->new_msg_flag = 0;
                printf("usart Receive: %s\n",g_usart1_handle->rx_buf);
            }
        }
    }
}
void led_init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}
