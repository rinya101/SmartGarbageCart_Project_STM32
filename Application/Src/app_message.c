/**
 * @file app_message.c
 * @brief 应用层消息（单例面向对象）
 */
#include "app_message.h"
#include "bsp_message.h"
#include "PeripheralParamConfig.h"

/* 底层驱动句柄 */
static message_handle_t msg_handle;

/* 内部函数 */
static AppStatusTypeDef App_Message_Init(void);
static AppStatusTypeDef App_Message_Send(uint8_t *data, uint16_t len);
static AppStatusTypeDef App_Message_Process(void);
static void App_Message_RxCallback(message_handle_t *handle);

/* 全局单例对象 */
AppMessageObjectTypeDef App_Message = {
    .Init    = App_Message_Init,
    .Send    = App_Message_Send,
    .Process = App_Message_Process,
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
 * @brief 发送数据
 * @param data
 * @param len
 * @return AppStatusTypeDef
 */
static AppStatusTypeDef App_Message_Send(uint8_t *data, uint16_t len)
{
    if (data == NULL || len == 0)
        return APP_INVALID;

    bsp_message_send_string(&msg_handle, (char *)data, len);
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
    // 接收回显
    App_Message.Send(handle->rx_buf, handle->rx_len);
}