# 使用说明

## USART 串口

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

## ENCODER 编码器
### 示例
```C
#include "stm32f4xx.h"
#include "SysClockConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "bsp_usart.h"
#include <string.h>
#include "bsp_encoder.h"
void app(void *pvParameters);
void led_init(void);

int main(void)
{
    /* 时钟配置 */
    SysClockConfig_HSE25MHZ();
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
    /* 创建任务 */
    if(xTaskCreate(app, "app", 512, NULL, 5, NULL) != pdPASS)
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
    /* 编码器配置 */
    g_encoder = pvPortMalloc(sizeof(encoder_handle_t));
    if (g_encoder == NULL)
    {
        while(1); /* 内存不足 */
    }
    memset(g_encoder, 0, sizeof(encoder_handle_t));
    encoder_cfg_t *encoder_cfg = pvPortMalloc(sizeof(encoder_cfg_t));
    if (encoder_cfg == NULL)
    {
        while(1); /* 内存不足 */
    }
    memset(encoder_cfg, 0, sizeof(encoder_cfg_t));
    *encoder_cfg = ENCODER_DEFAULT_CONFIG();
    encoder_attach_callback(encoder_cfg);
    encoder_attach_event(encoder_cfg);
    encoder_init(g_encoder, *encoder_cfg);
    vPortFree(encoder_cfg);/* 释放内存 */
    uint16_t count = 0;
    while(1)
    {
        count++;
        vTaskDelay(50);
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
```
### 全局变量
```C
encoder_handle_t* g_encoder;/* 编码器句柄 */
```
### 库函数
```C
/* 注意使用顺序 如下 */
void encoder_attach_callback(encoder_cfg_t *cfg);
void encoder_attach_event(encoder_cfg_t *cfg);
void encoder_init(encoder_handle_t* handle, encoder_cfg_t cfg);
```
### 函数指针
```C
/**
 * @brief 编码器回调函数
 * 
 */
typedef void (*encoder_cb_t)(encoder_handle_t *handle);
/**
 * @brief 编码器按键回调函数
 * 
 */
typedef void (*encoder_btn_cb_t)(encoder_handle_t *handle);
/**
 * @brief 编码器左转事件
 * 
 */
typedef void (*encoder_left_event)(encoder_handle_t* encoder);
/**
 * @brief 编码器右转事件
 * 
 */
typedef void (*encoder_right_event)(encoder_handle_t* encoder);
/**
 * @brief 编码器按键按下事件
 * 
 */
typedef void (*btn_press_event)(encoder_handle_t* encoder);
/**
 * @brief 编码器按键长按事件
 * 
 */
typedef void (*btn_long_press_event)(encoder_handle_t* encoder);
```
### 枚举
```C
/**
 * @brief 按键状态枚举
 * 
 */
typedef enum 
{
    UNKNOW = 0,
    PRESS = 1,
    RELEASE = -1,
    LONG_PRESS = 2
} key_state_e;
/**
 * @brief 转向枚举
 * 
 */
typedef enum 
{
    STOP = 0,
    LEFT = -1,
    RIGHT = 1
} encoder_turn_t;
```
### 结构体
```C
/**
 * @brief 编码器配置结构体
 * 
 */
typedef struct 
{
    /* GPIO */
    GPIO_TypeDef*   btn_port;
    uint32_t        btn_pin;
    GPIO_TypeDef*   left_port;
    uint32_t        left_pin;
    GPIO_TypeDef*   right_port;
    uint32_t        right_pin;
    /* EXIT */
    uint32_t                btn_exti_line;    
    EXTITrigger_TypeDef     btn_trigger;
    uint32_t                left_exti_line;
    EXTITrigger_TypeDef     left_trigger;
    /* NVIC */
    uint8_t                 btn_irq_channel;
    uint8_t                 left_irq_channel;
    uint8_t                 preemption_priority;
    uint8_t                 sub_priority;
    /* CallBack */
    encoder_cb_t            event_cb;
    encoder_btn_cb_t        btn_cb;
    /* EVENT */
    encoder_left_event      left_event;
    encoder_right_event     right_event;
    btn_press_event         btn_press;
    btn_long_press_event    btn_long_press;

} encoder_cfg_t;
/**
 * @brief 编码器句柄
 * 
 */
struct encoder_handle
{
    /* GPIO */
    GPIO_TypeDef*   btn_port;
    uint32_t        btn_pin;
    GPIO_TypeDef*   left_port;
    uint32_t        left_pin;
    GPIO_TypeDef*   right_port;
    uint32_t        right_pin;
    /* MISC */
    uint8_t             dir;
    int16_t             count;
    key_state_e         btn_state;
    encoder_turn_t      turn_state;
    encoder_cb_t        encoder_cb;
    encoder_btn_cb_t    btn_cb;
    uint32_t            btn_line;
    uint32_t            left_line;
    /* EVENT */
    encoder_left_event      left_event;
    encoder_right_event     right_event;
    btn_press_event         btn_press;
    btn_long_press_event    btn_long_press;
};
```

