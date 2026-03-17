#include "bsp_oled.h"

/**
 * @brief OLED 句柄
 * 
 */
oled_handle_t* g_oled_handle = NULL;
/**
 * @brief OLED      时钟配置
 * @note  GPIO      时钟
 *        I2C       时钟
 *        DMA       时钟
 *        SYSCFG    时钟
 * @param oled_handle 
 * @param cfg 
 */
static void bsp_oled_clk_cfg(oled_handle_t* oled_handle, oled_cfg_t cfg)
{
    /* GPIO 时钟 */
    RCC_AHB1PeriphClockCmd(cfg.scl_clk, ENABLE);
    RCC_AHB1PeriphClockCmd(cfg.sda_clk, ENABLE);
    /* I2Cx 时钟 */
    RCC_APB1PeriphClockCmd(cfg.i2c_clk, ENABLE);
    /* DAM 时钟 */
    RCC_AHB1PeriphClockCmd(cfg.dma_clk, ENABLE);
    /* SYSCFG 时钟 */
    RCC_APB2PeriphClockCmd(cfg.syscfg_clk, ENABLE);
}
/**
 * @brief OLED GPIO 配置
 * 
 * @param oled_handle 
 * @param cfg 
 */
static void bsp_oled_gpio_cfg(oled_handle_t* oled_handle, oled_cfg_t cfg)
{
    /* AF */
    GPIO_PinAFConfig(cfg.sda_port, cfg.sda_source, cfg.sda_af);
    GPIO_PinAFConfig(cfg.scl_port, cfg.scl_source, cfg.scl_af);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin         = cfg.scl_pin | cfg.sda_pin;
    GPIO_InitStructure.GPIO_Mode        = cfg.scl_mode;
    GPIO_InitStructure.GPIO_OType       = cfg.scl_otype;
    GPIO_InitStructure.GPIO_PuPd        = cfg.scl_pu;
    GPIO_InitStructure.GPIO_Speed       = cfg.scl_speed;
    GPIO_SetBits(cfg.scl_port, cfg.scl_pin | cfg.sda_pin);
    GPIO_Init(cfg.sda_port, &GPIO_InitStructure);
    GPIO_Init(cfg.scl_port, &GPIO_InitStructure);
}
/**
 * @brief OLED I2C 配置
 * 
 * @param oled_handle 
 * @param cfg 
 */
static void bsp_oled_i2c_cfg(oled_handle_t* oled_handle, oled_cfg_t cfg)
{
    /* 参数拷贝 */
    oled_handle->i2cx       = cfg.i2cx;
    oled_handle->oled_addr  = cfg.i2c_addr;
    /* I2C 配置 */
    I2C_DeInit(cfg.i2cx);
    I2C_InitTypeDef I2C_InitStructure;
    I2C_InitStructure.I2C_Mode                  = cfg.i2c_mode;
    I2C_InitStructure.I2C_Ack                   = cfg.i2c_ack;
    I2C_InitStructure.I2C_AcknowledgedAddress   = cfg.i2c_ack_addr;
    I2C_InitStructure.I2C_DutyCycle             = cfg.i2c_duty;
    I2C_InitStructure.I2C_ClockSpeed            = cfg.i2c_speed;
    I2C_InitStructure.I2C_OwnAddress1           = cfg.i2c_own_addr;
    I2C_Init(cfg.i2cx, &I2C_InitStructure);

    /* 使能 I2C */
    I2C_Cmd(cfg.i2cx, ENABLE);
}
/**
 * @brief OLED DMA 配置
 * 
 * @param oled_handle 
 * @param cfg 
 */
