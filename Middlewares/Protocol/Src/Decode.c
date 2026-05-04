#include "Decode.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Decode_Cmd_t g_cmd_pc = {0};
uint8_t g_cmd_flag = 0;

/**
 * @brief 指令分割
 *
 * @param s
 * @param index
 * @param dest
 * @param max_len
 */
static void get_field(const uint8_t *s, int index, uint8_t *dest, int max_len)
{
    if (s == NULL || dest == NULL || max_len <= 0)
    {
        if (dest)
            dest[0] = '\0';
        return;
    }

    int start = 0;
    int end = 0;
    int current = 0;
    int len = strlen((char*)s);

    while (current <= index && end < len)
    {
        if (s[end] == ',' || s[end] == '\0')
        {
            if (current == index)
            {
                break;
            }
            current++;
            start = end + 1;
        }
        end++;
    }

    if (current != index)
    {
        dest[0] = '\0';
        return;
    }

    int copy_len = end - start;
    if (copy_len >= max_len)
        copy_len = max_len - 1;
    strncpy((char*)dest, (char*)s + start, copy_len);
    dest[copy_len] = '\0';
}
/**
 * @brief 指令解析
 * 
 * @param buf 未解析指令字符串
 * @param cmd 解析后的指令结构体
 * @return int 0 成功，-1 失败
 */
int Decode_ParseFrame(uint8_t *buf, Decode_Cmd_t *cmd)
{
    if (buf == NULL || cmd == NULL)
        return -1;
    cmd->cmdCode = CMD_NONE;                    /* 清空指令 */
    memset(cmd->key, 0, sizeof(cmd->key));      /* 清空key */
    memset(cmd->value, 0, sizeof(cmd->value));  /* 清空value */
    /* 提取字段 */
    uint8_t target[16] = {0};
    uint8_t item[16]  = {0};
    uint8_t key[16]   = {0};
    uint8_t value[32] = {0};
    get_field(buf, 0, target, sizeof(target));
    get_field(buf, 1, item, sizeof(item));
    get_field(buf, 2, key, sizeof(key));
    get_field(buf, 3, value, sizeof(value));

    /* 目标校验 */
    if (strcmp((const char *)target, "STM32") != 0)
        return -1;
    /* LED */
    if (strcmp((char*)item, "LED") == 0)
    {
        cmd->cmdCode = CMD_STM32_LED;
        strcpy((char*)cmd->key, (char*)key);
        strcpy((char*)cmd->value, (char*)value);
        return 0;
    }
    /* CAR */
    if (strcmp((char*)item, "CAR") == 0)
    {
        cmd->cmdCode = CMD_STM32_CAR;
        strcpy((char*)cmd->key, (char*)key);
        strcpy((char*)cmd->value, (char*)value);
        return 0;
    }
    /* SERVO */
    if (strcmp((char*)item, "SERVO") == 0)
    {
        cmd->cmdCode = CMD_STM32_SERVO;
        strcpy((char*)cmd->key, (char*)key);
        strcpy((char*)cmd->value, (char*)value);
        return 0;
    }
    /* COMPASS */
    if (strcmp((char*)item, "COMPASS") == 0) 
    {
        cmd->cmdCode = CMD_STM32_COMPASS;
        strcpy((char*)cmd->key, (char*)key);
        strcpy((char*)cmd->value, (char*)value);
        return 0;
    }
    /* ULTRASONIC */
    if (strcmp((char*)item, "ULT") == 0) 
    {
        cmd->cmdCode = CMD_STM32_ULTRASONIC;
        strcpy((char*)cmd->key, (char*)key);
        strcpy((char*)cmd->value, (char*)value);
        return 0;
    }
    return -1;
}