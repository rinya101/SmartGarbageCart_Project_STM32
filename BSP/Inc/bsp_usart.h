/*******************************************************************************
 *      文    件: bsp_usart.c
 *      说    明: 适用于STM32F4(具体为 STM32F411CEU6) 的 USART BSP驱动
 *      版    本: V1.0
 *      作    者: Rinya101(http://github.com/rinya101) 学号：220802040137
 *      版权说明：本程序代码仅用于 2026 本科毕业设计，不得用于其他商业用途，
 *               可以作为个人参考学习使用。
*******************************************************************************/
#ifndef _BSP_USART_H
#define _BSP_USART_H
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_dma.h"
#include "bsp_pin_cfg.h"
#include "PeripheralParamConfig.h"
#include <stdio.h>

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
    uint8_t             tx_dma_irq;
    uint8_t             dma_preemption_priority;
    uint8_t             dma_sub_priority;

} usart_cfg_t;
typedef struct usart_handle usart_handle_t;
/**
 * @brief 接收一组数据完成回调函数
 * 
 */
typedef void (*usart_rx_callback_t)(usart_handle_t *handle);
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

/**
 * @brief USART1 句柄指针
 * @note NULL 未定义
 */
extern usart_handle_t*     g_usart1_handle;
/**
 * @brief USART2 句柄指针
 * @note NULL 未定义
 */
extern usart_handle_t*     g_usart2_handle;

void bsp_usart_init(usart_handle_t* handle, usart_cfg_t cfg);

void bsp_usart_send_byte(usart_handle_t* handle, uint8_t byte);

void bsp_usart_send_string(usart_handle_t* handle, char* str, uint16_t len);

void bsp_usart_send_dma(usart_handle_t* handle);
#endif