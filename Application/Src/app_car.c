#include "app_car.h"
#include <stdlib.h>
#include "app_motor.h"
#include "app_buzzer.h"
#include "app_tracking.h"
#include "app_ultrasonic.h"
#include "app_servo.h"
#include "Decode.h"
#include "Encode.h"
#include "FreeRTOS.h"
#include "task.h"
#include "app_esp32_msg.h"
#include "app_compass.h"
#include "stdlib.h"
static void App_Car_Init(void);
static void App_Car_SetMode(CarMode_t mode);
static void App_Car_Run(void);

/* 全局对象实体 */
App_CarObjectTypeDef App_Car = {
    .mode       = CAR_MODE_STOP,   /* 运行模式 */
    .mode_str   = "停止",
    .roam_state = ROAM_STATE_NONE, /* 漫游状态 */
    .target     = TARGET_UNKNOW,   /* 未知 */
    .Init      = App_Car_Init,
    .SetMode   = App_Car_SetMode,
    .Run       = App_Car_Run
};

/* 随机漫游计时 */
static uint32_t s_rand_tick = 0;

static void App_Car_PCControl(void);
static void App_Car_AutoRoam(void);
static void App_Car_TraceOnly(void);

/**
 * @brief 初始化小车
 */
void App_Car_Init(void)
{
    App_Car.mode        = CAR_MODE_STOP;
    App_Motor.Init();
    App_Buzzer.Beep(50000);
}

/**
 * @brief 设置小车模式（切换模式时会自动停车）
 */
void App_Car_SetMode(CarMode_t mode)
{
    App_Motor.Stop();

    s_rand_tick = xTaskGetTickCount();
    /* 更新随机种子 */
    srand(s_rand_tick);
    App_Car.mode = mode;
    memset(App_Car.mode_str, 0, sizeof(App_Car.mode_str));
    switch (mode)
    {
        case CAR_MODE_STOP:
            snprintf((char*)App_Car.mode_str, sizeof(App_Car.mode_str), "停止");
            break;
        
        case CAR_MODE_PC_CONTROL:
            snprintf((char*)App_Car.mode_str, sizeof(App_Car.mode_str), "PC 控制");
            break;
        
        case CAR_MODE_AUTO_ROAM:
            snprintf((char*)App_Car.mode_str, sizeof(App_Car.mode_str), "自动漫游");
            break;
        
        case CAR_MODE_TRACE_ONLY:   
            snprintf((char*)App_Car.mode_str, sizeof(App_Car.mode_str), "循迹");
            break;
            
        default:
            snprintf((char*)App_Car.mode_str, sizeof(App_Car.mode_str), "未知");
            break;
    }
}

/**
 * @brief 小车运行逻辑（必须 20ms 调用一次）
 */
