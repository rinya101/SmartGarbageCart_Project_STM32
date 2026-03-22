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

usart_handle_t*     g_usart1_handle = NULL;

/**
 * @brief 主消息处理函数
 * 
 * @param handle 
 */
void main_message_irq_handler(void *param)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        uint8_t ch = USART_ReceiveData(USART1);
        if (ch == USART_STRING_END_CR || ch == USART_STRING_END_LF)
        {
            if (g_usart1_handle->buf_index < USART_RECEIVE_BUF_SIZE)
            {
                g_usart1_handle->rx_buf[g_usart1_handle->buf_index] = USART_STRING_END_NULL;
            }
            g_usart1_handle->new_msg_flag = 1;
            g_usart1_handle->rx_len = g_usart1_handle->buf_index;
            g_usart1_handle->buf_index = 0;
            if (g_usart1_handle->rx_callback != NULL)
            {
                g_usart1_handle->rx_callback(g_usart1_handle);
            }
        }
        else
        {
            if (g_usart1_handle->buf_index < USART_RECEIVE_BUF_SIZE - 1)
            {
                g_usart1_handle->rx_buf[g_usart1_handle->buf_index++] = ch;
            }
        }
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}

/**
 * @brief DMA 中断处理函数
 * 
 */
void DMA_main_message_irq_handler(void* param)
{
    /* DAM 传输完成 */
    if(DMA_GetITStatus(DMA2_Stream7, DMA_IT_TCIF7) != RESET)
    {
        DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);
        /* 关闭 DMA */
        DMA_Cmd(DMA2_Stream7, DISABLE);
    }
}
/**
 * @brief 串口时钟初始化
 * 
 * @param handle 
 * @param cfg 
 */
static void usart_clock_init(usart_handle_t* handle, usart_cfg_t cfg)
{
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
}
/**
 * @brief 串口 GPIO 初始化
 * 
 * @param handle 
 * @param cfg 
 */
static void usart_gpio_init(usart_handle_t* handle, usart_cfg_t cfg)
{
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
}
/**
 * @brief 串口初始化
 * 
 * @param handle 
 * @param cfg 
 */
static void usart_init(usart_handle_t* handle, usart_cfg_t cfg)
{
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate              = cfg.baudrate;
    USART_InitStructure.USART_Mode                  = cfg.usart_mode;
    USART_InitStructure.USART_Parity                = cfg.usart_parity;
    USART_InitStructure.USART_StopBits              = cfg.usart_stopbit;
    USART_InitStructure.USART_WordLength            = cfg.usart_wordlength;
    USART_InitStructure.USART_HardwareFlowControl   = cfg.hfc;
    USART_Init(cfg.usart, &USART_InitStructure);
}
/**
 * @brief 串口使用 DMA 
 * 
 * @param handle 
 * @param cfg 
 */
