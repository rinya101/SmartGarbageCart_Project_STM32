#include "app_buzzer.h"
#include "bsp_buzzer.h"
#include "PeripheralParamConfig.h"

static AppStatusTypeDef App_BuzzerInit(void);
static AppStatusTypeDef App_BuzzerBeep(uint16_t time);
static AppStatusTypeDef App_BuzzerOn(void);
static AppStatusTypeDef App_BuzzerOff(void);
static AppStatusTypeDef App_BuzzerTrigger(void);
static AppStatusTypeDef App_BuzzerSetPeriod(uint16_t period);
static AppStatusTypeDef App_BuzzerSetDutyCycle(uint16_t dutyCycle);
static AppStatusTypeDef App_BuzzerPlayerMusic(void);
/**
 * @brief 蜂鸣器单例程
 * 
 */
AppBuzzerObjectTypeDef App_Buzzer = {
    .Init = App_BuzzerInit,
    .Beep = App_BuzzerBeep,
    .On = App_BuzzerOn,
    .Off = App_BuzzerOff,
    .Trigger = App_BuzzerTrigger,
    .SetPeriod = App_BuzzerSetPeriod,
    .SetDuty = App_BuzzerSetDutyCycle,
    .PlayMusic = App_BuzzerPlayerMusic,
};
/**
 * @brief 蜂鸣器句柄
 * 
 */
buzzer_handle_t buzzer;

/**
 * @brief 蜂鸣器初始化
 * 
 * @return AppBuzzerObjectTypeDef 
 */
static AppStatusTypeDef App_BuzzerInit(void)
{
    bsp_buzzer_init(&buzzer, &BUZZER_DEFAULT_CONFIG());
    return APP_OK;
}
/**
 * @brief 蜂鸣器beep
 * 
 * @param time 
 * @return AppStatusTypeDef 
 */
static AppStatusTypeDef App_BuzzerBeep(uint16_t time)
{
    bsp_buzzer_beep(&buzzer, time);
    return APP_OK;
}
/**
 * @brief 蜂鸣器打开
 * 
 * @return AppStatusTypeDef 
 */
static AppStatusTypeDef App_BuzzerOn(void)
{
    bsp_buzzer_on(&buzzer);
    return APP_OK;
}
/**
 * @brief 蜂鸣器关闭
 * 
 * @return AppStatusTypeDef 
 */
static AppStatusTypeDef App_BuzzerOff(void)
{
    bsp_buzzer_off(&buzzer);
    return APP_OK;
}
/**
 * @brief 蜂鸣器触发
 * 
 * @return AppStatusTypeDef 
 */
static AppStatusTypeDef App_BuzzerTrigger(void)
{
    bsp_buzzer_trigger(&buzzer);
    return APP_OK;
}
/**
 * @brief 蜂鸣器设置周期
 *
 * @param period 
 *     @arg 0 - 65535
 * @return AppStatusTypeDef 
 */
static AppStatusTypeDef App_BuzzerSetPeriod(uint16_t period)
{
    bsp_buzzer_set_duty(&buzzer, period);
    return APP_OK;
}
/**
 * @brief 蜂鸣器设置占空比
 * 
 * @param dutyCycle 
 *     @arg 0 - 65535
 * @return AppStatusTypeDef 
 */
static AppStatusTypeDef App_BuzzerSetDutyCycle(uint16_t dutyCycle)
{
    bsp_buzzer_set_duty(&buzzer, dutyCycle);
    return APP_OK;
}
/**
 * @brief 蜂鸣器播放音乐
 * @note 内部 加入 《蒲公英的约定》- 周杰伦 前奏
 * @return AppStatusTypeDef 
 */
static AppStatusTypeDef App_BuzzerPlayerMusic(void)
{
    buzzer_play_dandelion_interlude(&buzzer);
    return APP_OK;
}