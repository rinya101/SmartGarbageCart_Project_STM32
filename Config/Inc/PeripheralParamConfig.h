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
//#define USART1_IRQ_HANDLE(void)         USART1_IRQHandler(void)
//#define USART2_IRQ_HANDLE(void)         USART2_IRQHandler(void)
/**
 * @brief USART 默认初始化配置 
 * @note 修改 usart_tx_source 与 usart_rx_source 对应接口 任务交给程序员自己配置
 * @note 注意 DMA 通道 与 USART 通道的对应关系
 */
#define USART1_DEFAULT_CONFIG()     (message_cfg_t){\
    /* RCC  */\
    .usart_rcc              = RCC_APB2Periph_USART1,\
    .tx_pin_rcc             = RCC_AHB1Periph_GPIOA,\
    .rx_pin_rcc             = RCC_AHB1Periph_GPIOA,\
    /* GPIO */\
    .tx_port                = USART1_TX_PORT,\
    .rx_port                = USART1_RX_PORT,\
    .tx_pin                 = USART1_TX_PIN,\
    .rx_pin                 = USART1_RX_PIN,\
    .tx_mode                = GPIO_Mode_AF,\
    .rx_mode                = GPIO_Mode_AF,\
    .tx_otype               = GPIO_OType_OD,\
    .rx_otype               = GPIO_OType_OD,\
    .tx_pu                  = GPIO_PuPd_UP,\
    .rx_pu                  = GPIO_PuPd_UP,\
    .tx_speed               = GPIO_Speed_50MHz,\
    .rx_speed               = GPIO_Speed_50MHz,\
    .usart_tx_source        = GPIO_PinSource9,\
    .usart_rx_source        = GPIO_PinSource10,\
    .usart_af               = GPIO_AF_USART1,\
    /* NVIC */\
    .irq_channel            = USART1_IRQn,\
    .preemption_priority    = 5,\
    .sub_priority           = 0,\
    /* USART */\
    .usart                  = USART1,\
    .baudrate               = 115200,\
    .usart_mode             = USART_Mode_Rx | USART_Mode_Tx,\
    .usart_parity           = USART_Parity_No,\
    .usart_stopbit          = USART_StopBits_1,\
    .usart_wordlength       = USART_WordLength_8b,\
    .hfc                    = USART_HardwareFlowControl_None,\
    /* DMA */\
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
#define ENCODER_BTN_IRQ_HANDLE(void)    EXTI2_IRQHandler(void)
#define ENCODER_LEFT_IRQ_HANDLE(void)   EXTI0_IRQHandler(void)
#define ENCODER_TIM_IRQ_HANDLE(void)    TIM5_IRQHandler(void)
/**
 * @note 根据实际情况修改 中断服务函数参考 bsp_encoder_it.c 中的函数：
 * 
 */
