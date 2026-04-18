/*******************************************************************************
 *      文    件: bsp_ultrasonic.h
 *      说    明: 适用于STM32F4(具体为 STM32F411CEU6) 的 超声波 模块
 *      版    本: V1.0
 *      作    者: Rinya101(http://github.com/rinya101) 学号：220802040137
 *      版权说明：本程序代码仅用于 2026 本科毕业设计，不得用于其他商业用途，
 *               可以作为个人参考学习使用。
 * 私有说明：
 *       1. 有三个超声波模块可以使用，必须注意，每一个模块都采用第一个配置的定时器
 *          也就是说三个超声波模块必须使用同一个定时器，否则会出错
 *       2. 必须从第一个开始使用配置，不能跳过第一个的配置，否则会出错
 *       3，提供一个 __weak void ult_data_updata_event(void* data[]) 函数
 *           用于处理超声波数据，需要在 APP 层重写该函数，
 *          void* data[] 通过 ult_handle_t 数据结构进行解耦使用
 * 例如：
 *         ult_handle_t* ult_frist     = (ult_handle_t*)data[0];
 *        此函数为中断内部函数，禁止采用阻塞方式，否则会卡死
*******************************************************************************/
#ifndef _BSP_ULTRASONIC_H
#define _BSP_ULTRASONIC_H
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"

typedef struct ult_handle ult_handle_t;

/**
 * @brief 超声波状态枚举
 * 
 */
typedef enum 
{
    ULT_BUSY,
    ULT_READY,
} ult_sta_t;

/**
 * @brief 超声波模块初始化
 * 
 */
typedef struct 
{
    /* RCC */
    uint32_t            tr_gpio_rcc;
    uint32_t            eh_gpio_rcc;
    uint32_t            tim_rcc;
    /* GPIO */
    GPIO_TypeDef*       tr_gpio_port;
    GPIO_TypeDef*       eh_gpio_port;
    uint32_t            tr_gpio_pin;
    uint32_t            eh_gpio_pin;
    GPIOMode_TypeDef    tr_gpio_mode;
    GPIOMode_TypeDef    eh_gpio_mode;
    GPIOOType_TypeDef   tr_gpio_otype;
    GPIOOType_TypeDef   eh_gpio_otype;
    GPIOSpeed_TypeDef   tr_gpio_speed;  
    GPIOSpeed_TypeDef   eh_gpio_speed;
    GPIOPuPd_TypeDef    tr_gpio_pupd;
    GPIOPuPd_TypeDef    eh_gpio_pupd;
    uint8_t             tr_port_source;
    uint8_t             eh_port_source;
    uint8_t             tr_pin_source;
    uint8_t             eh_pin_source;
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
    uint8_t  state;
    ult_dev_handle_t dev_handle;
};

void bsp_ultrasonic_init(ult_handle_t* handle, ult_cfg_t* cfg);
uint16_t bsp_ultrasonic_get_distance(ult_handle_t* handle);
void bsp_ultrasonic_trigger(ult_handle_t *handle);
#endif