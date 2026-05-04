#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "app_esp32_msg.h"
#include "bsp_esp32_msg.h"
#include "PeripheralParamConfig.h"
#include "app_message.h"
#include "Decode.h"
#include "Encode.h"
/**
 * @brief 解码 标志位 内容
 * 
 */
extern Decode_Cmd_t g_cmd_pc;
extern uint8_t g_cmd_flag;

static AppESP32MsgObjectTypedef* App_ESP32MsgInit(void);
static AppESP32MsgObjectTypedef* App_ESP32MsgSend(uint8_t *buf);
static AppESP32MsgObjectTypedef* App_ESP32MsgRecLoop(void);
static int App_ESP32MsgSendCmd(Decode_Cmd_t *cmd);
static void AppESP32RecEventCallbact(void *param);


esp32_msg_handle_t esp32msg;

AppESP32MsgObjectTypedef App_ESP32Msg = {
    .Init       = App_ESP32MsgInit,
    .Send       = App_ESP32MsgSend,
    .Scan       = App_ESP32MsgRecLoop,
    .SendCmd    = App_ESP32MsgSendCmd,
};


static void AppESP32RecEventCallbact(void *param)
{
    /* 剔除无效指令 */
    if(strlen((char*)esp32msg.buf) < 5)
    return;
    if (Decode_ParseFrame(esp32msg.buf, &g_cmd_pc) == 0)
    {
        g_cmd_flag = 1;
        portYIELD_FROM_ISR(NULL);
    }
}
/**
 * @brief ESP32 消息初始化
 * 
 * @return AppESP32MsgObjectTypedef 
 */
static AppESP32MsgObjectTypedef* App_ESP32MsgInit(void)
{
    esp32msg.esp32_msg_callback.rx_callback = AppESP32RecEventCallbact;
    bsp_esp32_msg_init(&esp32msg, &ESP32_MSG_USART6_DEFAULT_CONFIG());
    return &App_ESP32Msg;
}
/**
 * @brief ESP32 消息发送
 * 
 * @return AppESP32MsgObjectTypedef 
 */
static AppESP32MsgObjectTypedef* App_ESP32MsgSend(uint8_t *buf)
{
    uint16_t len = strlen((char*)buf);
    bsp_esp32_msg_send(&esp32msg, buf, len);
    return &App_ESP32Msg;
}
/**
 * @brief ESP32 消息循环接收
 * 
 * @return AppESP32MsgObjectTypedef 
 */
static AppESP32MsgObjectTypedef* App_ESP32MsgRecLoop(void)
{
    if (esp32msg.new_msg_flag)
    {
        esp32msg.new_msg_flag = 0;
    }
    return &App_ESP32Msg;
}
/**
 * @brief ESP32 消息发送指令
 * 
 * @param cmd 
 * @return AppESP32MsgObjectTypedef* 
 */
static int App_ESP32MsgSendCmd(Decode_Cmd_t *cmd)
{
    uint8_t buf[128] = {0};

    if(Encode_Frame(cmd, buf) != 0)
        return -1;
    strcat((char*)buf, "\r\n");

    bsp_esp32_msg_send(&esp32msg, buf, strlen((char*)buf));

    return 0;
}
