#include "app_oled.h"
#include "PeripheralParamConfig.h"
#include "bsp_oled.h"
/**
 * @brief OLED 底层句柄
 * 
 */
oled_handle_t oled;

static AppStatusTypeDef App_OledInit(void);
static AppStatusTypeDef App_OledClear(void);

/**
 * @brief 导出对象
 * 
 */
AppOledObjectTypedef App_Oled = {
    .Init = App_OledInit,
    .Clear = App_OledClear,
};

/**
 * @brief OLED 初始化
 * 
 */
static AppStatusTypeDef App_OledInit(void)
{
    bsp_oled_init(&oled, &OLED_DEFAULT_CONFIG());
    return APP_OK;
}
/**
 * @brief OLEd 清屏
 * 
 * @return AppStatusTypeDef 
 */
static AppStatusTypeDef App_OledClear(void)
{
    uint16_t width = oled.width;
    uint16_t height = oled.height;
    for (uint32_t i = 0; i < width * height; i++)
    {
        oled.screen_buf[i] = 0x00;
    }
    bsp_oled_refresh(&oled);

    return APP_OK;
}
