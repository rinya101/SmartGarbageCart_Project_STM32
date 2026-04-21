/*******************************************************************************
 *      文    件: bsp_battery.c
 *      说    明: 适用于STM32F4(具体为 STM32F411CEU6) 的 电池电压检测驱动
 *      版    本: V1.0
 *      作    者: Rinya101(http://github.com/rinya101) 学号：220802040137
 *      版权说明：本程序代码仅用于 2026 本科毕业设计，不得用于其他商业用途，
 *               可以作为个人参考学习使用。
*******************************************************************************/
#include "bsp_battery.h"
#ifndef NULL
#define NULL (void*)0
#endif

/**
 * @brief 电池时钟初始化
 */
static void battery_clk_init(const battery_cfg_t *cfg)
{
    RCC_AHB1PeriphClockCmd(cfg->gpio_clk, ENABLE);
    RCC_APB2PeriphClockCmd(cfg->adc_clk, ENABLE);
}

/**
 * @brief 电池GPIO初始化（模拟输入）
 */
static void battery_gpio_init(const battery_cfg_t *cfg)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = cfg->gpio_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(cfg->gpio_port, &GPIO_InitStructure);
}

/**
 * @brief 电池ADC初始化
 */
static void battery_adc_init(const battery_cfg_t *cfg)
{
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);
    ADC_InitTypeDef ADC_InitStructure;
    ADC_DeInit();  
    ADC_InitStructure.ADC_Resolution            = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode          = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode    = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge  = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign             = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion       = 1;
    ADC_Init(cfg->adc, &ADC_InitStructure);
    ADC_RegularChannelConfig(cfg->adc, cfg->adc_channel, 1, ADC_SampleTime_3Cycles);
    ADC_Cmd(cfg->adc, ENABLE);
}
/**
 * @brief 读取电池电压ADC值
 * 
 * @param handle 
 * @return uint16_t 
 */
static uint16_t battery_adc_read(battery_handle_t *handle)
{
    uint32_t timeout = 100000;

    ADC_RegularChannelConfig(handle->dev.adc, handle->dev.adc_channel, 1, ADC_SampleTime_480Cycles);
    ADC_SoftwareStartConv(handle->dev.adc);

    while (!ADC_GetFlagStatus(handle->dev.adc, ADC_FLAG_EOC) && timeout--);

    uint16_t adc_val = ADC_GetConversionValue(handle->dev.adc) & 0x0FFF;

    ADC_ClearFlag(handle->dev.adc, ADC_FLAG_EOC);

    return adc_val;
}

/**
 * @brief 电池初始化
 * @param handle 电池句柄
 * @param cfg 配置
 */
void bsp_battery_init(battery_handle_t *handle, const battery_cfg_t *cfg)
{
    if (handle == NULL || cfg == NULL) return;

    handle->dev.gpio_port   = cfg->gpio_port;
    handle->dev.gpio_pin    = cfg->gpio_pin;
    handle->dev.adc         = cfg->adc;
    handle->dev.adc_channel = cfg->adc_channel;
    handle->dev.offset      = cfg->offset;

    handle->adc_value = 0;
    handle->voltage   = 0.0f;
    handle->level     = 0;

    battery_clk_init(cfg);
    battery_gpio_init(cfg);
    battery_adc_init(cfg);
}
/**
 * @brief 获取电池电压
 * 
 * @param handle 
 * @return float 
 */
float bsp_battery_get_voltage(battery_handle_t *handle)
{
    uint16_t adc_val = battery_adc_read(handle);
    handle->adc_value = adc_val;
    handle->voltage = (adc_val / 4095.0f) * 5.0f * handle->dev.offset;
    return handle->voltage;
}

/**
 * @brief 获取电池电量 0~100%
 */
uint8_t bsp_battery_get_level(battery_handle_t *handle)
{
    bsp_battery_get_voltage(handle);

    handle->level = (((float)(handle->voltage) - 4.4f) / (5.0f - 4.4f)) * 100;

    return handle->level;
}