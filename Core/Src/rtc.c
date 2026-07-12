#include "rtc.h"
#include "main.h"   
#include "oled_api.h"
#include <stdint.h>
#include <stdio.h>
/* -------------------------------结构体初始化------------------------------------------- */
RTC_TimeTypeDef stime;
RTC_DateTypeDef sdate;
extern RTC_HandleTypeDef hrtc;
/* -------------------------------变量初始化------------------------------------------- */
volatile uint8_t rtc_flag=1;
static uint8_t last_date = 0;
char date_buf[14];
char time_buf[12];
/* -------------------------------数据读取------------------------------------------- */
static void Rtc_read()
{
    HAL_RTC_GetTime(&hrtc, &stime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sdate, RTC_FORMAT_BIN);
}
/* --------------------------------rtc唤醒中断------------------------------------------ */
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
    rtc_flag=1;
}
/* --------------------------------时钟显示更新------------------------------------------ */
void Rtc_display()
{
    if(rtc_flag==0)
    return;
    rtc_flag=0;
    Rtc_read();
    if(last_date!=sdate.Date)
    {
        last_date=sdate.Date;
        snprintf(date_buf, sizeof(date_buf), "20%02d/%02d/%02d", sdate.Year, sdate.Month, sdate.Date);
        OLED_Clear_AreaBuf(31, 0 ,16);
        OLED_Buf(31, 0, date_buf, 1);
        OLED_Refresh(0,1);
    }
    snprintf(time_buf, sizeof(time_buf), "%02d:%02d:%02d", stime.Hours, stime.Minutes, stime.Seconds);
    OLED_Clear_AreaBuf(31, 1 ,14);
    OLED_Buf(31, 1, time_buf, 1);
    OLED_Refresh(1,2);
}