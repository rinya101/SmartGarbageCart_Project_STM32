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
#include "bsp_pin_cfg.h"
#include "PeripheralParamConfig.h"
#include <stdio.h>
/**
 * @brief USART 默认初始化配置 
 * @note 修改 usart_tx_source 与 usart_rx_source 对应接口 任务交给程序员自己配置
 */
#define USART1_DEFAULT_CONFIG()     (usart_cfg_t){\
    .usart                  = USART1,\
    .tx_port                = USART1_TX_PORT,\
    .rx_port                = USART1_RX_PORT,\
    .tx_pin                 = USART1_TX_PIN,\
    .rx_pin                 = USART1_RX_PIN,\
    .usart_tx_source        = GPIO_PinSource9,\
    .usart_rx_source        = GPIO_PinSource10,\
    .preemption_priority    = 5,\
    .sub_priority           = 0,\
    .baudrate               = 115200,\
    .usart_mode             = USART_Mode_Rx | USART_Mode_Tx,\
    .usart_parity           = USART_Parity_No,\
    .usart_stopbit          = USART_StopBits_1,\
    .usart_wordlength       = USART_WordLength_8b,\
    .hfc                    = USART_HardwareFlowControl_None}

#define USART2_DEFAULT_CONFIG()     (usart_cfg_t){\
    .usart                  = USART2,\
    .tx_port                = USART2_TX_PORT,\
    .rx_port                = USART2_RX_PORT,\
    .tx_pin                 = USART2_TX_PIN,\
    .rx_pin                 = USART2_RX_PIN,\
    .usart_tx_source        = GPIO_PinSource2,\
    .usart_rx_source        = GPIO_PinSource3,\
    .preemption_priority    = 5,\
    .sub_priority           = 0,\
    .baudrate               = 115200,\
    .usart_mode             = USART_Mode_Rx | USART_Mode_Tx,\
    .usart_parity           = USART_Parity_No,\
    .usart_stopbit          = USART_StopBits_1,\
    .usart_wordlength       = USART_WordLength_8b,\
    .hfc                    = USART_HardwareFlowControl_None}
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
    uint8_t  rx_buf[USART_RECEIVE_BUF_SIZE];
    uint16_t rx_len;
    uint16_t buf_index;
    uint8_t  new_msg_flag;
    usart_rx_callback_t rx_callback;
    USART_TypeDef*  usart;
};

/**
 * @brief USART1 句柄指针
 * @note NULL 未定义
 */
extern usart_handle_t*     usart1_handle;
/**
 * @brief USART2 句柄指针
 * @note NULL 未定义
 */
extern usart_handle_t*     usart2_handle;

void bsp_usart_init(usart_handle_t* handle, usart_cfg_t cfg);

void bsp_usart_send_byte(usart_handle_t* handle, uint8_t byte);

void bsp_usart_send_string(usart_handle_t* handle, char* str, uint16_t len);



#endif