#include "EncoderObject.h"
static encoder_handle_t s_bsp_encoder_handle; 
static EncoderObject s_app_encoder_object;

static int16_t prv_get_value(void* self) 
{
    EncoderObject* obj = (EncoderObject*)self;
    if (obj == NULL || obj->bsp_handle == NULL) return 0;
    encoder_handle_t* bsp_h = (encoder_handle_t*)obj->bsp_handle;
    return bsp_h->count;
}

static key_state_e prv_get_btn_state(void* self) 
{
    EncoderObject* obj = (EncoderObject*)self;
    if (obj == NULL || obj->bsp_handle == NULL) return UNKNOW;

    encoder_handle_t* bsp_h = (encoder_handle_t*)obj->bsp_handle;
    return bsp_h->btn_state; 
}

static encoder_turn_t prv_get_turn_state(void* self) 
{
    EncoderObject* obj = (EncoderObject*)self;
    if (obj == NULL || obj->bsp_handle == NULL) return STOP;

    encoder_handle_t* bsp_h = (encoder_handle_t*)obj->bsp_handle;
    return bsp_h->turn_state;
}

EncoderObject* CreateEncoderObject()
{
    /* 创建对象 */
    EncoderObject* newEncoderObject = &s_app_encoder_object;
    /* 初始化对象 */
    newEncoderObject->bsp_handle = &s_bsp_encoder_handle;
    newEncoderObject->current_value = 0;
    newEncoderObject->current_btn_state = UNKNOW;
    newEncoderObject->current_turn_state = STOP;
    /* 方法挂载 */
    newEncoderObject->get_value = prv_get_value;
    newEncoderObject->get_btn_state = prv_get_btn_state;
    newEncoderObject->get_turn_state = prv_get_turn_state;
    
    return newEncoderObject;
}