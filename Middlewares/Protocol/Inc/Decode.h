#ifndef _DECODE_H_
#define _DECODE_H_
#include <stdint.h>
#include "FreeRTOS.h"
#include "queue.h"
/**
 * @brief 指令枚举
 * 
 */
typedef enum
{
    CMD_ERROR = -1,         /* 指令解析错误 */
    CMD_NONE = 0,           /* 未知指令 */
    /* STM32 */
    CMD_STM32_LED,          /* LED */
    CMD_STM32_CAR,          /* 车子 */
    CMD_STM32_SERVO,        /* 舵机 */
    CMD_STM32_COMPASS,      /* 电子罗盘 */
    CMD_STM32_ULTRASONIC,   /* 超声波 */
    /* PC */
    CMD_PC_CAR_STA,                     /* 车子 */
    CMD_PC_SERVO_ANGLEUPLOAD,           /* 舵机 */
    CMD_PC_COMPASS_ANGLEUPLOAD,         /* 电子罗盘 */
    CMD_PC_ULTRASONIC_DISTANCEUPLOAD,   /* 超声波 */
    CMD_PC_BAT_PERCENTUPLOAD,           /* 电池电量 */
    CMD_PC_TARGET,                      /* 目标 */
    /* SYS */ 
    CMD_STM32_SYS,                /* 系统指令 */
    CMD_SYS_RESET,                /* 系统复位 */
} Decode_CmdCode_t;

/**
 * @brief 指令结构体
 * 
 */
typedef struct {
    Decode_CmdCode_t cmdCode;       /* 指令码 */
    uint8_t key[16];                /* 键 */
    uint8_t value[32];              /* 值 */
} Decode_Cmd_t;



/**
 * @brief  解析一帧字符串指令 STM32,CAR,FORWARD,100
 * @param  buf: 原始数据
 * @param  cmd: 解析后输出的指令结构体
 * @retval 0:成功  -1:失败
 */
int Decode_ParseFrame(uint8_t *buf, Decode_Cmd_t *cmd);

#endif /* _DECODE_H_ */