static void usart_use_dma_init(usart_handle_t* handle, usart_cfg_t cfg)
{
    handle->tx_dma_stream   = cfg.tx_dma_stream;
    if (cfg.usart == USART1) 
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    } else if (cfg.usart == USART2) 
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    } else if (cfg.usart == USART3) 
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    }
    DMA_Cmd(handle->tx_dma_stream, DISABLE);
    while (DMA_GetCmdStatus(handle->tx_dma_stream) == ENABLE);
    if (handle->tx_dma_stream == DMA2_Stream7) 
    {
        DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7 | 
                                    DMA_FLAG_HTIF7 | 
                                    DMA_FLAG_TEIF7 | 
                                    DMA_FLAG_DMEIF7 | 
                                    DMA_FLAG_FEIF7);
    } else if (handle->tx_dma_stream == DMA1_Stream6) 
    {
        DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6 | 
                                    DMA_FLAG_HTIF6 | 
                                    DMA_FLAG_TEIF6 | 
                                    DMA_FLAG_DMEIF6 | 
                                    DMA_FLAG_FEIF6);
    } else if (handle->tx_dma_stream == DMA1_Stream3) 
    {
        DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3 | 
                                    DMA_FLAG_HTIF3 | 
                                    DMA_FLAG_TEIF3 | 
                                    DMA_FLAG_DMEIF3 | 
                                    DMA_FLAG_FEIF3);
    }
    DMA_DeInit(handle->tx_dma_stream);
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_Channel = cfg.tx_dma_channel;                         // 通道4对应USART1/2/3/6
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(handle->usart->DR);  // 外设地址：USART1的数据寄存器
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)(handle->dma_buf);        // 内存地址：我们的全局缓冲区
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                     // 传输方向：内存到外设
    DMA_InitStructure.DMA_BufferSize = handle->dma_buf_size;                    // 传输数据量
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            // 外设地址不增长
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     // 内存地址自动增长
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;     // 外设数据宽度：字节
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;             // 内存数据宽度：字节
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                               // 普通模式，传输一次后停止
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                         // 高优先级
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                      // 禁用FIFO
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;               // FIFO阈值
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;                 // 单次突发
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;         // 单次突发
    DMA_Init(handle->tx_dma_stream, &DMA_InitStructure);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = cfg.tx_dma_irq;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = cfg.dma_preemption_priority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority           = cfg.dma_sub_priority;
    NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    DMA_ITConfig(handle->tx_dma_stream, DMA_IT_TC, ENABLE);
    USART_DMACmd(cfg.usart, USART_DMAReq_Tx, ENABLE);
}
/**
 * @brief 串口中断配置
 * 
 * @param handle 
 * @param cfg 
 */
static void usart_irq_init(usart_handle_t* handle, usart_cfg_t cfg)
{
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
    usart_clock_init(handle, cfg);
    /* GPIO 初始化 */
    usart_gpio_init(handle, cfg);
    /* 串口初始化 */
    usart_init(handle, cfg);
    /* NVIC 中断配置 */
    usart_irq_init(handle, cfg);
    /* 接收中断使能 */
    USART_ITConfig(cfg.usart, USART_IT_RXNE, ENABLE);
    /* 使能串口 */
    USART_Cmd(cfg.usart, ENABLE);
    /* USART 更新 */
    handle->usart = cfg.usart;
    /* 启用 DMA 传输 */
    usart_use_dma_init(handle, cfg);
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

/**
 * @brief DMA 方式发送字符串
 */
void bsp_usart_send_dma(usart_handle_t* handle)
{
    if(handle == NULL || handle->dma_buf == NULL) return;
    while (DMA_GetCmdStatus(handle->tx_dma_stream) == ENABLE);
    if (handle->tx_dma_stream == DMA2_Stream7) 
    {
        DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7 | 
                                    DMA_FLAG_HTIF7 | 
                                    DMA_FLAG_TEIF7 | 
                                    DMA_FLAG_DMEIF7 | 
                                    DMA_FLAG_FEIF7);
    } else if (handle->tx_dma_stream == DMA1_Stream6) 
    {
        DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6 | 
                                    DMA_FLAG_HTIF6 | 
                                    DMA_FLAG_TEIF6 | 
                                    DMA_FLAG_DMEIF6 | 
                                    DMA_FLAG_FEIF6);
    } else if (handle->tx_dma_stream == DMA1_Stream3) 
    {
        DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3 | 
                                    DMA_FLAG_HTIF3 | 
                                    DMA_FLAG_TEIF3 | 
                                    DMA_FLAG_DMEIF3 | 
                                    DMA_FLAG_FEIF3);
    }
    handle->tx_dma_stream->NDTR = handle->dma_buf_size;
    handle->tx_dma_stream->M0AR = (uint32_t)handle->dma_buf;
    DMA_Cmd(handle->tx_dma_stream, ENABLE);
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
    if (g_usart1_handle != NULL)
        bsp_usart_send_byte(g_usart1_handle, (uint8_t)ch);
    return ch;
}