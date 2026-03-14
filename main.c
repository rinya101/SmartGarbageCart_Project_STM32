#include "stm32f4xx.h"
#include "SysClockConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "bsp_usart.h"
#include <string.h>

void app(void *pvParameters);
void led_init(void);
/**
 * @brief 接收一组数据回调
 * 
 * @param handle 
 */
void usart1_rx_callback(usart_handle_t *handle)
{
    // 回调事件
    bsp_usart_send_string(handle, "usart1_rx_callback\r\n", handle->rx_len);

}
int main(void)
{
    /* 时钟配置 */
    SysClockConfig_HSE25MHZ();
    /* 创建任务 */
    xTaskCreate(app, "app", 512, NULL, 5, NULL);
    vTaskStartScheduler();
}

/**
 * @brief 用户任务
 */
void app(void *pvParameters)
{
    usart1_handle = pvPortMalloc(sizeof(usart_handle_t));
    if (usart1_handle == NULL)
    {
        while(1); // 内存不足
    }
    memset(usart1_handle, 0, sizeof(usart_handle_t));
    /* 初始化 USATR1 */
    usart_cfg_t cfg1 = USART1_DEFAULT_CONFIG();
    cfg1.sub_priority    = 1;
    bsp_usart_init(usart1_handle, cfg1);

    usart2_handle = pvPortMalloc(sizeof(usart_handle_t));
    if (usart2_handle == NULL)
    {
        while(1); // 内存不足
    }
    memset(usart2_handle, 0, sizeof(usart_handle_t));
    /* 初始化 USATR2 */
    usart_cfg_t cfg2 = USART2_DEFAULT_CONFIG();
    cfg2.sub_priority    = 2;
    bsp_usart_init(usart2_handle, cfg2);
    uint16_t count = 0;
    while(1)
    {
        count++;
        printf("count = %d\r\n", count);
        vTaskDelay(1000);
        if (usart1_handle != NULL)
        {
            if (usart1_handle->new_msg_flag)
            {
                usart1_handle->new_msg_flag = 0;
                printf("usart Receive: %s\n",usart1_handle->rx_buf);
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