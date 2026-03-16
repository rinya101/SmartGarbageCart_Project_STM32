/*******************************************************************************
 *      文    件: bsp_pin_cfg.h
 *      说    明: 适用于STM32F4(具体为 STM32F411CEU6) 的 引脚配置宏文件
 *      作    者: Rinya101(http://github.com/rinya101) 学号：220802040137
 *      版权说明：本程序代码仅用于 2026 本科毕业设计，不得用于其他商业用途，
 *               可以作为个人参考学习使用。
*******************************************************************************/
#ifndef _BSP_PIN_CFG_H
#define _BSP_PIN_CFG_H

#include "stm32f4xx.h"

/*
********************************************************************************
*                          智能垃圾拾取车 STM32 引脚分配表
********************************************************************************
* 工程名称：SmartGarbageCart_Project_STM32
* 功能    ：所有硬件引脚统一宏定义管理
* 注意    ：所有模块引脚仅在此处修改，实现硬件与软件解耦
********************************************************************************

PA 组
    PA0     --> 预留
    PA1     --> 预留
    PA2     --> 预留
    PA3     --> 预留
    PA4     --> 预留
    PA5     --> 预留
    PA6     --> 预留
    PA7     --> 预留
    PA8     --> 预留
    PA9     --> 预留
    PA10    --> USART1_RX
    PA11    --> 预留
    PA12    --> 预留
    PA13    --> 预留
    PA14    --> 预留
    PA15    --> USART1_TX

PB 组
    PB0     --> 预留
    PB1     --> 预留
    PB2     --> 预留
    PB3     --> 预留
    PB4     --> 预留
    PB5     --> 预留
    PB6     --> 预留
    PB7     --> 预留
    PB8     --> 预留
    PB9     --> 预留
    PB10    --> 预留
    PB11    --> 预留
    PB12    --> 预留
    PB13    --> 预留
    PB14    --> 预留
    PB15    --> 预留

PC 组
    PC13    --> 预留
    PC14    --> 预留
    PC15    --> 预留

********************************************************************************
*/

/* --------------------------- USART1 引脚 --------------------------- */
/**
 * @note 千万注意配置 对应的 Source AF DMA Stream 
 * 
 */
#define USART1_TX_PORT                  GPIOA
#define USART1_TX_PIN                   GPIO_Pin_9

#define USART1_RX_PORT                  GPIOA
#define USART1_RX_PIN                   GPIO_Pin_10
/* --------------------------- USART2 引脚 --------------------------- */
/**
 * @note 千万注意配置 对应的 Source AF DMA Stream 
 * 
 */
#define USART2_TX_PORT                  GPIOA
#define USART2_TX_PIN                   GPIO_Pin_2

#define USART2_RX_PORT                  GPIOA
#define USART2_RX_PIN                   GPIO_Pin_3
#endif

/* --------------------------- ENCODER(编码器) 引脚 --------------------------- */
#define ENCODER_BTN_PORT                GPIOB
#define ENCODER_BTN_PIN                 GPIO_Pin_14

#define ENCODER_LEFT_PORT               GPIOB
#define ENCODER_LEFT_PIN                GPIO_Pin_12

#define ENCODER_RIGHT_PORT              GPIOB
#define ENCODER_RIGHT_PIN               GPIO_Pin_13