void App_Car_Run(void)
{
    /* 解析 模式 指令 */
    if (g_cmd_flag == 1)
    {
        if (g_cmd_pc.cmdCode == CMD_STM32_SYS) /* SYS */
        {
            if (strcmp((char*)g_cmd_pc.key, "MODE") == 0) /* MODE */
            {
                if (strcmp((char*)g_cmd_pc.value, "PCCTRL\r") == 0) /* PC */
                {
                    App_Buzzer.Beep(10000);
                    App_Car_SetMode(CAR_MODE_PC_CONTROL);
                    g_cmd_flag = 0;
                }
                else if (strcmp((char*)g_cmd_pc.value, "ROAM\r") == 0) /* ROAM */
                {
                    App_Buzzer.Beep(10000);
                    App_Car_SetMode(CAR_MODE_AUTO_ROAM);
                    g_cmd_flag = 0;
                }
                else if (strcmp((char*)g_cmd_pc.value, "TRACKER\r") == 0) /* TRACKER */
                {
                    App_Buzzer.Beep(10000);
                    App_Car_SetMode(CAR_MODE_TRACE_ONLY);
                    g_cmd_flag = 0;
                }
                else if (strcmp((char*)g_cmd_pc.value, "STOP\r") == 0) /* STOP */
                {
                    App_Buzzer.Beep(10000);
                    App_Car_SetMode(CAR_MODE_STOP);
                    g_cmd_flag = 0;
                }
                else
                {
                    printf("Unknown command: %s %s\n", (char*)g_cmd_pc.key, (char*)g_cmd_pc.value);
                    g_cmd_flag = 1;
                }
            }
        }
        /* 解析 舵机 指令 */
        else if (g_cmd_pc.cmdCode == CMD_STM32_SERVO)
        {
            float angle = atof((char*)g_cmd_pc.value);
            if (strcmp((char*)g_cmd_pc.key, "SETSERVO1") == 0)
            {
                App_Servo.SetAngle(1, angle);
                g_cmd_flag = 0;
            }
            else if (strcmp((char*)g_cmd_pc.key, "SETSERVO2") == 0)
            {
                App_Servo.SetAngle(2, angle);
                g_cmd_flag = 0;
            }
            else if (strcmp((char*)g_cmd_pc.key, "SETSERVO3") == 0)
            {
                App_Servo.SetAngle(3, angle);
                g_cmd_flag = 0;
            }
        }
        /* 复位指令解析 */
        else if (g_cmd_pc.cmdCode == CMD_SYS_RESET)
        {
            if (strcmp((char*)g_cmd_pc.key, "101") == 0)
            {
                if (strcmp((char*)g_cmd_pc.value, "010") == 0)
                {
                    App_Car.mode = CAR_MODE_STOP;
                    App_Motor.Stop();
                    g_cmd_flag = 0;
                    /* 系统复位 */
                    SCB->AIRCR  = ((0x5FA << 16) | (1 << 2));
                    while(1);
                }
            }
        }
        /* 罗盘校准指令 解析 */
        else if (g_cmd_pc.cmdCode == CMD_STM32_COMPASS)
        {
            if (strcmp((char*)g_cmd_pc.key, "CAL") == 0)
            {
                if (strcmp((char*)g_cmd_pc.value, "OK") == 0)
                {
                    App_Car.mode = CAR_MODE_STOP;
                    App_Motor.Stop();
                    /* 校准 */
                    App_Compass.Calibrate();
                    g_cmd_flag = 0;
                }
            }
        }
    }
    switch (App_Car.mode)
    {
        case CAR_MODE_STOP:         /* 停止 */
            App_Motor.Stop();
            break;

        case CAR_MODE_PC_CONTROL:   /* PC 控制模式 */
            App_Car_PCControl();
            break;

        case CAR_MODE_AUTO_ROAM:    /* 自动漫游 */
            App_Car.roam_state = ROAM_STATE_SEARCH;
            App_Car_AutoRoam();
            break;

        case CAR_MODE_TRACE_ONLY:    /* 纯边界巡视 */
            App_Car_TraceOnly();
            break;

        default:
            App_Motor.Stop();
            break;
    }
}
/**
 * @brief PC 控制模式（从队列取指令并执行）
 * @brief 指令格式：STM32,CAR,FORWARD,100
 */
