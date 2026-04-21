/*******************************************************************************
 *      文    件: bsp_battery.h
 *      说    明: 适用于STM32F4(具体为 STM32F411CEU6) 的 电池电压检测驱动
 *      版    本: V1.0
 *      作    者: Rinya101(http://github.com/rinya101) 学号：220802040137
 *      版权说明：本程序代码仅用于 2026 本科毕业设计，不得用于其他商业用途，
 *               可以作为个人参考学习使用。
*******************************************************************************/
#ifndef _BSP_BATTERY_H_
#define _BSP_BATTERY_H_

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_adc.h"

typedef struct battery_handle battery_handle_t;

/**
 * @brief 电池配置
 */
typedef struct
{
    /* RCC 时钟 */
    uint32_t            gpio_clk;
    uint32_t            adc_clk;

    /* GPIO */
    GPIO_TypeDef        *gpio_port;
    uint32_t             gpio_pin;

    /* ADC */
    ADC_TypeDef          *adc;
    uint8_t              adc_channel;
    /* OFFSET */
    float                offset;
} battery_cfg_t;

/**
 * @brief 电池句柄
 */
struct battery_handle
{
    uint16_t  adc_value;        /* ADC原始值 */
    float     voltage;          /* 实际电压 */
    uint8_t   level;            /* 电量百分比 */

    struct
    {
        GPIO_TypeDef*  gpio_port;
        uint32_t       gpio_pin;
        ADC_TypeDef*   adc;
        uint8_t        adc_channel;
        float          offset;
    } dev;                      /* 设备，用户不需要访问修改 */

    struct
    {
        void (*battery_update)(const void *param);
    } event_callback;           /* 事件回调 */
};


void bsp_battery_init(battery_handle_t *handle, const battery_cfg_t *cfg);
float bsp_battery_get_voltage(battery_handle_t *handle);
uint8_t bsp_battery_get_level(battery_handle_t *handle);

#endif