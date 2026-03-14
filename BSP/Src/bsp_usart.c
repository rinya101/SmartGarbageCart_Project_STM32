/*******************************************************************************
 *      文    件: bsp_usart.c
 *      说    明: 适用于STM32F4(具体为 STM32F411CEU6) 的 USART BSP驱动
 *      版    本: V1.0
 *      作    者: Rinya101(http://github.com/rinya101) 学号：220802040137
 *      版权说明：本程序代码仅用于 2026 本科毕业设计，不得用于其他商业用途，
 *               可以作为个人参考学习使用。
*******************************************************************************/
#include "bsp_usart.h"
#include "FreeRTOS.h"
#ifndef INC_FREERTOS_H
#include <stdlib.h>
#endif

usart_handle_t*     usart1_handle = NULL;
usart_handle_t*     usart2_handle = NULL;

/**
 * @brief USART1 中断处理函数
 */
void USART1_IRQHandler(void)
{

    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        uint8_t ch = USART_ReceiveData(USART1);
        if (ch == USART_STRING_END_CR || ch == USART_STRING_END_LF)
        {
            if (usart1_handle->buf_index < USART_RECEIVE_BUF_SIZE)
            {
                usart1_handle->rx_buf[usart1_handle->buf_index] = USART_STRING_END_NULL;
            }
            usart1_handle->new_msg_flag = 1;
            usart1_handle->rx_len = usart1_handle->buf_index;
            usart1_handle->buf_index = 0;
            if (usart1_handle->rx_callback != NULL)
            {
                usart1_handle->rx_callback(usart1_handle);
            }
        }
        else
        {
            if (usart1_handle->buf_index < USART_RECEIVE_BUF_SIZE - 1)
            {
                usart1_handle->rx_buf[usart1_handle->buf_index++] = ch;
            }
        }
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}

/**
 * @brief USART2 中断处理函数
 */
void USART2_IRQHandler(void)
{
    if (usart2_handle == NULL) return;

    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        uint8_t ch = USART_ReceiveData(USART2);
        if (ch == USART_STRING_END_CR || ch == USART_STRING_END_LF)
        {
            if (usart2_handle->buf_index < USART_RECEIVE_BUF_SIZE)
            {
                usart2_handle->rx_buf[usart2_handle->buf_index] = USART_STRING_END_NULL;
            }
            usart2_handle->new_msg_flag = 1;
            usart2_handle->rx_len = usart2_handle->buf_index;
            usart2_handle->buf_index = 0;
            if (usart2_handle->rx_callback != NULL)
            {
                usart2_handle->rx_callback(usart2_handle);
            }
        }
        else
        {
            if (usart2_handle->buf_index < USART_RECEIVE_BUF_SIZE - 1)
            {
                usart2_handle->rx_buf[usart2_handle->buf_index++] = ch;
            }
        }
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}

/**
 * @brief 串口初始化
 * 
 * @param handle    串口句柄
 * @param cfg       串口配置
 */
void bsp_usart_init(usart_handle_t* handle, usart_cfg_t cfg)
{
    /* 数据校验 */
    if (NULL == handle)
    {
        return;
    }
    /* 时钟使能 */
    if (cfg.usart == USART1)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    }
    else if (cfg.usart == USART2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    }
    else if (cfg.usart == USART3)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    }
    /* GPIO 时钟使能 */
    if (cfg.tx_port == GPIOA || cfg.rx_port == GPIOA)
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    }
    if (cfg.tx_port == GPIOB || cfg.rx_port == GPIOB)
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    }
    /* GPIO 复用配置 */
    if (cfg.usart == USART1)
    {
        GPIO_PinAFConfig(cfg.tx_port, cfg.usart_tx_source, GPIO_AF_USART1);
        GPIO_PinAFConfig(cfg.rx_port, cfg.usart_rx_source, GPIO_AF_USART1);
    }
    else if (cfg.usart == USART2)
    {
        GPIO_PinAFConfig(cfg.tx_port, cfg.usart_tx_source, GPIO_AF_USART2);
        GPIO_PinAFConfig(cfg.rx_port, cfg.usart_rx_source, GPIO_AF_USART2);
    }
    else if (cfg.usart == USART3)
    {
        GPIO_PinAFConfig(cfg.tx_port, cfg.usart_tx_source, GPIO_AF_USART3);
        GPIO_PinAFConfig(cfg.rx_port, cfg.usart_rx_source, GPIO_AF_USART3);
    }
    /* TX GPIO 初始化 */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin     = cfg.tx_pin;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_Init(cfg.tx_port, &GPIO_InitStructure);
    /* RX GPIO 初始化 */
    GPIO_InitStructure.GPIO_Pin     = cfg.rx_pin;
    GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
    GPIO_Init(cfg.rx_port, &GPIO_InitStructure);
    /* 串口初始化 */
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate              = cfg.baudrate;
    USART_InitStructure.USART_Mode                  = cfg.usart_mode;
    USART_InitStructure.USART_Parity                = cfg.usart_parity;
    USART_InitStructure.USART_StopBits              = cfg.usart_stopbit;
    USART_InitStructure.USART_WordLength            = cfg.usart_wordlength;
    USART_InitStructure.USART_HardwareFlowControl   = cfg.hfc;
    USART_Init(cfg.usart, &USART_InitStructure);
    /* NVIC 中断配置 */
    NVIC_InitTypeDef NVIC_InitStruct;
    if (cfg.usart == USART1)
    {
        NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    }
    else if (cfg.usart == USART2)
    {
        NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
    }
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = cfg.preemption_priority;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = cfg.sub_priority;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    /* 接收中断使能 */
    USART_ITConfig(cfg.usart, USART_IT_RXNE, ENABLE);
    /* 使能串口 */
    USART_Cmd(cfg.usart, ENABLE);
    /* USART 更新 */
    handle->usart = cfg.usart;
}

/**
 * @brief 发送一个总结
 * 
 * @param handle    串口句柄
 * @param byte      字节
 */
void bsp_usart_send_byte(usart_handle_t* handle, uint8_t byte)
{
    if (NULL == handle) return;

    USART_SendData(handle->usart, byte);
    while (USART_GetFlagStatus(handle->usart, USART_FLAG_TXE) == RESET);
}

/**
 * @brief 发送字符串
 * 
 * @param handle    串口句柄
 * @param str       字符串
 * @param len       字符串长度
 */
void bsp_usart_send_string(usart_handle_t* handle, char* str, uint16_t len)
{
    while (*str != '\0' && len--)
    {
        bsp_usart_send_byte(handle, *str);
        str++;
    }
}
/* 不使用 MicroLib */
#pragma import(__use_no_semihosting)

void _sys_exit(int x)
{
    x = x;
}

/**
 * @brief printf 重定向
 */
int fputc(int ch, FILE *f)
{
    if (usart1_handle != NULL)
        bsp_usart_send_byte(usart1_handle, (uint8_t)ch);
    return ch;
}