## TIM5
### 全局变量
```C
tim5_handle_t g_tim5_handle;
```
### 库函数
```C
/* -------------------------- TIM5 配置（注意使用顺序） ---------------------- */
void bsp_tim5_attach_callback(tim5_handle_t* handle);
void bsp_tim5_init(tim5_handle_t* handle, tim5_cfg_t cfg);
/* ---------------------------------- TIM5 使用 ----------------------------- */
void bsp_tim5_timing(uint16_t period, uint8_t user_id);
void bsp_tim5_stop(void);
```
### 函数指针
```C
/**
 * @brief 编码器回调函数
 * 
 */
typedef void (*encoder_cb_t)(encoder_handle_t *handle);
/**
 * @brief 编码器按键回调函数
 * 
 */
typedef void (*encoder_btn_cb_t)(encoder_handle_t *handle);
/**
 * @brief 编码器左转事件
 * 
 */
typedef void (*encoder_left_event)(encoder_handle_t* encoder);
/**
 * @brief 编码器右转事件
 * 
 */
typedef void (*encoder_right_event)(encoder_handle_t* encoder);
/**
 * @brief 编码器按键按下事件
 * 
 */
typedef void (*btn_press_event)(encoder_handle_t* encoder);
/**
 * @brief 编码器按键长按事件
 * 
 */
typedef void (*btn_long_press_event)(encoder_handle_t* encoder);
```
### 枚举
```C
/**
 * @brief 按键状态枚举
 * 
 */
typedef enum 
{
    UNKNOW = 0,
    PRESS = 1,
    RELEASE = -1,
    LONG_PRESS = 2
} key_state_e;
/**
 * @brief 转向枚举
 * 
 */
typedef enum 
{
    STOP = 0,
    LEFT = -1,
    RIGHT = 1
} encoder_turn_t;
```
### 结构体
```C
/**
 * @brief 编码器配置结构体
 * 
 */
typedef struct 
{
    /* GPIO */
    GPIO_TypeDef*   btn_port;
    uint32_t        btn_pin;
    GPIO_TypeDef*   left_port;
    uint32_t        left_pin;
    GPIO_TypeDef*   right_port;
    uint32_t        right_pin;
    /* EXIT */
    uint32_t                btn_exti_line;    
    EXTITrigger_TypeDef     btn_trigger;
    uint32_t                left_exti_line;
    EXTITrigger_TypeDef     left_trigger;
    /* NVIC */
    uint8_t                 btn_irq_channel;
    uint8_t                 left_irq_channel;
    uint8_t                 preemption_priority;
    uint8_t                 sub_priority;
    /* CallBack */
    encoder_cb_t            event_cb;
    encoder_btn_cb_t        btn_cb;
    /* EVENT */
    encoder_left_event      left_event;
    encoder_right_event     right_event;
    btn_press_event         btn_press;
    btn_long_press_event    btn_long_press;

} encoder_cfg_t;
/**
 * @brief 编码器句柄
 * 
 */
struct encoder_handle
{
    /* GPIO */
    GPIO_TypeDef*   btn_port;
    uint32_t        btn_pin;
    GPIO_TypeDef*   left_port;
    uint32_t        left_pin;
    GPIO_TypeDef*   right_port;
    uint32_t        right_pin;
    /* MISC */
    uint8_t             dir;
    int16_t             count;
    key_state_e         btn_state;
    encoder_turn_t      turn_state;
    encoder_cb_t        encoder_cb;
    encoder_btn_cb_t    btn_cb;
    uint32_t            btn_line;
    uint32_t            left_line;
    /* EVENT */
    encoder_left_event      left_event;
    encoder_right_event     right_event;
    btn_press_event         btn_press;
    btn_long_press_event    btn_long_press;
};
```
## OLED 显示屏
### 示例
```C
#include "stm32f4xx.h"
#include "SysClockConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "bsp_usart.h"
#include <string.h>
#include "bsp_encoder.h"
#include "bsp_oled.h"
void app(void *pvParameters);
void led_init(void);

int main(void)
{
    /* 时钟配置 */
    SysClockConfig_HSE25MHZ();
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
    /* 创建任务 */
    if(xTaskCreate(app, "app", 512, NULL, 5, NULL) != pdPASS)
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
    /* OLED 初始化 */
    oled_handle_t* g_oled_handle = pvPortMalloc(sizeof(oled_handle_t));
    if (g_oled_handle == NULL) while(1);
    memset(g_oled_handle, 0, sizeof(oled_handle_t));
    bsp_oled_init(g_oled_handle, &OLED_DEFAULT_CONFIG());
    for (int i = 0; i < 1024; i++)
    {
        g_oled_handle->screen_buf[i] = 0xAA;
    }
    bsp_oled_refresh(g_oled_handle);
    uint16_t count = 0;
    printf("g_oled_handle size: %d\r\n",sizeof(*g_oled_handle));
    while(1)
    {
        for (int i = 0; i < 1024; i++)
        {
            g_oled_handle->screen_buf[i] = 0x00;
            bsp_oled_refresh(g_oled_handle);
            count++;
            printf("count = %d\n", count);
            vTaskDelay(5);
        }
        for (int i = 0; i < 1024; i++)
        {
            g_oled_handle->screen_buf[i] = 0xFF;
            bsp_oled_refresh(g_oled_handle);
            count--;
            printf("count = %d\n", count);
            vTaskDelay(5);
        }
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

```
### 全局变量
```C
/**
 * @brief OLED 句柄
 * @note 锚点
 */
oled_handle_t* s_oled_handle = NULL;
/**
 * @brief OLED  显存
 * 
 */
static uint8_t s_screen_buf[OLED_WIDTH * OLED_HEIGHT / OLED_PAGE_SIZE];
```
### 库函数
```C
void bsp_oled_init(oled_handle_t* oled_handle, oled_cfg_t* cfg);
void bsp_oled_draw_point(oled_handle_t* oled_handle, uint8_t x, uint8_t y, uint8_t data);
void bsp_oled_refresh(oled_handle_t* oled_handle);
```
### 结构体
```C
/**
 * @brief OLED 配置结构体
 * 
 */
typedef struct 
{
    /* GPIO */
    uint32_t            scl_clk;
    uint32_t            sda_clk;
    GPIO_TypeDef*       scl_port;
    GPIO_TypeDef*       sda_port;
    uint32_t            scl_pin;
    uint32_t            sda_pin;
    GPIOPuPd_TypeDef    scl_pu;
    GPIOPuPd_TypeDef    sda_pu;
    GPIOSpeed_TypeDef   scl_speed;
    GPIOSpeed_TypeDef   sda_speed;
    GPIOMode_TypeDef    scl_mode;
    GPIOMode_TypeDef    sda_mode;
    GPIOOType_TypeDef   scl_otype;
    GPIOOType_TypeDef   sda_otype;
    uint16_t            scl_source;
    uint16_t            sda_source;
    uint8_t             scl_af;
    uint8_t             sda_af;
    /* I2Cx */
    uint32_t        i2c_clk;
    I2C_TypeDef*    i2cx;
    uint16_t        i2c_mode;
    uint16_t        i2c_ack;
    uint16_t        i2c_ack_addr;
    uint16_t        i2c_duty;
    uint32_t        i2c_speed;
    uint16_t        i2c_own_addr;   /* 本机地址 */
    /* DMA */
    uint32_t                dma_clk;
    DMA_Stream_TypeDef*     dma_stream;
    uint32_t                tx_dma_channel;
    uint32_t                dma_priority;
    uint8_t                 dma_irq_channel;
    uint8_t                 dma_irq_prio;
    uint8_t                 dma_irq_sub_prio;
    /* SYSCFG (部分私有) */
    uint32_t                syscfg_clk;
    /* OLED */
    uint8_t                 i2c_addr;  /* 从机地址 */
    uint16_t                width;
    uint16_t                height;
} oled_cfg_t;
/**
 * @brief OLED 句柄结构体
 * @note 包含 I2C、DMA、全屏缓存、显示参数
 */
struct oled_handle
{
    /* I2C */
    I2C_TypeDef*        i2cx;
    /* OLED */
    uint8_t             width;
    uint8_t             height;
    uint8_t*            screen_buf;  /* 全屏显示缓存 */
    uint8_t             oled_addr;
    /* DMA */
    DMA_Stream_TypeDef*     dma_stream;
};
```
## 超声波模块

