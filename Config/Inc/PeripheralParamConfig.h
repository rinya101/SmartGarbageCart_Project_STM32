/*******************************************************************************
 *      文    件: PeripheralParamConfig.h
 *      说    明: 适用于STM32F4(具体为 STM32F411CEU6) 的外设参数配置宏文件
 *      版    本: V1.0
 *      作    者: Rinya101(http://github.com/rinya101) 学号：220802040137
 *      版权说明：本程序代码仅用于 2026 本科毕业设计，不得用于其他商业用途，
 *               可以作为个人参考学习使用。
*******************************************************************************/
#ifndef _PERIPHERAL_PARAM_CONFIG_H
#define _PERIPHERAL_PARAM_CONFIG_H
#include "stm32f4xx.h"
/* ---------------------------------- DEBUG 配置 ----------------------------- */
#define DEBUG   1
#define DETAIL  0
/* ---------------------------------- USART 配置 ----------------------------- */
#define USART_RECEIVE_BUF_SIZE          64         /* 缓存区大小 */
#define USART_STRING_END_CR             '\r'        /* 字符串回车符 */
#define USART_STRING_END_LF             '\n'        /* 字符串换行符 */
#define USART_STRING_END_NULL           '\0'        /* 字符串结束符 */
/**
 * @brief USART 默认初始化配置 
 * @note 修改 usart_tx_source 与 usart_rx_source 对应接口 任务交给程序员自己配置
 * @note 注意 DMA 通道 与 USART 通道的对应关系
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
    .hfc                    = USART_HardwareFlowControl_None,\
    .tx_dma_stream          = DMA2_Stream7,\
    .tx_dma_channel         = DMA_Channel_4,\
    .tx_dma_irq             = DMA2_Stream7_IRQn,\
    .dma_preemption_priority= 5,\
    .dma_sub_priority       = 0}

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
    .hfc                    = USART_HardwareFlowControl_None,\
    .tx_dma_stream          = DMA1_Stream6,\
    .tx_dma_channel         = DMA_Channel_4,\
    .tx_dma_irq             = DMA1_Stream6_IRQn,\
    .dma_preemption_priority= 5,\
    .dma_sub_priority       = 0}

/* ---------------------------------- 编码器 配置 ----------------------------- */
#define ENCODER_BTN_IRQ_HANDLE(void)    EXTI15_10_IRQHandler(void)
#define ENCODER_LEFT_IRQ_HANDLE(void)   EXTI15_10_IRQHandler(void)
/**
 * @note 根据实际情况修改 中断服务函数参考 bsp_encoder_it.c 中的函数：
 * 
 */
#define ENCODER_DEFAULT_CONFIG()    (const encoder_cfg_t){\
    .btn_port           = ENCODER_BTN_PORT,\
    .btn_pin            = ENCODER_BTN_PIN,\
    .left_port          = ENCODER_LEFT_PORT,\
    .left_pin           = ENCODER_LEFT_PIN,\
    .right_port         = ENCODER_RIGHT_PORT,\
    .right_pin          = ENCODER_RIGHT_PIN,\
    .btn_exti_line      = EXTI_Line14,\
    .btn_trigger        = EXTI_Trigger_Rising_Falling,\
    .left_exti_line     = EXTI_Line12,\
    .left_trigger       = EXTI_Trigger_Falling,\
    .btn_irq_channel    = EXTI15_10_IRQn,\
    .left_irq_channel   = EXTI15_10_IRQn,\
    .preemption_priority= 5,\
    .sub_priority       = 0}
/* ---------------------------------- TIM5 配置 ----------------------------- */
#define TIM5_DEFAULT_CONFIG()    (const tim5_cfg_t){\
    .tim5           = TIM5,\
    .prescaler      = (uint16_t)(1000 - 1),    /* 100MHz / 1000 = 100kHz */\
    .period         = (uint16_t)(50000 - 1),   /* 默认 500ms */\
    .counter_mode   = TIM_CounterMode_Up,\
    .division       = TIM_CKD_DIV1,\
    .irq_channel    = TIM5_IRQn,\
    .irq_prepriority= 5,\
    .irq_subpriority= 0}

