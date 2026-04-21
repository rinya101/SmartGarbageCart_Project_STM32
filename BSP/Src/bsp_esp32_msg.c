#include "bsp_esp32_msg.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "bsp_debug.h"
#ifndef NULL
#define NULL (void*)0
#endif
#define ESP32_MSG_BUF_CR    '\r'
#define ESP32_MSG_BUF_LF    '\n'
#define EPS32_MSG_END_NULL  '\0'
#define EPS32_MSG_TIMEOUT   1000
/**
 * @brief ESP32 通讯缓冲区大小
 * 
 */
static uint8_t esp32_msg_buf[ESP32_MSG_BUF_SIZE];
/**
 * @brief ESP32 私有通讯句柄
 * @note 作为中断锚点
 */
static esp32_msg_handle_t* s_msg_handle;
/**
 * @brief ESP32 通讯 接收数据中断服务
 * 
 * @param pamam 
 */
void esp32_msg_IRQHandler(void *pamam)
{
    if (USART_GetITStatus(s_msg_handle->esp32_msg_dev.usart, USART_IT_RXNE) != RESET)
    {
        uint8_t ch = USART_ReceiveData(s_msg_handle->esp32_msg_dev.usart);
        if (ch == ESP32_MSG_BUF_LF)
        {
            if (s_msg_handle->buf_index < s_msg_handle->buf_len - 2)
            {
                s_msg_handle->buf[s_msg_handle->buf_index++] = ESP32_MSG_BUF_LF;
            }
            s_msg_handle->buf[s_msg_handle->buf_index] = EPS32_MSG_END_NULL;
            s_msg_handle->rx_len = s_msg_handle->buf_index;
            s_msg_handle->new_msg_flag = 1;
            s_msg_handle->buf_index = 0;
        }
        else if (s_msg_handle->buf_index < s_msg_handle->buf_len - 2)
        {
            s_msg_handle->buf[s_msg_handle->buf_index++] = ch;
        }
        USART_ClearITPendingBit(s_msg_handle->esp32_msg_dev.usart, USART_IT_RXNE);
    }
}



/**
 * @brief ESP32 通讯时钟初始化
 * 
 * @param cfg 
 */
static void esp32_msg_clk_init(esp32_msg_cfg_t *cfg)
{
    /* GPIO RCC */
    RCC_AHB1PeriphClockCmd(cfg->gpio_rcc, ENABLE);
    /* USART RCC */
    RCC_APB2PeriphClockCmd(cfg->usart_rcc, ENABLE);
}
/**
 * @brief ESP32 通讯 GPIO 初始化
 * 
 * @param cfg 
 */
static void esp32_msg_gpio_init(esp32_msg_cfg_t *cfg)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin     = cfg->gpio_tx_pin;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType   = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
    GPIO_Init(cfg->gpio_tx_port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin     = cfg->gpio_rx_pin;
    GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
    GPIO_Init(cfg->gpio_rx_port, &GPIO_InitStructure);

    GPIO_PinAFConfig(cfg->gpio_tx_port, cfg->gpio_tx_pin_src, cfg->gpio_tx_pin_af);
    GPIO_PinAFConfig(cfg->gpio_rx_port, cfg->gpio_rx_pin_src, cfg->gpio_rx_pin_af);
}
/**
 * @brief ESP32 通讯 USART 初始化
 * 
 * @param cfg 
 */
static void esp32_msg_usart_init(esp32_msg_cfg_t *cfg)
{
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate              = cfg->usart_baudrate;
    USART_InitStructure.USART_Mode                  = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_Parity                = cfg->usart_parity;
    USART_InitStructure.USART_StopBits              = cfg->usart_stopbits;
    USART_InitStructure.USART_WordLength            = cfg->usart_wordlen;
    USART_InitStructure.USART_HardwareFlowControl   = USART_HardwareFlowControl_None;
    USART_Init(cfg->usart, &USART_InitStructure);
    /* 开启 USART */
    USART_Cmd(cfg->usart, ENABLE);
    /* 开启中断 */
    USART_ITConfig(cfg->usart, USART_IT_RXNE, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                      = cfg->usart_irq;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority           = cfg->usart_irq_subpriority;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = cfg->usart_irq_preemptionpriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**
 * @brief  ESP32 通讯初始化
 * 
 * @param handle  句柄
 * @param cfg     配置
 */
void bsp_esp32_msg_init(esp32_msg_handle_t *handle, esp32_msg_cfg_t *cfg)
{
    if (handle == NULL || cfg == NULL)
    {
        return;
    }
    if (handle->buf == NULL)    /* 用户没有指定 缓存区 */
    {
        handle->buf = esp32_msg_buf;                /* 使用默认缓冲区 */
        handle->buf_len = sizeof(esp32_msg_buf);    /* 缓冲区大小 */
        handle->buf_index = 0;                      /* 缓冲区索引 */
    }
    else                        /* 用户指定缓冲区 */
    {
        handle->buf = cfg->buf;
        handle->buf_len = cfg->buf_len;
        handle->buf_index = 0;
    }

    handle->new_msg_flag = 0;
    handle->esp32_msg_dev.usart = cfg->usart;

    s_msg_handle = handle; /* 保存句柄 */
    /* RCC */
    esp32_msg_clk_init(cfg);
    /* GPIO */
    esp32_msg_gpio_init(cfg);
    /* USART */
    esp32_msg_usart_init(cfg);
}
/**
 * @brief 消息发送
 * 
 * @param handle 
 * @param data 
 * @param len 
 */
void bsp_esp32_msg_send(esp32_msg_handle_t *handle, uint8_t *data, uint16_t len)
{
    if (handle == NULL || data == NULL) return;
    while(len--)
    {
        uint32_t timeout = EPS32_MSG_TIMEOUT;
        USART_SendData(handle->esp32_msg_dev.usart, *data++);
        while(USART_GetFlagStatus(handle->esp32_msg_dev.usart, USART_FLAG_TC) == RESET && timeout--);
    }
}
