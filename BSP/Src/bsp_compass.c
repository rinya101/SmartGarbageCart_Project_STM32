/*******************************************************************************
 *      文    件: bsp_compass.h
 *      说    明: 适用于STM32F4(具体为 STM32F411CEU6) 的 指南针 传感器驱动
 *      特别说明： 指南针数据变量已经定义，具体请参考本文件
 *      版    本: V1.0
 *      作    者: Rinya101(http://github.com/rinya101) 学号：220802040137
 *      版权说明：本程序代码仅用于 2026 本科毕业设计，不得用于其他商业用途，
 *               可以作为个人参考学习使用。
*******************************************************************************/
#include "bsp_compass.h"
#include "bsp_debug.h"
#include <math.h>
#include "FreeRTOS.h"
#include "task.h"
/**
 * @brief 指南针数据
 * 
 */
copmass_data_t s_compass_data;
/**
 * @brief 寄存器地址
 * 
 */
/**
 * @brief 向指南针模块写入一个字节的数据
 * 
 * @param handle 
 * @param reg 
 * @param data 
 */
static compass_status_t compass_write_reg(compass_handle_t *handle, uint8_t reg, uint8_t data)
{
    compass_status_t status = COMPASS_OK;
    uint32_t timeout = 0;
    timeout = COMPASS_I2C_TIMEOUT;
    while(I2C_GetFlagStatus(handle->dev.i2c, I2C_FLAG_BUSY) != RESET)
    {
        if(timeout-- == 0)
        {
            SGCS_ERROR("Timeout");
            return COMPASS_ERROR;
        }
    }
    timeout = COMPASS_I2C_TIMEOUT;
    I2C_GenerateSTART(handle->dev.i2c, ENABLE);
    while(!I2C_CheckEvent(handle->dev.i2c, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if(timeout-- == 0)
        {
            SGCS_ERROR("Timeout");
            return COMPASS_ERROR;
        }
    }
    timeout = COMPASS_I2C_TIMEOUT;
    I2C_Send7bitAddress(handle->dev.i2c, handle->dev.addr, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(handle->dev.i2c, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if(timeout-- == 0)
        {
            SGCS_ERROR("Timeout");
            return COMPASS_ERROR;
        }
    }
    timeout = COMPASS_I2C_TIMEOUT;
    I2C_SendData(handle->dev.i2c, reg);
    while(!I2C_CheckEvent(handle->dev.i2c, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        if(timeout-- == 0)
        {
            SGCS_ERROR("Timeout");
            return COMPASS_ERROR;
        }
    }
    timeout = COMPASS_I2C_TIMEOUT;
    I2C_SendData(handle->dev.i2c, data);
    while(!I2C_CheckEvent(handle->dev.i2c, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        if(timeout-- == 0)
        {
            SGCS_ERROR("Timeout");
            return COMPASS_ERROR;
        }
    }
    I2C_GenerateSTOP(handle->dev.i2c, ENABLE);
    return status;
}
/**
 * @brief 从指南针模块读取一个字节的数据
 * 
 * @param handle 
 * @param reg 
 * @return uint8_t 
 */
static uint8_t compass_read_reg(compass_handle_t *handle, uint8_t reg)
{
    uint8_t data = 0;
    uint32_t timeout = 0;
    if (I2C_GetFlagStatus(handle->dev.i2c, I2C_FLAG_BUSY))
    {
        I2C_GenerateSTOP(handle->dev.i2c, ENABLE);
        for (uint32_t i = 1000; i; i--);
    }
    timeout = COMPASS_I2C_TIMEOUT;
    while(I2C_GetFlagStatus(handle->dev.i2c, I2C_FLAG_BUSY) != RESET)
    {
        if(timeout-- == 0)
        {
            SGCS_ERROR("Timeout");
            return COMPASS_ERROR;
        }
    }
    timeout = COMPASS_I2C_TIMEOUT;
    I2C_GenerateSTART(handle->dev.i2c, ENABLE);
    while(!I2C_CheckEvent(handle->dev.i2c, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if(timeout-- == 0)
        {
            SGCS_ERROR("Timeout");
            return COMPASS_ERROR;
        }
    }
    timeout = COMPASS_I2C_TIMEOUT;
    I2C_Send7bitAddress(handle->dev.i2c, handle->dev.addr, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(handle->dev.i2c, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if(timeout-- == 0)
        {
            SGCS_ERROR("Timeout");
            return COMPASS_ERROR;
        }
    }
    timeout = COMPASS_I2C_TIMEOUT;
    I2C_SendData(handle->dev.i2c, reg);
    while(!I2C_CheckEvent(handle->dev.i2c, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        if(timeout-- == 0)
        {
            SGCS_ERROR("Timeout");
            return COMPASS_ERROR;
        }
    }
    timeout = COMPASS_I2C_TIMEOUT;
    I2C_GenerateSTART(handle->dev.i2c, ENABLE);
    while(!I2C_CheckEvent(handle->dev.i2c, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if(timeout-- == 0)
        {
            SGCS_ERROR("Timeout");
            return COMPASS_ERROR;
        }
    }
    timeout = COMPASS_I2C_TIMEOUT;
    I2C_Send7bitAddress(handle->dev.i2c, handle->dev.addr, I2C_Direction_Receiver);
    while(!I2C_CheckEvent(handle->dev.i2c, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
        if(timeout-- == 0)
        {
            SGCS_ERROR("Timeout");
            return COMPASS_ERROR;
        }
    }
    I2C_AcknowledgeConfig(handle->dev.i2c, DISABLE);
    timeout = COMPASS_I2C_TIMEOUT;
    while(!I2C_CheckEvent(handle->dev.i2c, I2C_EVENT_MASTER_BYTE_RECEIVED))
    {
        if(timeout-- == 0)
        {
            SGCS_ERROR("Timeout");
            return COMPASS_ERROR;
        }
    }
    data = I2C_ReceiveData(handle->dev.i2c);
    I2C_GenerateSTOP(handle->dev.i2c, ENABLE);
    I2C_AcknowledgeConfig(handle->dev.i2c, ENABLE);
    return data;
}
/**
 * @brief 从I2C设备读取数据
 * 
 * @param handle 
 * @param reg 
 * @param buf 
 * @param len 
 */
static compass_status_t compass_read_bytes(compass_handle_t *handle, uint8_t reg, uint8_t *buf, uint8_t len)
{
    compass_status_t status = COMPASS_OK;
    uint32_t timeout = COMPASS_I2C_TIMEOUT;
    while(I2C_GetFlagStatus(handle->dev.i2c, I2C_FLAG_BUSY))
    {
        if(timeout-- == 0)
        {
            SGCS_ERROR("Timeout");
            return COMPASS_ERROR;
        }
    }
    timeout = COMPASS_I2C_TIMEOUT;
    I2C_GenerateSTART(handle->dev.i2c, ENABLE);
    while(!I2C_CheckEvent(handle->dev.i2c, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if(timeout-- == 0)
        {
            SGCS_ERROR("Timeout");
            return COMPASS_ERROR;
        }
    }
    timeout = COMPASS_I2C_TIMEOUT;
    I2C_Send7bitAddress(handle->dev.i2c, handle->dev.addr, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(handle->dev.i2c, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if(timeout-- == 0)
        {
            SGCS_ERROR("Timeout");
            return COMPASS_ERROR;
        }
    }
    timeout = COMPASS_I2C_TIMEOUT;
    I2C_SendData(handle->dev.i2c, reg);
    while(!I2C_CheckEvent(handle->dev.i2c, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        if(timeout-- == 0)
        {
            SGCS_ERROR("Timeout");
            return COMPASS_ERROR;
        }
    }
    timeout = COMPASS_I2C_TIMEOUT;
    I2C_GenerateSTART(handle->dev.i2c, ENABLE);
    while(!I2C_CheckEvent(handle->dev.i2c, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if(timeout-- == 0)
        {
            SGCS_ERROR("Timeout");
            return COMPASS_ERROR;
        }
    }
    timeout = COMPASS_I2C_TIMEOUT;
    I2C_Send7bitAddress(handle->dev.i2c, handle->dev.addr, I2C_Direction_Receiver);
    while(!I2C_CheckEvent(handle->dev.i2c, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
        if(timeout-- == 0)
        {
            SGCS_ERROR("Timeout");
            return COMPASS_ERROR;
        }
    }
    while (len--)
    {
        if (len == 0)
            I2C_AcknowledgeConfig(handle->dev.i2c, DISABLE);
        timeout = COMPASS_I2C_TIMEOUT;
        while(!I2C_CheckEvent(handle->dev.i2c, I2C_EVENT_MASTER_BYTE_RECEIVED))
        {
            if(timeout-- == 0)
            {
                SGCS_ERROR("Timeout");
                return COMPASS_ERROR;
            }
        }
        *buf++ = I2C_ReceiveData(handle->dev.i2c);
    }
    I2C_GenerateSTOP(handle->dev.i2c, ENABLE);
    I2C_AcknowledgeConfig(handle->dev.i2c, ENABLE);

    return status;
}
/**
 * @brief 指南针时钟配置
 * 
 * @param cfg 
 */
static void compass_clk_init(const compass_cfg_t *cfg)
{
    /* GPIO CLK */
    RCC_AHB1PeriphClockCmd(cfg->gpio_sda_rcc, ENABLE);
    RCC_AHB1PeriphClockCmd(cfg->gpio_scl_rcc, ENABLE);
    /* I2C CLK */
    RCC_APB1PeriphClockCmd(cfg->i2c_rcc, ENABLE);
}
/**
 * @brief 指南针GPIO配置
 * 
 * @param cfg 
 */
static void compass_gpio_init(const compass_cfg_t *cfg)
{
    /* AF */
    GPIO_PinAFConfig(cfg->gpio_sda_port, cfg->gpio_sda_source, cfg->gpio_sda_af);
    GPIO_PinAFConfig(cfg->gpio_scl_port, cfg->gpio_scl_source, cfg->gpio_scl_af);
    /* SDA */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin         = cfg->gpio_sda_pin;
    GPIO_InitStructure.GPIO_Mode        = cfg->gpio_sda_mode;
    GPIO_InitStructure.GPIO_OType       = cfg->gpio_sda_otype;
    GPIO_InitStructure.GPIO_PuPd        = cfg->gpio_sda_pu;
    GPIO_InitStructure.GPIO_Speed       = cfg->gpio_sda_speed;
    GPIO_SetBits(cfg->gpio_sda_port, cfg->gpio_sda_pin);
    GPIO_Init(cfg->gpio_sda_port, &GPIO_InitStructure);
    /* SCL */
    GPIO_InitStructure.GPIO_Pin         = cfg->gpio_scl_pin;
    GPIO_InitStructure.GPIO_Mode        = cfg->gpio_scl_mode;
    GPIO_InitStructure.GPIO_OType       = cfg->gpio_scl_otype;
    GPIO_InitStructure.GPIO_PuPd        = cfg->gpio_scl_pu;
    GPIO_InitStructure.GPIO_Speed       = cfg->gpio_scl_speed;
    GPIO_SetBits(cfg->gpio_scl_port, cfg->gpio_scl_pin);
    GPIO_Init(cfg->gpio_scl_port, &GPIO_InitStructure);
}
/**
 * @brief 指南针I2C初始化
 * 
 * @param cfg 
 */
static void compass_i2c_init(const compass_cfg_t *cfg)
{
    /* 默认 */
    I2C_DeInit(cfg->i2c);
    /* I2C 配置 */
    I2C_InitTypeDef  I2C_InitStructure;
    I2C_InitStructure.I2C_Mode                  = cfg->i2c_mode;
    I2C_InitStructure.I2C_DutyCycle             = cfg->i2c_duty;
    I2C_InitStructure.I2C_OwnAddress1           = cfg->i2c_own_addr;
    I2C_InitStructure.I2C_Ack                   = cfg->i2c_ack;
    I2C_InitStructure.I2C_AcknowledgedAddress   = cfg->i2c_ack_addr;
    I2C_InitStructure.I2C_ClockSpeed            = cfg->i2c_speed;
    I2C_Init(cfg->i2c, &I2C_InitStructure);
    /* 使能I2C */
    I2C_Cmd(cfg->i2c, ENABLE);

}
/**
 * @brief 指南针传感器初始化
 * 
 * @param handle 
 */
static void compass_sensor_init(compass_handle_t *handle)
{
    // 软复位
    compass_write_reg(handle, QMC5883P_CTRL2_REG, 0x80);
    // 控制寄存器2：Set/Reset开启，量程8G
    compass_write_reg(handle, QMC5883P_CTRL2_REG, 0x08);
    // 控制寄存器1：正常模式，200Hz输出
    compass_write_reg(handle, QMC5883P_CTRL1_REG, 0xCD);
    uint8_t id = compass_read_reg(handle, QMC5883P_CHIPID_REG);
    handle->id = id;
    SGCS_LOG("compass ID = 0x%02x\n",handle->id);
}
/**
 * @brief 指南针初始化
 * 
 * @param compass_handle 
 * @param compass_cfg 
 */
compass_status_t bsp_compass_init(compass_handle_t *compass_handle,const compass_cfg_t *cfg)
{
    compass_status_t status = COMPASS_OK;
    /* 传感器句柄 */
    compass_handle->data        = &s_compass_data;
    compass_handle->dev.i2c     = cfg->i2c;
    compass_handle->dev.addr    = cfg->addr;
    /* 时钟配置 */
    compass_clk_init(cfg);
    /* GPIO配置 */
    compass_gpio_init(cfg);
    /* I2C 配置 */
    compass_i2c_init(cfg);
    /* 传感器初始化 */
    compass_sensor_init(compass_handle);
    /* 初始化完成 */
    SGCS_INFO("compass init success!");
    return status;
}
/**
 * @brief 获取指南针数据（已加入硬铁补偿 + 角度补偿）
 * 
 * @param compass_handle 
 */
compass_status_t bsp_compass_read(compass_handle_t *compass_handle)
{
    compass_status_t status = COMPASS_OK;
    uint8_t buf[6];
    status = compass_read_bytes(compass_handle, QMC5883P_XOUT_L_REG, buf, 6);
    if (status != COMPASS_OK)
    {
        SGCS_ERROR("compass read data fail!");
        return status;
    }

    // 原始数据拼接
    int16_t x = (buf[1] << 8) | buf[0];
    int16_t y = (buf[3] << 8) | buf[2];
    int16_t z = (buf[5] << 8) | buf[4];

    // ===================== 【补偿：硬铁偏移】 =====================
    // 根据你提供的 北/东/南/西 4点校准数据计算得出
    const int16_t x_offset = 372;  // X 补偿
    const int16_t y_offset = -294; // Y 补偿
    const int16_t z_offset = -361; // Z 补偿

    x += x_offset;  // 原始X + 补偿
    y += y_offset;  // 原始Y + 补偿
    z += z_offset;  // 原始Z + 补偿

    // 存入补偿后的坐标
    compass_handle->data->x = x;
    compass_handle->data->y = y;
    compass_handle->data->z = z;

    // ===================== 【补偿：角度校准】 =====================
    float angle = atan2(y, x) * 180.0f / 3.1415926f;
    if(angle < 0) angle += 360.0f;

    // 角度补偿：测量值 - 21.13°
    const float angle_offset = -21.13f;
    angle += angle_offset;
    if(angle < 0) angle += 360.0f;  // 防止负角度
    if(angle > 360) angle -= 360.0f;

    // 存入补偿后的角度
    compass_handle->data->angle = angle;

    return status;
}

/**
 * @brief 指南针一键自动校准（水平转一圈即可）
 * 校准原理：采集最大/最小值，求中点 = 硬铁偏移
 */
void bsp_compass_calibrate(compass_handle_t *compass_handle)
{
    int16_t x, y;
    int16_t x_min = 32767, x_max = -32768;
    int16_t y_min = 32767, y_max = -32768;

    SGCS_INFO("开始校准指南针... 请水平旋转设备 360°");
    vTaskDelay(1000);

    // 连续采样 500 次，采集最大最小值
    for (int i = 0; i < 500; i++)
    {
        uint8_t buf[6];
        compass_read_bytes(compass_handle, QMC5883P_XOUT_L_REG, buf, 6);

        x = (buf[1] << 8) | buf[0];
        y = (buf[3] << 8) | buf[2];

        // 更新极值
        if (x < x_min) x_min = x;
        if (x > x_max) x_max = x;
        if (y < y_min) y_min = y;
        if (y > y_max) y_max = y;
        printf("x: %d, y: %d\n", x, y);
        vTaskDelay(20); // 20ms 采样一次
    }

    // 计算偏移：(最大值 + 最小值)/2
    compass_handle->x_offset = (x_min + x_max) / 2;
    compass_handle->y_offset = (y_min + y_max) / 2;

    SGCS_INFO("指南针校准完成！");
    SGCS_INFO("X偏移: %d, Y偏移: %d",
              compass_handle->x_offset,
              compass_handle->y_offset);
}