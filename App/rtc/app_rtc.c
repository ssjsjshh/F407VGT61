#include "app_rtc.h"
#include "bsp_rtc.h"
#include "service_oled.h"
#include <stdio.h>
/* 变量初始化 */
static uint8_t last_date = 0;
static char date_buf[14];
static char time_buf[12];
/* 时钟显示更新 */
void rtc_display()
{
    if(rtc_flag==0)
    return;
    rtc_flag=0;
    rtc_read();
    if(last_date!=sdate.Date)
    {
        last_date=sdate.Date;
        snprintf(date_buf, sizeof(date_buf), "20%02d/%02d/%02d", sdate.Year, sdate.Month, sdate.Date);
        oled_clear_areabuf(31, 0 ,16);
        oled_buf(31, 0, date_buf, 1);
        oled_refresh(0,1);
    }
    snprintf(time_buf, sizeof(time_buf), "%02d:%02d:%02d", stime.Hours, stime.Minutes, stime.Seconds);
    oled_clear_areabuf(31, 1 ,14);
    oled_buf(31, 1, time_buf, 1);
    oled_refresh(1,2);
}