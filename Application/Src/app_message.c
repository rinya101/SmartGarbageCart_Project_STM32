/**
 * @file app_message.c
 * @brief 应用层消息（单例面向对象）
 */
#include "app_message.h"
#include "bsp_message.h"
#include "PeripheralParamConfig.h"
#include "app_motor.h"
#include "app_esp32_msg.h"
/* 底层驱动句柄 */
static message_handle_t msg_handle;

/* 内部函数 */
static AppStatusTypeDef App_Message_Init(void);
static AppStatusTypeDef App_Message_Send(uint8_t *data);
static AppStatusTypeDef App_Message_Process(void);
static void App_Message_RxCallback(message_handle_t *handle);
static AppStatusTypeDef App_MessageDeCode(void);

/* 全局单例对象 */
AppMessageObjectTypeDef App_Message = {

    .newMsg = 0,

    .Init    = App_Message_Init,
    .Send    = App_Message_Send,
    .Process = App_Message_Process,
    .Scan = App_MessageDeCode,
};

/**
 * @brief 初始化
 * @return AppStatusTypeDef
 */
static AppStatusTypeDef App_Message_Init(void)
{
    bsp_message_init(&msg_handle, &USART1_DEFAULT_CONFIG());
    msg_handle.rx_callback = App_Message_RxCallback;
    return APP_OK;
}

/**
 * @brief 发送数据（自动补充 \r\n）
 * @param data 要发送的字符串
 * @return AppStatusTypeDef
 */
static AppStatusTypeDef App_Message_Send(uint8_t *data)
{
    if (data == NULL)
        return APP_INVALID;
    uint8_t buf[64] = {0};
    uint16_t len = strlen((char *)data);
    if (len >= sizeof(buf) - 2)
        return APP_INVALID;
    strncpy((char *)buf, (char *)data, sizeof(buf) - 2);
    len = strlen((char *)buf);
    if (len >= 2 && buf[len-2] == '\r' && buf[len-1] == '\n')
    {

    }
    else
    {
        buf[len++] = '\r';
        buf[len++] = '\n';
        buf[len] = '\0'; 
    }

    bsp_message_send_string(&msg_handle, (char *)buf, len);

    return APP_OK;
}

/**
 * @brief 轮询处理
 * @return AppStatusTypeDef
 */
static AppStatusTypeDef App_Message_Process(void)
{
    return APP_OK;
}

/**
 * @brief 接收回调
 * @param handle
 */
static void App_Message_RxCallback(message_handle_t *handle)
{
    App_Message.newMsg = 1;
    // 接收回显
    //App_Message.Send(handle->rx_buf, handle->rx_len);
}
/**
 * @brief 消息解码
 * 指令格式：
 *    M,F,H   前进 高速
 *    M,B,M   后退 中速
 *    M,S,L   停止 (S=Stop)
 * @return AppStatusTypeDef 
 */
static AppStatusTypeDef App_MessageDeCode(void)
{
    if (App_Message.newMsg)
    {
        App_Message.newMsg = 0;
        App_Message.Send("[Log Send]:");
        App_Message.Send(msg_handle.rx_buf);
    }
    return APP_OK;
}