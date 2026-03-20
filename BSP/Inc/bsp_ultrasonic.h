#ifndef _BSP_ULTRASONIC_H
#define _BSP_ULTRASONIC_H
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"

typedef struct ult_handle ult_handle_t;
/**
 * @brief 超声波模块初始化
 * 
 */
typedef struct 
{
    /* NUM */
    uint8_t             num;
    /* RCC */
    uint32_t            tr_gpio_rcc;
    uint32_t            eh_gpio_rcc;
    uint32_t            tim_rcc;
    /* GPIO */
    GPIO_TypeDef*       tr_gpio_port;
    GPIO_TypeDef*       eh_gpio_port;
    uint16_t            tr_gpio_pin;
    uint16_t            eh_gpio_pin;
    GPIOMode_TypeDef    tr_gpio_mode;
    GPIOMode_TypeDef    eh_gpio_mode;
    GPIOOType_TypeDef   tr_gpio_otype;
    GPIOOType_TypeDef   eh_gpio_otype;
    GPIOSpeed_TypeDef   tr_gpio_speed;  
    GPIOSpeed_TypeDef   eh_gpio_speed;
    GPIOPuPd_TypeDef    tr_gpio_pupd;
    GPIOPuPd_TypeDef    eh_gpio_pupd;
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
    uint8_t  num;
    ult_dev_handle_t dev_handle;
};

void bsp_ultrasonic_init(ult_handle_t* handle, ult_cfg_t* cfg);
uint16_t bsp_ultrasonic_get_distance(ult_handle_t* handle);
void bsp_ultrasonic_trigger(ult_handle_t *handle);
#endif