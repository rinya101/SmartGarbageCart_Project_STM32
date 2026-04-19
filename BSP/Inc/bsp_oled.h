/*******************************************************************************
 *      文    件: bsp_oled.h
 *      说    明: 适用于STM32F4(具体为 STM32F411CEU6) 的 OLED 驱动
 *      版    本: V1.0
 *      作    者: Rinya101(http://github.com/rinya101) 学号：220802040137
 *      版权说明：本程序代码仅用于 2026 本科毕业设计，不得用于其他商业用途，
 *               可以作为个人参考学习使用。
 * 私有说明：
 *      1. 截至目前（2026.3.18）还未完成 DMA 屏幕刷新功能，DMA 可列为暂不可用状态
 *      2. 在文件 bsp_oled.c 中已经定义 OLED 显示缓存区，可直接使用，
 *         不需要额外配置
 *      3. 库函数使用说明/顺序：
 *          [1] void bsp_oled_init(oled_handle_t* oled_handle, oled_cfg_t* cfg)
 *              对于 oled_handle 需要进行声明使用，例如：
 *                  oled_handle_t oled_handle;
 *              对于 cfg 【优先推荐】使用文件 PerpheraParamsConfig.h 中定义的
 *                   OLED_DEFAULT_CONFIG()，例如：
 *                  oled_cfg_t oled_cfg = OLED_DEFAULT_CONFIG();
 *               PerpheraParamsConfig.h 根据需求更改
 *          [2] void bsp_oled_refresh(oled_handle_t* oled_handle)
 *               刷新屏幕，将缓存区内容写入屏幕
 *      4. 库函数这里仅仅提供 DrowPoint 函数：
 *          void bsp_oled_draw_point(oled_handle_t* oled_handle, 
 *                                      uint8_t x, 
 *                                      uint8_t y, 
 *                                      uint8_t data);
 *          剩余函数可自行开发。
*******************************************************************************/
#ifndef _BSP_OLED_H
#define _BSP_OLED_H
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_dma.h"
#include "PeripheralParamConfig.h"
#include "bsp_debug.h"
typedef struct oled_handle oled_handle_t;
/**
 * @brief OLED 配置结构体
 * 
 */
typedef struct 
{
    /* GPIO */
    uint32_t            scl_clk;
    uint32_t            sda_clk;
    GPIO_TypeDef*       scl_port;
    GPIO_TypeDef*       sda_port;
    uint32_t            scl_pin;
    uint32_t            sda_pin;
    GPIOPuPd_TypeDef    scl_pu;
    GPIOPuPd_TypeDef    sda_pu;
    GPIOSpeed_TypeDef   scl_speed;
    GPIOSpeed_TypeDef   sda_speed;
    GPIOMode_TypeDef    scl_mode;
    GPIOMode_TypeDef    sda_mode;
    GPIOOType_TypeDef   scl_otype;
    GPIOOType_TypeDef   sda_otype;
    uint16_t            scl_source;
    uint16_t            sda_source;
    uint8_t             scl_af;
    uint8_t             sda_af;
    /* I2Cx */
    uint32_t        i2c_clk;
    I2C_TypeDef*    i2cx;
    uint16_t        i2c_mode;
    uint16_t        i2c_ack;
    uint16_t        i2c_ack_addr;
    uint16_t        i2c_duty;
    uint32_t        i2c_speed;
    uint16_t        i2c_own_addr;   /* 本机地址 */
    /* DMA */
    uint32_t                dma_clk;
    DMA_Stream_TypeDef*     dma_stream;
    uint32_t                tx_dma_channel;
    uint32_t                dma_priority;
    uint8_t                 dma_irq_channel;
    uint8_t                 dma_irq_prio;
    uint8_t                 dma_irq_sub_prio;
    /* SYSCFG (部分私有) */
    uint32_t                syscfg_clk;
    /* OLED */
    uint8_t                 i2c_addr;  /* 从机地址 */
    uint16_t                width;
    uint16_t                height;
} oled_cfg_t;
/**
 * @brief OLED 句柄结构体
 * @note 包含 I2C、DMA、全屏缓存、显示参数
 */
struct oled_handle
{
    /* I2C */
    I2C_TypeDef*        i2cx;
    /* OLED */
    uint8_t             width;
    uint8_t             height;
    uint8_t*            screen_buf;  /* 全屏显示缓存 */
    uint8_t             oled_addr;
    /* DMA */
    DMA_Stream_TypeDef*     dma_stream;
};


void bsp_oled_init(oled_handle_t* oled_handle, oled_cfg_t* cfg);
void bsp_oled_draw_point(oled_handle_t* oled_handle, uint8_t x, uint8_t y, uint8_t data);
void bsp_oled_refresh(oled_handle_t* oled_handle);

void bsp_oled_welcome(oled_handle_t* oled_handle);
#endif /* _BSP_OLED_H */