### 示例
```C

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
void app(void *pvParameters);
void led_init(void);

int main(void)
{
    /* 时钟配置 */
    SysClockConfig_HSE25MHZ();
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
    /* 编码器配置 */
    encoder_handle_t* g_encoder = pvPortMalloc(sizeof(encoder_handle_t));
    if (g_encoder == NULL)
    {
        while(1); /* 内存不足 */
    }
    memset(g_encoder, 0, sizeof(encoder_handle_t));
    encoder_cfg_t *encoder_cfg = pvPortMalloc(sizeof(encoder_cfg_t));
    if (encoder_cfg == NULL)
    {
        while(1); /* 内存不足 */
    }
    memset(encoder_cfg, 0, sizeof(encoder_cfg_t));
    *encoder_cfg = ENCODER_DEFAULT_CONFIG();
    encoder_attach_callback(encoder_cfg);
    encoder_attach_event(encoder_cfg);
    encoder_init(g_encoder, encoder_cfg);
    vPortFree(encoder_cfg);/* 释放内存 */
    /* 超声波模块1初始化 */
    ult_handle_t ult1_handle;
    ult_cfg_t ult1_cfg;
    ult1_cfg = ULT_FIRST_DEFAULT_CONDFIG();
    bsp_ultrasonic_init(&ult1_handle, &ult1_cfg);
    /* 超声波模块2 */
    ult_handle_t ult2_handle;
    ult_cfg_t ult2_cfg;
    ult2_cfg = ULT_SECOND_DEFAULT_CONDFIG();
    bsp_ultrasonic_init(&ult2_handle, &ult2_cfg);
    /* 超声波模块3 */
    ult_handle_t ult3_handle;
    ult_cfg_t ult3_cfg;
    ult3_cfg = ULT_THIRD_DEFAULT_CONDFIG();
    bsp_ultrasonic_init(&ult3_handle, &ult3_cfg);
    uint16_t count = 0;
    while(1)
    {
        count++;
        if (ult1_handle.state == ULT_READY)
        {
            bsp_ultrasonic_trigger(&ult1_handle);
        }
        if (ult2_handle.state == ULT_READY)
        {
            bsp_ultrasonic_trigger(&ult2_handle);
        }
        if (ult3_handle.state == ULT_READY)
        {
            bsp_ultrasonic_trigger(&ult3_handle);
        }
        vTaskDelay(50);
        // printf("first distance:%d\n second distance:%d\n",
        //     bsp_ultrasonic_get_distance(&ult1_handle),
        //     bsp_ultrasonic_get_distance(&ult2_handle));
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

```
### 私有全局变量
```C
ult_handle_t* s_ult_first_handle = NULL;
ult_handle_t* s_ult_second_handle = NULL;
ult_handle_t* s_ult_third_handle = NULL;
```