static void bsp_oled_dma_cfg(oled_handle_t* oled_handle, oled_cfg_t cfg)
{
    /* 参数拷贝 */
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_Channel               = cfg.tx_dma_channel;
    DMA_InitStructure.DMA_Priority              = cfg.dma_priority;
    DMA_InitStructure.DMA_PeripheralBaseAddr    = (uint32_t)&(oled_handle->i2cx->DR);
    DMA_InitStructure.DMA_Memory0BaseAddr       = (uint32_t)(oled_handle->screen_buf);
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Normal;
    DMA_InitStructure.DMA_FIFOMode              = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_MemoryBurst           = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst       = DMA_PeripheralBurst_Single;
    DMA_Init(cfg.dma_stream, &DMA_InitStructure);
    /* DMA NVIC */
    NVIC_InitTypeDef    NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                      = cfg.dma_irq_channel;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = cfg.dma_irq_prio;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority           = cfg.dma_irq_sub_prio;
    NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* DMA 中断使能 */
    DMA_ITConfig(cfg.dma_stream, DMA_IT_TC, ENABLE);
    /* DMA 使能 */
    I2C_DMACmd(oled_handle->i2cx, DISABLE);
}
/**
 * @brief OLED 写指令
 * @param oled_handle 
 * @param cmd 
 */