#define ENCODER_DEFAULT_CONFIG()    (encoder_cfg_t){\
    /* RCC */\
    .btn_clk            = RCC_AHB1Periph_GPIOB,\
    .left_clk           = RCC_AHB1Periph_GPIOB,\
    .right_clk          = RCC_AHB1Periph_GPIOB,\
    .syscfg_clk         = RCC_APB2Periph_SYSCFG,\
    .tim_clk            = RCC_APB1Periph_TIM5,\
    /* GPIO */\
    .btn_port           = GPIOB,\
    .btn_pin            = GPIO_Pin_2,\
    .left_port          = GPIOB,\
    .left_pin           = GPIO_Pin_0,\
    .right_port         = GPIOB,\
    .right_pin          = GPIO_Pin_1,\
    /* TIM */\
    .timx               = TIM5,\
    .tim_prescaler      = (uint16_t)(1000 - 1),\
    .tim_period         = (uint16_t)(60000 - 1),\
    .tim_counter_mode   = TIM_CounterMode_Up,\
    .tim_division       = TIM_CKD_DIV1,\
    /* EXIT */\
    .btn_exti_line      = EXTI_Line2,\
    .btn_trigger        = EXTI_Trigger_Rising_Falling,\
    .left_exti_line     = EXTI_Line0,\
    .left_trigger       = EXTI_Trigger_Falling,\
    /* NVIC */\
    .btn_irq_channel    = EXTI2_IRQn,\
    .left_irq_channel   = EXTI0_IRQn,\
    .preemption_priority= 5,\
    .sub_priority       = 0,\
    .tim_irq_channel    = TIM5_IRQn,\
    .tim_irq_prepriority= 6,\
    .tim_irq_subpriority= 0,\
    .press_long_time    = 40000 /* 最大 uint16_t */\
}
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
/* ----------------------- 超声波模块（HC-SR04） 配置 ------------------------- */
//#define ULT_FIRST_EH_EXTI_HANDLE(void)  EXTI1_IRQHandler(void)
//#define ULT_TIM_HANDLE(void)            TIM3_IRQHandler(void)
#define ULT_DEFAULT_CONDFIG() (ult_cfg_t){\
    /* RCC */\
    .tr_gpio_rcc = RCC_AHB1Periph_GPIOA,\
    .eh_gpio_rcc = RCC_AHB1Periph_GPIOA,\
    .tim_rcc     = RCC_APB1Periph_TIM3,\
    /* GPIO */\
    .tr_gpio_port   = GPIOA,\
    .eh_gpio_port   = GPIOA,\
    .tr_gpio_pin    = GPIO_Pin_6,\
    .eh_gpio_pin    = GPIO_Pin_7,\
    .tr_gpio_mode   = GPIO_Mode_OUT,\
    .eh_gpio_mode   = GPIO_Mode_IN,\
    .tr_gpio_otype  = GPIO_OType_PP,\
    .eh_gpio_otype  = GPIO_OType_PP,\
    .tr_gpio_speed  = GPIO_Speed_25MHz,\
    .eh_gpio_speed  = GPIO_Speed_50MHz,\
    .tr_gpio_pupd   = GPIO_PuPd_NOPULL,\
    .eh_gpio_pupd   = GPIO_PuPd_UP,\
    .tr_port_source = EXTI_PortSourceGPIOA,\
    .eh_port_source = EXTI_PortSourceGPIOA,\
    .tr_pin_source  = EXTI_PinSource6,\
    .eh_pin_source  = EXTI_PinSource7,\
    /* EXTI */\
    .eh_exti_line   = EXTI_Line7,\
    .eh_exti_mode   = EXTI_Mode_Interrupt,\
    .eh_exti_trigger= EXTI_Trigger_Rising_Falling,\
    /* NVIC */\
    .eh_nvic_irqn   = EXTI9_5_IRQn,\
    .eh_nvic_pri    = 3,\
    .eh_nvic_subpri = 0,\
    .tim_nvic_irqn  = TIM3_IRQn,\
    .tim_nvic_pri   = 4,\
    .tim_nvic_subpri= 0,\
    /* TIM */\
    .timx               = TIM3,\
    .tim_prescaler      = 100 - 1, /* 1MHZ 1us */\
    .tim_period         = 10000 - 1, /* 1us*10000 = 10ms */\
    .tim_counter_mode   = TIM_CounterMode_Up,\
    .tim_division       = TIM_CKD_DIV1,\
}
/* ----------------------- 循迹模块 配置 ------------------------- */
#define TRACKER_A_DEFAULT_CONFIG() (track_cfg_t){\
    /* TRACKER */\
    .id                 = TRACKER_A,\
    /* RCC */\
    .rcc_gpio           = RCC_AHB1Periph_GPIOB,\
    .gpio_port          = GPIOB,\
    .gpio_pin           = GPIO_Pin_3,\
    .gpio_mode          = GPIO_Mode_IN,\
    .gpio_otype         = GPIO_OType_PP,\
    .gpio_speed         = GPIO_Speed_25MHz,\
    .gpio_pupd          = GPIO_PuPd_UP,\
    .gpio_port_source   = EXTI_PortSourceGPIOB,\
    .gpio_pin_source    = EXTI_PinSource3,\
    /* EXTI */\
    .exti_line          = EXTI_Line3,\
    .exti_mode          = EXTI_Mode_Interrupt,\
    .exti_trigger       = EXTI_Trigger_Rising_Falling,\
    /* NVIC */\
    .nvic_irqn          = EXTI3_IRQn,\
    .nvic_pri           = 5,\
    .nvic_subpri        = 4\
}
#define TRACKER_B_DEFAULT_CONFIG() (track_cfg_t){\
    /* TRACKER */\
    .id                 = TRACKER_B,\
    /* RCC */\
    .rcc_gpio           = RCC_AHB1Periph_GPIOB,\
    .gpio_port          = GPIOB,\
    .gpio_pin           = GPIO_Pin_4,\
    .gpio_mode          = GPIO_Mode_IN,\
    .gpio_otype         = GPIO_OType_PP,\
    .gpio_speed         = GPIO_Speed_25MHz,\
    .gpio_pupd          = GPIO_PuPd_UP,\
    .gpio_port_source   = EXTI_PortSourceGPIOB,\
    .gpio_pin_source    = EXTI_PinSource4,\
    /* EXTI */\
    .exti_line          = EXTI_Line4,\
    .exti_mode          = EXTI_Mode_Interrupt,\
    .exti_trigger       = EXTI_Trigger_Rising_Falling,\
    /* NVIC */\
    .nvic_irqn          = EXTI4_IRQn,\
    .nvic_pri           = 6,\
    .nvic_subpri        = 3\
}
#define TRACKER_C_DEFAULT_CONFIG() (track_cfg_t){\
    /* TRACKER */\
    .id                 = TRACKER_C,\
    /* RCC */\
    .rcc_gpio           = RCC_AHB1Periph_GPIOB,\
    .gpio_port          = GPIOB,\
    .gpio_pin           = GPIO_Pin_5,\
    .gpio_mode          = GPIO_Mode_IN,\
    .gpio_otype         = GPIO_OType_PP,\
    .gpio_speed         = GPIO_Speed_25MHz,\
    .gpio_pupd          = GPIO_PuPd_UP,\
    .gpio_port_source   = EXTI_PortSourceGPIOB,\
    .gpio_pin_source    = EXTI_PinSource5,\
    /* EXTI */\
    .exti_line          = EXTI_Line5,\
    .exti_mode          = EXTI_Mode_Interrupt,\
    .exti_trigger       = EXTI_Trigger_Rising_Falling,\
    /* NVIC */\
    .nvic_irqn          = EXTI9_5_IRQn,\
    .nvic_pri           = 7,\
    .nvic_subpri        = 2\
}