### 库函数
```C
void bsp_ultrasonic_init(ult_handle_t* handle, ult_cfg_t* cfg);
uint16_t bsp_ultrasonic_get_distance(ult_handle_t* handle);
void bsp_ultrasonic_trigger(ult_handle_t *handle);
```
### 枚举
```C
/**
 * @brief 超声波状态枚举
 * 
 */
typedef enum 
{
    ULT_BUSY,
    ULT_READY,
} ult_sta_t;
```
### 结构体
```C
/**
 * @brief 超声波模块初始化
 * 
 */
typedef struct 
{
    /* NUM */
    uint8_t             num;
    /* RCC */
    uint32_t            tr_gpio_rcc;
    uint32_t            eh_gpio_rcc;
    uint32_t            tim_rcc;
    /* GPIO */
    GPIO_TypeDef*       tr_gpio_port;
    GPIO_TypeDef*       eh_gpio_port;
    uint16_t            tr_gpio_pin;
    uint16_t            eh_gpio_pin;
    GPIOMode_TypeDef    tr_gpio_mode;
    GPIOMode_TypeDef    eh_gpio_mode;
    GPIOOType_TypeDef   tr_gpio_otype;
    GPIOOType_TypeDef   eh_gpio_otype;
    GPIOSpeed_TypeDef   tr_gpio_speed;  
    GPIOSpeed_TypeDef   eh_gpio_speed;
    GPIOPuPd_TypeDef    tr_gpio_pupd;
    GPIOPuPd_TypeDef    eh_gpio_pupd;
    /* EXTI */
    uint8_t             eh_exti_line;
    EXTIMode_TypeDef    eh_exti_mode;
    EXTITrigger_TypeDef eh_exti_trigger;
    /* NVIC */
    uint8_t             eh_nvic_irqn;
    uint8_t             eh_nvic_pri;
    uint8_t             eh_nvic_subpri;
    uint8_t             tim_nvic_irqn;
    uint8_t             tim_nvic_pri;
    uint8_t             tim_nvic_subpri;
    /* TIM */
    TIM_TypeDef*        timx;
    uint16_t            tim_prescaler;
    uint32_t            tim_period;
    uint16_t            tim_counter_mode;
    uint16_t            tim_division;
} ult_cfg_t;


typedef struct 
{
    /* EXTI */
    uint8_t             eh_exti_line;
    /* TIM */
    TIM_TypeDef*        timx;
    /* GPIO */
    GPIO_TypeDef*       tr_gpio_port;
    uint16_t            tr_gpio_pin;
    GPIO_TypeDef*       eh_gpio_port;
    uint16_t            eh_gpio_pin;
} ult_dev_handle_t;
/**
 * @brief 超声波句柄
 * 
 */
struct ult_handle
{
    /* base */
    uint16_t distance_cm;
    uint8_t  num;
    uint8_t  state;
    ult_dev_handle_t dev_handle;
};
```

