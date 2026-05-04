#include "Encode.h"
#include "string.h"
#include "stdio.h"
#include "Decode.h"

/**
 * @brief  指令编码（结构体 → 字符串）
 * @param  cmd  指令结构体
 * @param  buf  输出缓冲区
 * @return int  0成功
 */
int Encode_Frame(Decode_Cmd_t *cmd, uint8_t *buf)
{
    if (cmd == NULL || buf == NULL)
        return -1;
    memset(buf, 0, 128);
    
    switch (cmd->cmdCode)
    {
        case CMD_STM32_CAR:
            // 格式：STM32,CAR,FORWARD,500
            sprintf((char *)buf, "STM32,CAR,%s,%s", cmd->key, cmd->value);
            break;

        case CMD_STM32_SERVO:
            // 格式：STM32,SERVO,1,90
            sprintf((char *)buf, "STM32,SERVO,%s,%s", cmd->key, cmd->value);
            break;

        case CMD_STM32_LED:
            // 格式：STM32,LED,RED,ON
            sprintf((char *)buf, "STM32,LED,%s,%s", cmd->key, cmd->value);
            break;

        case CMD_STM32_ULTRASONIC:
            sprintf((char *)buf, "STM32,ULT,%s,%s", cmd->key, cmd->value);
            break;

        case CMD_STM32_COMPASS:
            sprintf((char *)buf, "STM32,COMPASS,%s,%s", cmd->key, cmd->value);
            break;
        case CMD_PC_CAR_STA:
            sprintf((char *)buf, "PC,CAR,%s,%s",cmd->key, cmd->value);
            break;
        default:
            printf("Unknown Command Code!\n");
            return -1;
    }

    return 0;
}