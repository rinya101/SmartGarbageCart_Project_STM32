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
        /* 向 PC 发送指令 */
        case CMD_PC_CAR_STA:
            sprintf((char *)buf, "PC,CAR,%s,%s",cmd->key, cmd->value);
            break;
        case CMD_PC_ULTRASONIC_DISTANCEUPLOAD:
            sprintf((char *)buf, "PC,ULT,%s,%s", cmd->key, cmd->value);
            break;
        case CMD_PC_COMPASS_ANGLEUPLOAD:
            sprintf((char *)buf, "PC,COMPASS,%s,%s", cmd->key, cmd->value);break;
        case CMD_PC_BAT_PERCENTUPLOAD:
            sprintf((char *)buf, "PC,BATTERY,%s,%s", cmd->key, cmd->value);break;
        default:
            printf("Unknown Command Code!\n");
            return -1;
    }

    return 0;
}