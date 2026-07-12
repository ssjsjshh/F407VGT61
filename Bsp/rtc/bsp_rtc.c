#include "bsp_rtc.h"
#include "rtc.h"
/* -------------------------------结构体初始化------------------------------------------- */
RTC_TimeTypeDef stime;
RTC_DateTypeDef sdate;
extern RTC_HandleTypeDef hrtc;
/* -------------------------------标志变量初始化------------------------------------------- */
volatile uint8_t rtc_flag=1;
/* -------------------------------数据读取------------------------------------------- */
void Rtc_read()
{
    HAL_RTC_GetTime(&hrtc, &stime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sdate, RTC_FORMAT_BIN);
}
/* --------------------------------rtc唤醒中断------------------------------------------ */
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
    rtc_flag=1;
}