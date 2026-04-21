/**
 * @file app_battery.c
 * @brief 电池应用层（面向对象实现）
 */
#include <stdio.h>
#include "app_message.h"
#include "app_battery.h"
#include "bsp_battery.h"
#include "PeripheralParamConfig.h"
/* 底层句柄 */
static battery_handle_t battery_handle;

/* 内部函数 */
static AppStatusTypeDef App_Battery_Init(void);
static uint8_t App_Battery_GetLevel(void);
static uint8_t App_Battery_IsLowPower(void);
static AppStatusTypeDef App_Battery_PrintMsg(void);
/* 导出对象 */
AppBatteryObjectTypeDef App_Battery = {
    .level      = 0,
    .Init       = App_Battery_Init,
    .GetLevel   = App_Battery_GetLevel,
    .IsLowPower = App_Battery_IsLowPower,
    .Print   = App_Battery_PrintMsg,
};

/**
 * @brief 初始化
 * @return AppStatusTypeDef
 */
static AppStatusTypeDef App_Battery_Init(void)
{
    bsp_battery_init(&battery_handle, &BATTERY_DEFAULT_CONFIG());
    return APP_OK;
}

/**
 * @brief 获取电量
 * @return 0~100%
 */
static uint8_t App_Battery_GetLevel(void)
{
    App_Battery.level = bsp_battery_get_level(&battery_handle);
    return App_Battery.level;
}

/**
 * @brief 是否低电量
 * @return 1=低电
 */
static uint8_t App_Battery_IsLowPower(void)
{
    if (App_Battery_GetLevel() <= 20)
        return 1;
    return 0;
}

/**
 * @brief 通过MSG输出电量（你要的方法）
 * @return AppStatusTypeDef
 */
static AppStatusTypeDef App_Battery_PrintMsg(void)
{
    uint8_t buf[64];
    uint8_t level = App_Battery_GetLevel();

    /* 格式化消息 */
    sprintf((char *)buf, "[Battery] Level: %d%%\r\n", level);

    /* 调用APP MSG发送 */
    return App_Message.Send(buf, strlen((char *)buf));
}