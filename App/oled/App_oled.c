#include "app_oled.h"
#include "service_oled.h"
/* OLED初始化框架构建 */
void oled_init()
{
    oled_service_init();
    oled_buf(0,0,"date:",1);
    oled_buf(0,1,"time:",1);
    oled_buf(0,2,"Wait Calibrating:",1); 
    oled_refresh(0,3);  
}