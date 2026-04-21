#include "task_app.h"


/**
 * @brief APP 业务
 * 
 * @param pvParameters 
 */
void task_app(void *pvParameters)
{
    while(1)
    {
        vTaskDelay(100);
    }
}