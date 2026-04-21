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
#include "bsp_servo.h"
#include "bsp_encoder.h"
#include "bsp_battery.h"
#include "bsp_esp32_msg.h"
#include "string.h"
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
servo_handle_t servo1;
servo_handle_t servo2;
servo_handle_t servo3;
encoder_handle_t encoder;
battery_handle_t battery;
esp32_msg_handle_t esp32_msg;
static void press(const void *pvParameters)
{
    printf("press\n");
} 
static void long_press(const void *pvParameters)
{
    printf("long press\n");
}
static void release(const void *pvParameters)
{
    printf("release\n");
}
static void rotate_counterclockwise(const void *pvParameters)
{
    encoder_handle_t *handle = (encoder_handle_t*)pvParameters;
    printf("count: %d\r\n",handle->count);
}
static void rotate_clockwise(const void *pvParameters)
{
    encoder_handle_t *handle = (encoder_handle_t*)pvParameters;
    printf("count: %d\r\n",handle->count);
}
int main(void)
{
    /* 时钟配置 */
    SysClockConfig_HSE25MHZ();
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));
    led_init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    bsp_message_init(&msg, &USART1_DEFAULT_CONFIG());
    printf("msg init ok \r\n");
    /* ESP32 通讯初始化 */
    bsp_esp32_msg_init(&esp32_msg, &ESP32_MSG_USART6_DEFAULT_CONFIG());
    bsp_esp32_msg_send(&esp32_msg, "msg init ok \r\n", strlen("msg init ok \r\n"));
    printf("esp32 msg init ok \r\n");
    /* 创建任务 */
    if(xTaskCreate(app, "app", 1024, NULL, 5, NULL) != pdPASS)
    {
        printf("create task failed\r\n");
    }
    vTaskStartScheduler();
    while(1)
    {

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
    //bsp_buzzer_init(&buzzer, &BUZZER_DEFAULT_CONFIG());
    //buzzer_play_dandelion_interlude(&buzzer);
    /* 循迹初始化 */
    bsp_tracking_init(&track_a, &TRACKER_A_DEFAULT_CONFIG());
    bsp_tracking_init(&track_b, &TRACKER_B_DEFAULT_CONFIG());
    bsp_tracking_init(&track_c, &TRACKER_C_DEFAULT_CONFIG());
    /* 马达初始化 */
    //bsp_motor_init(&motor, &MOTOR_DEFAULT_CONFIG());
    /* 舵机初始化 */
    bsp_servo_init(&servo1, &SERVO1_DEFAULT_CONFIG());
    bsp_servo_init(&servo2, &SERVO2_DEFAULT_CONFIG());
    bsp_servo_init(&servo3, &SERVO3_DEFAULT_CONFIG());
    /* 编码器初始化 */
    encoder_cfg_t encoder_config = ENCODER_DEFAULT_CONFIG();
    encoder.event_callback.press = press;
    encoder.event_callback.longpress = long_press;
    encoder.event_callback.release = release;
    encoder.event_callback.rotate_clockwise = rotate_clockwise;
    encoder.event_callback.rotate_counterclockwise = rotate_counterclockwise;
    bsp_encoder_init(&encoder, &encoder_config);
    /* 电池测量初始化 */
    bsp_battery_init(&battery, &BATTERY_DEFAULT_CONFIG());

    bsp_oled_welcome(&oled);
    bsp_buzzer_on(&buzzer);
    vTaskDelay(200);
    bsp_buzzer_off(&buzzer);

    uint16_t count = 0;
    float angle = 0;
    while(1)
    {
        count++;
        // bsp_compass_read(&compass);
        // bsp_ultrasonic_trigger(&ult);
        // printf("angle: %.2f x:%d\ty:%d\tz:%d\tdistance:%d\tbuzz:%s\tleft:%s\tmid:%s\tright:%s\tcount:%d\n",compass.data->angle,
        //        compass.data->x, compass.data->y, compass.data->z,
        //        ult.distance_cm,
        //        buzzer.is_open ? "on" : "off",
        //        track_a.sta ? "black" : "white",
        //        track_b.sta ? "black" : "white",
        //        track_c.sta ? "black" : "white",
        //        count);
        // if (ult.distance_cm < 20   &&  ult.distance_cm > 1)
        // {
        //     //bsp_buzzer_on(&buzzer);
        // }
        // else
        // {
        //     //bsp_buzzer_off(&buzzer);
        // }
        // angle += 10;
        // if (angle > 180)
        // {
        //     angle = 0;
        // }
        // printf("angle: %.2f\n",angle);
        // bsp_servo_set_angle(&servo1, angle);
        // bsp_servo_set_angle(&servo2, angle);
        // bsp_servo_set_angle(&servo3, angle);
        // printf("battery: %.2fV\n",bsp_battery_get_voltage(&battery));
        // printf("level: %d%%\n",bsp_battery_get_level(&battery));
        // bsp_esp32_msg_send(&esp32_msg, "hello world\r\n", 13);
        vTaskDelay(10);
        if (esp32_msg.new_msg_flag)
        {
            esp32_msg.new_msg_flag = 0;
            bsp_esp32_msg_send(&esp32_msg, esp32_msg.buf, esp32_msg.rx_len);
        }
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