/**
 * @brief 定时器5使用 ID
 * @note 根据需要增加
 */
#define TIM5_USED_ENCODER_ID         0
/* ---------------------------------- OLED 配置 ----------------------------- */
extern const uint8_t oled_init_cmd_seq[];
#define OLED_INIT_CMD_SEQ_SIZE      32
/* 硬件寄存器配置 */
#define OLED_ADDR       0x78 /* I2C 地址 */
#define OLED_WIDTH      128  /* OLED 宽度 */
#define OLED_HEIGHT     64   /* OLED 高度 */
#define OLED_PAGE_SIZE  8    /* 每页显示 8 行 */
#define OLED_BUF_SIZE   (OLED_WIDTH * OLED_PAGE_SIZE)
#define OLED_BUF_BIT_SIZE (OLED_WIDTH * OLED_HEIGHT)
#define OLED_DMA_IRQ_HANDLE DMA1_Channel7_IRQHandler
/* 软件寄存器配置 */
#define OLED_DATA_MODE   0x40 /* 数据模式 */
#define OLED_CMD_MODE    0x00 /* 命令模式 */
#define OLED_PAGE(page)        (0xB0 + (page))                  /* 设置页地址 */
#define OLED_COL_LOW(col)      (0x00 | ((col) & 0x0F))          /* 设置列地址低4位 */
#define OLED_COL_HIGH(col)     (0x10 | (((col) >> 4) & 0x0F))   /* 设置列地址高4位 */
#define OLED_DEFAULT_CONFIG()    (oled_cfg_t){\
    /* GPIO 配置 */\
    .scl_clk            = RCC_AHB1Periph_GPIOB,\
    .sda_clk            = RCC_AHB1Periph_GPIOB,\
    .scl_port           = GPIOB,\
    .sda_port           = GPIOB,\
    .scl_pin            = GPIO_Pin_6,\
    .sda_pin            = GPIO_Pin_7,\
    .scl_pu             = GPIO_PuPd_UP,\
    .sda_pu             = GPIO_PuPd_UP,\
    .scl_speed          = GPIO_Speed_100MHz,\
    .sda_speed          = GPIO_Speed_100MHz,\
    .scl_mode           = GPIO_Mode_AF,\
    .sda_mode           = GPIO_Mode_AF,\
    .scl_otype          = GPIO_OType_OD,\
    .sda_otype          = GPIO_OType_OD,\
    .scl_source         = GPIO_PinSource6,\
    .sda_source         = GPIO_PinSource7,\
    .scl_af             = GPIO_AF_I2C1,\
    .sda_af             = GPIO_AF_I2C1,\
    /* I2C 配置 */\
    .i2c_clk            = RCC_APB1Periph_I2C1,\
    .i2cx               = I2C1,\
    .i2c_mode           = I2C_Mode_I2C,\
    .i2c_ack            = I2C_Ack_Enable,\
    .i2c_ack_addr       = I2C_AcknowledgedAddress_7bit,\
    .i2c_duty           = I2C_DutyCycle_2,\
    .i2c_speed          = 400000,\
    .i2c_own_addr       = 0x00,\
    /* DMA 配置 */\
    .dma_clk            = RCC_AHB1Periph_DMA1,\
    .dma_stream         = DMA1_Stream7,\
    .tx_dma_channel     = DMA_Channel_1,\
    .dma_priority       = DMA_Priority_High,\
    .dma_irq_channel    = DMA1_Stream7_IRQn,\
    .dma_irq_prio       = 5,\
    .dma_irq_sub_prio   = 0,\
    /* SYSCFG 配置 */\
    .syscfg_clk         = RCC_APB2Periph_SYSCFG,\
    /* OLED 配置 */\
    .i2c_addr           = OLED_ADDR,\
    .width              = OLED_WIDTH,\
    .height             = OLED_HEIGHT\
}

#endif  /* PeripheralParamConfig.h */