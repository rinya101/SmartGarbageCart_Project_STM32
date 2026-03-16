# 使用说明

# USART 串口

### 示例
```C
#include "stm32f4xx.h"
#include "SysClockConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "bsp_usart.h"
#include <string.h>
uint8_t data1[26] = "";
uint8_t data2[26] = "";
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
    /* data 数据计算 */
    for (uint16_t i = 0; i < sizeof(data1) - 1; i++)
    {
        data1[i] = i + 'A';
    }
    data1[25] = '\n';
    for (uint16_t i = 0; i < sizeof(data2) - 1; i++)
    {
        data2[i] = i + 'a';
    }
    data2[25] = '\n';
    usart1_handle = pvPortMalloc(sizeof(usart_handle_t));
    if (usart1_handle == NULL)
    {
        while(1); // 内存不足
    }
    memset(usart1_handle, 0, sizeof(usart_handle_t));
    /* 初始化 USATR1 */
    usart_cfg_t cfg1 = USART1_DEFAULT_CONFIG();
    usart1_handle->dma_buf_size = sizeof(data1);
    usart1_handle->dma_buf      = data1;
    bsp_usart_init(usart1_handle, cfg1);
    vTaskDelay(1000);
    bsp_usart_send_dma(usart1_handle);
    usart1_handle->dma_buf_size = sizeof(data2);
    usart1_handle->dma_buf      = data2;
    vTaskDelay(1000);
    bsp_usart_send_dma(usart1_handle);
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
```
### 库函数
1. `void bsp_usart_init(usart_handle_t* handle, usart_cfg_t cfg)`
2. `void bsp_usart_send_byte(usart_handle_t* handle, uint8_t byte)`
3. `void bsp_usart_send_string(usart_handle_t* handle, char* str, uint16_t len)`
4. `void bsp_usart_send_dma(usart_handle_t* handle)`
### 结构体
```C
/**
 * @brief USART 配置结构体
 * @note NULL
 * 
 */
typedef struct 
{
    /* GPIO */
    USART_TypeDef*  usart;
    GPIO_TypeDef*   tx_port;
    GPIO_TypeDef*   rx_port;
    uint16_t        tx_pin;
    uint16_t        rx_pin;
    uint16_t        usart_tx_source;
    uint16_t        usart_rx_source;
    /* EXIT */
    uint8_t         preemption_priority;
    uint8_t         sub_priority;
    /* USART */
    uint32_t        baudrate;
    uint16_t        usart_mode;
    uint16_t        usart_parity;
    uint16_t        usart_stopbit;
    uint16_t        usart_wordlength;
    uint16_t        hfc;
    /* DMA */
    DMA_Stream_TypeDef* tx_dma_stream;
    uint32_t            tx_dma_channel;

} usart_cfg_t;
```
```C
/**
 * @brief USART 数据句柄
 * 
 */
struct usart_handle
{
    uint8_t             rx_buf[USART_RECEIVE_BUF_SIZE];
    uint8_t*            dma_buf;
    uint16_t            dma_buf_size;
    uint16_t            rx_len;
    uint16_t            buf_index;
    uint8_t             new_msg_flag;
    usart_rx_callback_t rx_callback;
    USART_TypeDef*      usart;
    DMA_Stream_TypeDef* tx_dma_stream;
};
```