## 循迹模块
### 示例
```C

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
void app(void *pvParameters);
void led_init(void);

int main(void)
{
    /* 时钟配置 */
    SysClockConfig_HSE25MHZ();
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
    /* 编码器配置 */
    encoder_handle_t* g_encoder = pvPortMalloc(sizeof(encoder_handle_t));
    if (g_encoder == NULL)
    {
        while(1); /* 内存不足 */
    }
    memset(g_encoder, 0, sizeof(encoder_handle_t));
    encoder_cfg_t *encoder_cfg = pvPortMalloc(sizeof(encoder_cfg_t));
    if (encoder_cfg == NULL)
    {
        while(1); /* 内存不足 */
    }
    memset(encoder_cfg, 0, sizeof(encoder_cfg_t));
    *encoder_cfg = ENCODER_DEFAULT_CONFIG();
    encoder_attach_callback(encoder_cfg);
    encoder_attach_event(encoder_cfg);
    encoder_init(g_encoder, encoder_cfg);
    vPortFree(encoder_cfg);/* 释放内存 */
    /* 循迹模块 A路 初始化 */
    track_handle_t trackA_handle;
    track_cfg_t trackA_cfg = TRACKER_A_DEFAULT_CONFIG();
    bsp_tracking_init(&trackA_handle, &trackA_cfg);
    /* 循迹模块 B路 初始化 */
    track_handle_t trackB_handle;
    track_cfg_t trackB_cfg = TRACKER_B_DEFAULT_CONFIG();
    bsp_tracking_init(&trackB_handle, &trackB_cfg);
    uint16_t count = 0;
    while(1)
    {
        count++;
        vTaskDelay(50);
        printf("track_A_state = %s\ntrack_B_state = %s\n", 
            trackA_handle.sta ? "On_line": "Off_line",
            trackB_handle.sta ? "On_line": "Off_line");
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

```
### 私有全局变量
```C
/**
 * @brief 五路循迹私有句柄指针
 * 
 */
track_handle_t* s_track_a_handle = NULL;
track_handle_t* s_track_b_handle = NULL;
track_handle_t* s_track_c_handle = NULL;
track_handle_t* s_track_d_handle = NULL;
track_handle_t* s_track_e_handle = NULL;
```
### 库函数
```C
void bsp_tracking_init(track_handle_t* handle, track_cfg_t *cfg);
```
### 枚举
```C
/**
 * @brief 循迹模块ID枚举
 * 
 */
typedef enum
{
    TRACKER_A,
    TRACKER_B,
    TRACKER_C,  
    TRACKER_D,
    TRACKER_E,
} track_id_t;
/**
 * @brief 循迹模块状态枚举
 * 
 */
typedef enum
{
    TRACKER_STATE_NONE = -1,
    TRACKER_STATE_WHITE = 0,
    TRACKER_STATE_BLACK = 1,
}track_state_t;
```
### 结构体
```C

/**
 * @brief 循迹初始化
 * 
 */
typedef struct 
{
    /* TRACKER */
    track_id_t          id;
    /* RCC */
    uint32_t            rcc_gpio;
    /* GPIO */
    GPIO_TypeDef*       gpio_port;
    uint32_t            gpio_pin;
    GPIOMode_TypeDef    gpio_mode;
    GPIOOType_TypeDef   gpio_otype;
    GPIOSpeed_TypeDef   gpio_speed;
    GPIOPuPd_TypeDef    gpio_pupd;
    /* EXTI */
    uint32_t            exti_line;
    EXTIMode_TypeDef    exti_mode;
    EXTITrigger_TypeDef exti_trigger;
    /* NVIC */
    uint8_t             nvic_irqn;
    uint8_t             nvic_pri;
    uint8_t             nvic_subpri;
} track_cfg_t;

/**
 * @brief 循迹设备句柄
 * 
 */
typedef struct 
{
    /* EXTI */
    uint32_t            exti_line;
    /* GPIO */
    GPIO_TypeDef*       gpio_port;
    uint32_t            gpio_pin;
} track_dev_handle_t;

/**
 * @brief 循迹句柄
 * 
 */
struct track_handle
{
    /* BASE */
    track_id_t          id;
    track_state_t       sta;
    /* Dev */
    track_dev_handle_t  dev;
};
```