static void bsp_oled_write_cmd(oled_handle_t* oled_handle, uint8_t cmd)
{
    while (I2C_GetFlagStatus(oled_handle->i2cx, I2C_FLAG_BUSY) != RESET);
    I2C_GenerateSTART(oled_handle->i2cx, ENABLE);
    while (!I2C_CheckEvent(oled_handle->i2cx, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(oled_handle->i2cx, oled_handle->oled_addr, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData(oled_handle->i2cx, OLED_CMD_MODE);
    while (!I2C_CheckEvent(oled_handle->i2cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_SendData(oled_handle->i2cx, cmd);
    while (!I2C_CheckEvent(oled_handle->i2cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTOP(oled_handle->i2cx, ENABLE);
}
/**
 * @brief OLED 写数据
 * @param oled_handle
 * @param data
 */
static void bsp_oled_write_data(oled_handle_t* oled_handle, uint8_t data)
{
    while (I2C_GetFlagStatus(oled_handle->i2cx, I2C_FLAG_BUSY) != RESET);
    I2C_GenerateSTART(oled_handle->i2cx, ENABLE);
    while (!I2C_CheckEvent(oled_handle->i2cx, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(oled_handle->i2cx, oled_handle->oled_addr, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(oled_handle->i2cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(oled_handle->i2cx, OLED_DATA_MODE);

    while (!I2C_CheckEvent(oled_handle->i2cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_SendData(oled_handle->i2cx, data);
    while (!I2C_CheckEvent(oled_handle->i2cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_GenerateSTOP(oled_handle->i2cx, ENABLE);
}
/**
 * @brief  设置 OLED 光标位置（起始位置：左上角）
 * @param  page: 页地址 (0~7)
 * @param  col: 列地址 (0~127)
 */
static void bsp_oled_set_pos(oled_handle_t* oled_handle, uint8_t page, uint8_t col)
{
    bsp_oled_write_cmd(oled_handle, 0x00 | (col & 0x0F));
    bsp_oled_write_cmd(oled_handle, 0x10 | ((col >> 4) & 0x0F));
    bsp_oled_write_cmd(oled_handle, 0xB0 | page);
}
/**
 * @brief  OLED 全屏刷新（将 screen_buf 发送到屏幕）
 * @param  oled_handle
 */
void bsp_oled_refresh(oled_handle_t* oled_handle)
{
    uint8_t i, j;
    for (i = 0; i < OLED_PAGE_SIZE; i++)
    {
        /* 发送指令 */
        while (I2C_GetFlagStatus(oled_handle->i2cx, I2C_FLAG_BUSY) != RESET);
        I2C_GenerateSTART(oled_handle->i2cx, ENABLE);
        while (!I2C_CheckEvent(oled_handle->i2cx, I2C_EVENT_MASTER_MODE_SELECT));
        I2C_Send7bitAddress(oled_handle->i2cx, oled_handle->oled_addr, I2C_Direction_Transmitter);
        while (!I2C_CheckEvent(oled_handle->i2cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
        I2C_SendData(oled_handle->i2cx, OLED_CMD_MODE);
        while (!I2C_CheckEvent(oled_handle->i2cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
        I2C_SendData(oled_handle->i2cx, OLED_COL_LOW(0));
        while (!I2C_CheckEvent(oled_handle->i2cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
        I2C_SendData(oled_handle->i2cx, OLED_COL_HIGH(0));
        while (!I2C_CheckEvent(oled_handle->i2cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
        I2C_SendData(oled_handle->i2cx, OLED_PAGE(i));
        while (!I2C_CheckEvent(oled_handle->i2cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
        I2C_GenerateSTOP(oled_handle->i2cx, ENABLE);
        /* 发送数据 */
        while (I2C_GetFlagStatus(oled_handle->i2cx, I2C_FLAG_BUSY) != RESET);
        I2C_GenerateSTART(oled_handle->i2cx, ENABLE);
        while (!I2C_CheckEvent(oled_handle->i2cx, I2C_EVENT_MASTER_MODE_SELECT));
        I2C_Send7bitAddress(oled_handle->i2cx, oled_handle->oled_addr, I2C_Direction_Transmitter);
        while (!I2C_CheckEvent(oled_handle->i2cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
        I2C_SendData(oled_handle->i2cx, OLED_DATA_MODE);
        while (!I2C_CheckEvent(oled_handle->i2cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
        for (j = 0; j < oled_handle->width; j++)
        {

            I2C_SendData(oled_handle->i2cx, oled_handle->screen_buf[i * oled_handle->width + j]);
            while (!I2C_CheckEvent(oled_handle->i2cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
        }
        I2C_GenerateSTOP(oled_handle->i2cx, ENABLE);
    }
}
/**
 * @brief  OLED 初始化
 * 
 * @param oled_handle 
 * @param cfg 
 */
void bsp_oled_init(oled_handle_t* oled_handle, oled_cfg_t cfg)
{
    if (oled_handle == NULL)
    {
        SGCS_ERROR("oled_handle is null");
        return;
    }
    oled_handle->height         = cfg.height;
    oled_handle->width          = cfg.width;
    /* 时钟使能 */
    bsp_oled_clk_cfg(oled_handle, cfg);
    /* GPIO 配置 */
    bsp_oled_gpio_cfg(oled_handle,cfg);
    /* I2C 配置 */
    bsp_oled_i2c_cfg(oled_handle, cfg);
    /* DMA 配置 */
    bsp_oled_dma_cfg(oled_handle, cfg);
    SGCS_INFO("OLED init success");
    /* 发送 OLED 初始化命令序列 */
    for (uint8_t i = 0; i < OLED_INIT_CMD_SEQ_SIZE; i++)
    {
        bsp_oled_write_cmd(oled_handle, oled_init_cmd_seq[i]);
    }
    /* 清屏幕 */
    bsp_oled_refresh(oled_handle);
}

/**
 * @brief OLED 绘制 点
 * 
 * @param x 
 *     @arg x: 0~127
 * @param y 
 *     @arg y: 0~63
 * @param data 
 *     @arg data: 0:不显示 1:显示
 */
void OLED_DrawPoint(oled_handle_t* oled_handle, uint8_t x, uint8_t y, uint8_t data)
{
    /* 变量检测 */
    if (x > 127 || y > 63)
    {
        printf("===OLED_DrawPoint error! : x, y ===\r\n");
        return;
    }
    uint16_t byte_pos_x = x;            // x 方向 字节位置
    uint16_t byte_pos_y = y / 8;        // y 方向 字节位置
    uint8_t  bit_pos_y = y % 8;         // y 方向 位位置
    uint8_t  *to_written_p = &oled_handle->screen_buf[byte_pos_y * 128 + byte_pos_x]; // 指向对应字节
    if (data)
    {
        *to_written_p |= (1 << bit_pos_y); // 设置对应位
    }
    else
    {
        *to_written_p &= ~(1 << bit_pos_y); // 清除对应位
    }
}