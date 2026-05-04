#include "task_app.h"
#include "app_message.h"
#include "app_esp32_msg.h"
#include "app_oled.h"
#include "app_compass.h"
#include "app_buzzer.h"
#include "app_ultrasonic.h"
#include "app_motor.h"
#include "app_servo.h"
#include "app_tracking.h"
#include "PeripheralParamConfig.h"
#include "bsp_oled.h"
#include "bsp_motor.h"
extern motor_handle_t motor;
/**
 * @brief APP 业务
 * 
 * @param pvParameters 
 */
void task_app(void *pvParameters)
{
    App_Buzzer.Beep(50000);
    while(1)
    {
        App_ESP32Msg.Scan();
        App_Message.Scan();
        vTaskDelay(5);
    }
}