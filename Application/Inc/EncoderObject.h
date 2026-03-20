#ifndef _ENCODER_OBJECT_H
#define _ENCODER_OBJECT_H
#include "bsp_encoder.h"
#include "stdbool.h"
typedef struct 
{
    /*------------------ 属性 -------------------*/
    /**
     * @brief bsp层 encoder_handle_t 的指针
     * 
     */
    void* bsp_handle;
    /**
     * @brief 当前编码器的值 对应于 bsp_handle->count
     * 
     */
    int16_t                 current_value;
    /**
     * @brief 当前按键状态
     * 
     */
    key_state_e             current_btn_state;
    /**
     * @brief 当前旋转状态
     * 
     */
    encoder_turn_t          current_turn_state;
    /* 方法 */
    /**
     * @brief 获取当前编码器的值
     * 
     */
    int16_t (*get_value)(void* self);
    /**
     * @brief 获取当前按键状态
     * 
     */
    key_state_e (*get_btn_state)(void* self);
    /**
     * @brief 获取当前转向
     * 
     */
    encoder_turn_t (*get_turn_state)(void* self);
} EncoderObject;

EncoderObject* CreateEncoderObject();

#endif