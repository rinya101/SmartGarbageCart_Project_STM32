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
/* 全局小车状态 */
CarMode_t     g_car_mode      = CAR_MODE_NONE;
RoamState_t   g_car_roam_state= ROAM_STATE_SEARCH;

static void App_Car_Init(void);
static void App_Car_SetMode(CarMode_t mode);
static void App_Car_Run(void);

/* 全局对象实体 */
const App_CarObjectTypeDef App_Car = {
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
    // 默认设置为停止模式
    g_car_mode = CAR_MODE_STOP;
    g_car_roam_state = ROAM_STATE_SEARCH;
    App_Motor.Init();
    App_Buzzer.Beep(50000);
}

/**
 * @brief 设置小车模式（切换模式时会自动停车）
 */
void App_Car_SetMode(CarMode_t mode)
{
    App_Motor.Stop();

    g_car_roam_state = ROAM_STATE_SEARCH;
    s_rand_tick = xTaskGetTickCount();
    // 设置新模式
    g_car_mode = mode;
}

/**
 * @brief 小车运行逻辑（必须 20ms 调用一次）
 */
void App_Car_Run(void)
{
    switch (g_car_mode)
    {
        case CAR_MODE_STOP:         /* 停止 */
            App_Motor.Stop();
            break;

        case CAR_MODE_PC_CONTROL:   /* PC 控制模式 */
            App_Car_PCControl();
            break;

        case CAR_MODE_AUTO_ROAM:    /* 自动漫游 */
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
    Decode_Cmd_t cmd;
    if (g_cmd_flag == 1)
    {
        g_cmd_flag = 0;
        //printf("[PC]:Cmd: %d | Key: %s | Value: %s\r\n",g_cmd_pc.cmdCode, g_cmd_pc.key, g_cmd_pc.value);
        if (g_cmd_pc.cmdCode == CMD_STM32_CAR)
        {
            int speed = atoi((char*)g_cmd_pc.value);
            if (strcmp((char*)g_cmd_pc.key, "FORWARD") == 0)
            {
                App_Motor.Forward();
                printf("forward speed :%d\n", speed);
                App_Motor.SetSpeed(speed);
                cmd.cmdCode = CMD_PC_CAR_STA,
                strcpy((char*)cmd.key, "FORWARD");
                strcpy((char*)cmd.value, "OK");
                App_ESP32Msg.SendCmd(&cmd);
            }
            else if (strcmp((char*)g_cmd_pc.key, "BACK") == 0)
            {
                App_Motor.SetSpeed(speed);
                App_Motor.Backward();
                cmd.cmdCode = CMD_PC_CAR_STA,
                strcpy((char*)cmd.key, "BACK");
                strcpy((char*)cmd.value, "OK");
                App_ESP32Msg.SendCmd(&cmd);
            }
            else if (strcmp((char*)g_cmd_pc.key, "LEFT") == 0)
            {
                App_Motor.SetSpeed(speed);
                App_Motor.TurnLeft();
                cmd.cmdCode = CMD_PC_CAR_STA,
                strcpy((char*)cmd.key, "LEFT");
                strcpy((char*)cmd.value, "OK");
                App_ESP32Msg.SendCmd(&cmd);
            }
            else if (strcmp((char*)g_cmd_pc.key, "RIGHT") == 0)
            {
                App_Motor.SetSpeed(speed);
                App_Motor.TurnRight();
                cmd.cmdCode = CMD_PC_CAR_STA,
                strcpy((char*)cmd.key, "RIGHT");
                strcpy((char*)cmd.value, "OK");
                App_ESP32Msg.SendCmd(&cmd);
            }
            else if (strcmp((char*)g_cmd_pc.key, "STOP") == 0)
            {
                App_Motor.Stop();
                cmd.cmdCode = CMD_PC_CAR_STA,
                strcpy((char*)cmd.key, "STOP");
                strcpy((char*)cmd.value, "OK");
                App_ESP32Msg.SendCmd(&cmd);
            }
        }
        /* 舵机 */
        else if (g_cmd_pc.cmdCode == CMD_STM32_SERVO)
        {
            float angle = atof((char*)g_cmd_pc.value);
            if (strcmp((char*)g_cmd_pc.key, "SETSERVO1") == 0)
            {
                App_Servo.SetAngle(1, angle);   
            }
            else if (strcmp((char*)g_cmd_pc.key, "SETSERVO2") == 0)
            {
                App_Servo.SetAngle(2, angle);
            }
            else if (strcmp((char*)g_cmd_pc.key, "SETSERVO3") == 0)
            {
                App_Servo.SetAngle(3, angle);
            }
        }
    }
}
/**
 * @brief 自动漫游：找垃圾 + 靠近 + 报警 + 掉头
 */
static void App_Car_AutoRoam(void)
{

}
/**
 * @brief 小车仅仅进行循迹
 * 
 */
static void App_Car_TraceOnly(void)
{

}