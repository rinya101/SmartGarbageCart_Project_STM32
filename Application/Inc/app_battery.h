/**
 * @brief 电池应用层接口（面向对象风格）
 * @note 仅提供业务所需电量百分比，与电压硬件无关
 */

#ifndef _APP_BATTERY_H_
#define _APP_BATTERY_H_

#include "app_ctype.h"

/**
 * @brief 电池对象结构体（OOP）
 */
typedef struct
{
    /* 属性 */
    uint8_t level;      /* 电量百分比 0~100% */

    /* 方法 */
    AppStatusTypeDef (*Init)(void);
    uint8_t (*GetLevel)(void);
    uint8_t (*IsLowPower)(void);
    AppStatusTypeDef (*Print)(void);
} AppBatteryObjectTypeDef;

/**
 * @brief 全局电池对象
 */
extern AppBatteryObjectTypeDef App_Battery;

#endif