/* ----------------------- 指南针模块 配置 ------------------------- */
#define COMPASS_ADDR    0x58
#define COMPASS_DEFAULT_CONFIG() (const compass_cfg_t){\
    /* RCC */\
    .gpio_sda_rcc       = RCC_AHB1Periph_GPIOB,\
    .gpio_scl_rcc       = RCC_AHB1Periph_GPIOA,\
    .i2c_rcc            = RCC_APB1Periph_I2C3,\
    /* GPIO */\
    .gpio_sda_port      = GPIOB,\
    .gpio_scl_port      = GPIOA,\
    .gpio_sda_source    = GPIO_PinSource8,\
    .gpio_scl_source    = GPIO_PinSource8,\
    .gpio_sda_af        = GPIO_AF9_I2C3,\
    .gpio_scl_af        = GPIO_AF_I2C3,\
    .gpio_sda_pin       = GPIO_Pin_8,\
    .gpio_scl_pin       = GPIO_Pin_8,\
    .gpio_sda_mode      = GPIO_Mode_AF,\
    .gpio_scl_mode      = GPIO_Mode_AF,\
    .gpio_sda_otype     = GPIO_OType_OD,\
    .gpio_scl_otype     = GPIO_OType_OD,\
    .gpio_sda_pu        = GPIO_PuPd_UP,\
    .gpio_scl_pu        = GPIO_PuPd_UP,\
    .gpio_sda_speed     = GPIO_Speed_50MHz,\
    .gpio_scl_speed     = GPIO_Speed_50MHz,\
    /* I2C */\
    .i2c                = I2C3,\
    .i2c_mode           = I2C_Mode_I2C,\
    .i2c_duty           = I2C_DutyCycle_2,\
    .i2c_own_addr       = 0x00,\
    .i2c_ack            = I2C_Ack_Enable,\
    .i2c_ack_addr       = I2C_AcknowledgedAddress_7bit,\
    .i2c_speed          = 100000,\
    /* Base */\
    .addr               = COMPASS_ADDR\
}
/* ----------------------- 蜂鸣器 配置 ------------------------- */
#define BUZZER_DEFAULT_CONFIG() (buzzer_cfg_t){\
    .gpio_rcc       = RCC_AHB1Periph_GPIOB,\
    .tim_rcc        = RCC_APB1Periph_TIM4,\
    .gpio_port      = GPIOB,\
    .gpio_pin       = GPIO_Pin_9,\
    .timx           = TIM4,\
    .tim_channel    = TIM_Channel_4,\
    .prescaler      = 10 - 1,        /* 100MHz /10 = 10MHz → 0.1us */\
    .period         = 10000,         /* 1kHz 默认频率 */\
    .duty           = 1000,          /* 温和占空比 */\
}
#endif  /* PeripheralParamConfig.h */