static void App_Car_PCControl(void)
{
    /* 指令解析 */
    if (g_cmd_flag == 1)
    {
        if (g_cmd_pc.cmdCode == CMD_STM32_CAR)
        {
            int speed = atoi((char*)g_cmd_pc.value);
            if (strcmp((char*)g_cmd_pc.key, "FORWARD") == 0)
            {
                App_Motor.Forward();
                App_Motor.SetSpeed(speed);
                g_cmd_flag = 0;
            }
            else if (strcmp((char*)g_cmd_pc.key, "BACK") == 0)
            {
                App_Motor.SetSpeed(speed);
                App_Motor.Backward();
                g_cmd_flag = 0;
            }
            else if (strcmp((char*)g_cmd_pc.key, "LEFT") == 0)
            {
                App_Motor.SetSpeed(speed);
                App_Motor.TurnLeft();
                g_cmd_flag = 0;
            }
            else if (strcmp((char*)g_cmd_pc.key, "RIGHT") == 0)
            {
                App_Motor.SetSpeed(speed);
                App_Motor.TurnRight();
                g_cmd_flag = 0;
            }
            else if (strcmp((char*)g_cmd_pc.key, "STOP") == 0)
            {
                App_Motor.Stop();
                g_cmd_flag = 0;
            }
        }
    }
}
/**
 * @brief 自动漫游：找垃圾 + 靠近 + 报警 + 掉头
 */
static void App_Car_AutoRoam(void)
{
    static uint32_t count = 0; /* 进入计数器 */
    static Direction_t direction_rand = DIR_LEFT; /* 随机方向 */
    static Direction_t last_dir = DIR_RIGHT; /* 上一次方向 */
    /* 指令解析 有无找到目标 */
    if (g_cmd_flag == 1)
    {
        if (g_cmd_pc.cmdCode == CMD_PC_TARGET)
        {
            if (strcmp((char*)g_cmd_pc.key, "HAVE") == 0) /* 有无目标 */
            {
                if (strcmp((char*)g_cmd_pc.value, "NONE") == 0) /* 未发现目标 */
                {
                    App_Car.roam_state = ROAM_STATE_SEARCH; /* 进入搜索 */ 
                }
                if (strcmp((char*)g_cmd_pc.value, "FOUND") == 0) /* 发现目标 */
                {
                    App_Car.roam_state = ROAM_STATE_APPROACH; /* 进入靠近 */
                }
                if (strcmp((char*)g_cmd_pc.value, "LOST") == 0) /* 失去目标 */
                {
                    App_Car.roam_state = ROAM_STATE_SEARCH; /* 进入搜索 */ 
                }
                if (strcmp((char*)g_cmd_pc.value, "SUCCES") == 0) /* 目标已找到 */
                {
                    App_Car.roam_state = ROAM_STATE_SUCCES; /* 成功 */
                }
            }
        }
        g_cmd_flag = 0;
    }
    if (App_Car.roam_state == ROAM_STATE_SEARCH) /* 搜索 */
    {
        App_Motor.SetSpeed(250);
        if (count++ > 1000) 
        {
            last_dir = direction_rand;      /* 记录上一次方向 */
            direction_rand = (rand() % 2) ? DIR_RIGHT : DIR_LEFT;    /* 方向 */
            count = 0;
        }
        if (count < 300)
        {
            if (direction_rand == DIR_LEFT)
            {
                App_Motor.TurnLeft();
            }
            else
            {
                App_Motor.TurnRight();
            }
        }
        else
        {
            uint16_t distance = App_Ult.GetDistance();
            if (distance < 10 && distance > 0)
            {
                count = 0; /* 重置计数器 */
                App_Car.roam_state = ROAM_STATE_ALARM_BACK; /* 切换为 避障 */
            }
            else
            {
                App_Motor.Forward();
            }
        }
    }
    if (App_Car.roam_state == ROAM_STATE_APPROACH) /* 发现靠近 由 PC 指挥 */
    {
        if (g_cmd_flag == 1)
        {
            if (g_cmd_pc.cmdCode == CMD_STM32_CAR)
            {
                int speed = atoi((char*)g_cmd_pc.value);
                if (strcmp((char*)g_cmd_pc.key, "FORWARD") == 0)
                {
                    App_Motor.Forward();
                    App_Motor.SetSpeed(speed);
                    g_cmd_flag = 0;
                }
                else if (strcmp((char*)g_cmd_pc.key, "BACK") == 0)
                {
                    App_Motor.SetSpeed(speed);
                    App_Motor.Backward();
                    g_cmd_flag = 0;
                }
                else if (strcmp((char*)g_cmd_pc.key, "LEFT") == 0)
                {
                    App_Motor.SetSpeed(speed);
                    App_Motor.TurnLeft();
                    g_cmd_flag = 0;
                }
                else if (strcmp((char*)g_cmd_pc.key, "RIGHT") == 0)
                {
                    App_Motor.SetSpeed(speed);
                    App_Motor.TurnRight();
                    g_cmd_flag = 0;
                }
                else if (strcmp((char*)g_cmd_pc.key, "STOP") == 0)
                {
                    App_Motor.Stop();
                    g_cmd_flag = 0;
                }
            }
        }
    }
    if (App_Car.roam_state == ROAM_STATE_ALARM_BACK) /* 障碍物 */
    {
        if (count++ < 500) 
        {
            App_Motor.Backward();/* 先 后退 300 单位 */
        }
        else
        {
            if (last_dir != DIR_LEFT) /* 之前是向左转，则向右转 */
            {
                App_Motor.TurnRight();
            } else
            {
                App_Motor.TurnLeft();
            }
            uint16_t distance = App_Ult.GetDistance;
            if (distance > 15)
            {
                count = 0; /* 重新计数 */
                App_Car.roam_state = ROAM_STATE_SEARCH; /* 切换为 寻找模式 */
            }
        }
    }
    if (App_Car.roam_state == ROAM_STATE_SUCCES) /* 成功 */
    {
        App_Motor.Stop();
        App_Buzzer.Beep(60000); /* 长鸣 */
    }
}
/**
 * @brief 小车仅仅进行循迹
 * 
 */
