#include "app.h"
void app()
{
    
    uint16_t count = 0;
    while(1)
    {
        count++;
        vTaskDelay(50);
        if (g_usart1_debug_handle != NULL)
        {
            if (g_usart1_debug_handle->new_msg_flag)
            {
                g_usart1_debug_handle->new_msg_flag = 0;
                printf("usart Receive: %s\n",g_usart1_debug_handle->rx_buf);
            }
        }

    }
}