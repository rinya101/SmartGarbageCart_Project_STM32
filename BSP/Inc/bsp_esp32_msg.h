#ifndef _BSP_ESP32_MSG_H
#define _BSP_ESP32_MSG_H
#include "stm32f4xx_cryp.h"
#define ESP32_MSG_BUF_SIZE  64

typedef struct esp32_msg_handle esp32_msg_handle_t;

/**
 * @brief ESP32消息 配置
 * 
 */
typedef struct 
{
    /* RCC */
    uint32_t     gpio_rcc;
    uint32_t     usart_rcc;
    /* GPIO */
    GPIO_TypeDef*       gpio_tx_port;
    GPIO_TypeDef*       gpio_rx_port;
    uint32_t            gpio_tx_pin;
    uint32_t            gpio_rx_pin;
    uint16_t            gpio_tx_pin_src;
    uint16_t            gpio_rx_pin_src;
    uint8_t             gpio_tx_pin_af;
    uint8_t             gpio_rx_pin_af;
    /* USART */
    USART_TypeDef*      usart;
    uint32_t            usart_baudrate;
    uint16_t            usart_parity;
    uint16_t            usart_stopbits;
    uint16_t            usart_wordlen;
    /* NVIC */
    uint8_t             usart_irq;
    uint8_t             usart_irq_subpriority;
    uint8_t             usart_irq_preemptionpriority;
    /* BUFFER */
    uint8_t *buf;       /* 接收数据缓冲区 */
    uint16_t buf_len;   /* 缓冲区长度 */
    uint16_t buf_index; /* 缓冲区索引 */
} esp32_msg_cfg_t;

struct esp32_msg_handle
{
    uint8_t *buf;       /* 接收数据缓冲区 */
    uint16_t buf_len;   /* 缓冲区长度 */
    uint16_t buf_index; /* 缓冲区索引 */
    uint8_t  new_msg_flag; /* 新消息标志 */
    uint16_t rx_len;    /* 接收数据长度 */
    struct 
    {
        USART_TypeDef*      usart;
    } esp32_msg_dev;
    struct 
    {
        void (*rx_callback)(void *param);
    } esp32_msg_callback;
};

void bsp_esp32_msg_init(esp32_msg_handle_t *handle, esp32_msg_cfg_t *cfg);
void bsp_esp32_msg_send(esp32_msg_handle_t *handle, uint8_t *data, uint16_t len);


#endif