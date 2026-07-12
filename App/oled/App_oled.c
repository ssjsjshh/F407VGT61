#include "App_oled.h"
#include "service_oled.h"
/* ---------------------------OLED初始化框架构建----------------------------------------------- */
void OLED_Init_Frame()
{
    OLED_Init();
    OLED_Buf(0,0,"date:",1);
    OLED_Buf(0,1,"time:",1);
    //OLED_Buf(0,2,"Wait Calibrating:",1); 
    OLED_Refresh(0,3);  
}