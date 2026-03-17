#ifndef _BSP_OLED_H
#define _BSP_OLED_H
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_dma.h"
#include "PeripheralParamConfig.h"
#include "bsp_debug.h"
typedef struct oled_handle oled_handle_t;

/**
 * @brief OLED 句柄
 * 
 */
extern oled_handle_t* g_oled_handle;
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
    I2C_TypeDef*        i2cx;          
    uint8_t             width;
    uint8_t             height;
    uint8_t             screen_buf[OLED_BUF_BIT_SIZE];  /* 全屏显示缓存 */
    uint16_t            buf_size;
    uint8_t             oled_addr;
};

void bsp_oled_init(oled_handle_t* oled_handle, oled_cfg_t cfg);
void bsp_oled_refresh(oled_handle_t* oled_handle);
#endif /* _BSP_OLED_H */