static void App_Car_TraceOnly(void)
{
    uint32_t time_tick = xTaskGetTickCount();
    const uint16_t limit = 8; /* 8cm */
    uint16_t distance = App_Ult.GetDistance();
    /* 三路状态桥接 1: 黑线（LED灭） 0：白色背景、被抬起（LED亮） */
    uint8_t left    = *(App_Tracking.tracker1_sta);
    uint8_t mid     = *(App_Tracking.tracker2_sta);
    uint8_t right   = *(App_Tracking.tracker3_sta);

    extern uint8_t car_tracker_only_flag;
    /* 三路全部离线（抬起、离线） */
    if (!left && !mid && !right) 
    {
        /* 超声波距离检测 */
        if (distance < limit && distance > 0)
        {
            App_Buzzer.Beep(40000);
        }
        else
        {
            App_Motor.SetSpeed(400);
            App_Motor.Forward();
        }
    }
    /* 偏左 */
    else if (left && !mid && !right)
    {
        App_Motor.SetSpeed(400);
        App_Motor.TurnLeft();
    }
    /* 偏左中 */
    else if (left && mid && !right)
    {
        /* 超声波距离检测 */
        if (distance < limit && distance > 0)
        {
            App_Buzzer.Beep(40000);
        }
        else
        {
            App_Motor.SetSpeed(400);
            App_Motor.Forward();
        }
    }
    /* 中间 */
    else if (!left && mid && !right)
    {
        /* 超声波距离检测 */
        if (distance < limit && distance > 0)
        {
            App_Buzzer.Beep(40000);
        }
        else
        {
            App_Motor.SetSpeed(400);
            App_Motor.Forward();
        }
    }
    /* 偏右 */
    else if (!left && !mid && right)
    {
        App_Motor.SetSpeed(400);
        App_Motor.TurnRight();
    }
    /* 偏右中 */
    else if (!left && mid && right)
    {
        /* 超声波距离检测 */
        if (distance < limit && distance > 0)
        {
            App_Buzzer.Beep(40000);
        }
        else
        {
            App_Motor.SetSpeed(400);
            App_Motor.Forward();
        }
    }
    /* 全部触线 */
    else if (left && mid && right)
    {
        App_Motor.Stop();
    }
}