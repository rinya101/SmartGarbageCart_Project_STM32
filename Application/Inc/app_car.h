#ifndef __APP_CAR_H
#define __APP_CAR_H
#include <stdint.h>
#include "Decode.h"
extern Decode_Cmd_t g_cmd_pc;
extern uint8_t g_cmd_flag;

/* 小车运行模式 */
typedef enum {
    CAR_MODE_STOP          = -1,    /* 停止模式 */
    CAR_MODE_NONE          = 0,     /* 未定义 */
    CAR_MODE_PC_CONTROL,            /* PC 远程控制 */
    CAR_MODE_AUTO_ROAM,             /* 自动漫游（找垃圾） */
    CAR_MODE_TRACE_ONLY             /* 纯循迹模式 */
} CarMode_t;

/* 自动漫游内部状态 */
typedef enum {
    ROAM_STATE_SEARCH,             /* 搜索垃圾 */
    ROAM_STATE_APPROACH,           /* 靠近垃圾 */
    ROAM_STATE_ALARM_BACK          /* 报警 + 掉头 */
} RoamState_t;

/* 面向对象结构体 */
typedef struct {
    void (*Init)(void);
    void (*SetMode)(CarMode_t mode);
    void (*Run)(void);
} App_CarObjectTypeDef;

/* 全局模式变量 */
extern CarMode_t     g_car_mode;
extern RoamState_t   g_car_roam_state;

/* 全局唯一小车对象（面向对象调用） */
extern const App_CarObjectTypeDef App_Car;


#endif