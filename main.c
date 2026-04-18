
#include "stm32f4xx.h"
#include "SysClockConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "bsp_message.h"
#include <string.h>
#include "bsp_encoder.h"
#include "PeripheralParamConfig.h"
#include "bsp_ultrasonic.h"
#include "bsp_oled.h"
#include "bsp_tracking.h"
#include "bsp_compass.h"
#include "bsp_motor.h"
#include "bsp_buzzer.h"
void app(void *pvParameters);
void led_init(void);
message_handle_t msg;
oled_handle_t oled;
compass_handle_t compass;
buzzer_handle_t buzzer;
ult_handle_t ult;
track_handle_t track_a;
track_handle_t track_b;
track_handle_t track_c;
motor_handle_t motor;
int main(void)
{
    /* 时钟配置 */
    SysClockConfig_HSE25MHZ();
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));
    led_init();
    bsp_message_init(&msg, &USART1_DEFAULT_CONFIG());
    printf("msg init ok \r\n");
    /* 创建任务 */
    if(xTaskCreate(app, "app", 1024, NULL, 5, NULL) != pdPASS)
    {
        printf("create task failed\r\n");
    }
    vTaskStartScheduler();
    while(1)
    {
        printf("[Error]: FreeRTOS scheduler error!\r\n");
    }
}

/**
 * @brief 用户任务
 */
void app(void *pvParameters)
{
    /* 罗盘初始化 */
    bsp_compass_init(&compass, &COMPASS_DEFAULT_CONFIG());
    //bsp_compass_calibrate(&compass);
    /* OLED 初始化 */
    bsp_oled_init(&oled, &OLED_DEFAULT_CONFIG());
    /* 超声波初始化 */
    bsp_ultrasonic_init(&ult, &ULT_DEFAULT_CONDFIG());
    /* 蜂鸣器初始化 */
    bsp_buzzer_init(&buzzer, &BUZZER_DEFAULT_CONFIG());
    buzzer_play_dandelion_interlude(&buzzer);
    /* 循迹初始化 */
    bsp_tracking_init(&track_a, &TRACKER_A_DEFAULT_CONFIG());
    bsp_tracking_init(&track_b, &TRACKER_B_DEFAULT_CONFIG());
    bsp_tracking_init(&track_c, &TRACKER_C_DEFAULT_CONFIG());
    /* 马达初始化 */
    bsp_motor_init(&motor, &MOTOR_DEFAULT_CONFIG());
    bsp_oled_welcome(&oled);
    bsp_buzzer_on(&buzzer);
    vTaskDelay(200);
    bsp_buzzer_off(&buzzer);

    for (int16_t speed = -1000; speed < 1000; speed += 10)
    {
        printf("[Info]: Speed: %d\r\n", speed);
        bsp_motor_set_speed(&motor, speed, speed);
        vTaskDelay(200);
    }
    bsp_motor_set_speed(&motor, 0, 0);
    uint16_t count = 0;
    while(1)
    {
        count++;
        bsp_compass_read(&compass);
        bsp_ultrasonic_trigger(&ult);
        printf("angle: %.2f x:%d\ty:%d\tz:%d\tdistance:%d\tbuzz:%s\tleft:%s\tmid:%s\tright:%s\tcount:%d\n",compass.data->angle,
               compass.data->x, compass.data->y, compass.data->z,
               ult.distance_cm,
               buzzer.is_open ? "on" : "off",
               track_a.sta ? "black" : "white",
               track_b.sta ? "black" : "white",
               track_c.sta ? "black" : "white",
               count);
        if (ult.distance_cm < 20   &&  ult.distance_cm > 1)
        {
            bsp_buzzer_on(&buzzer);
        }
        else
        {
            bsp_buzzer_off(&buzzer);
        }
        vTaskDelay(80);
        if (msg.new_msg_flag)
        {
            msg.new_msg_flag = 0;
            printf("usart Receive: %s\n",msg.rx_buf);
        }
    }
}
void led_init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}