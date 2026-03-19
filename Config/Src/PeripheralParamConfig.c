#include "PeripheralParamConfig.h"
#include "bsp_encoder.h"


/* ---------------------------------- OLED 配置 ----------------------------- */
/**
 * @brief OLED 初始化命令序列（SSD1306 128x64）
 */
const uint8_t oled_init_cmd_seq[] = {
    0xAE,   // 关闭显示
    0xD5,   // 设置时钟分频
    0x80,   
    0xA8,   // 多路复用率
    0x3F,   
    0xD3,   // 设置显示偏移
    0x00,
    0x10,   
    0x40,   // 设置显示开始行
    0x8D,   // 电荷泵
    0x14,   // 开启电荷泵
    0x20,   // 内存地址模式
    0x00,   // 页面寻址模式 （0x00:横向 0x01纵向 0x02）
    0xA1,   // 段重映射
    0xC8,   // COM扫描方向
    0xDA,   // COM引脚配置
    0x12,   
    0x81,   // 对比度
    0xCF,   
    0xD9,   // 预充电周期
    0xF1,   
    0xDB,   // VCOMH取消选择级别
    0x30,   
    0xA4,   // 全局显示开启
    0xA6,   // 正常显示
    0x2E,   // 取消滚动
    0xAF    // 开启显示
};