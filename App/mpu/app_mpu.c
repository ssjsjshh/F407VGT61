#include "app_mpu.h"
#include "service_mpu.h"
#include "service_oled.h"
#include <stdio.h>
/* ---------------------------MPU显示缓存初始化----------------------------------------------- */
static char pitch_buf[8];
static char roll_buf[8];
/* ---------------------------MPU初始化框架构建----------------------------------------------- */
void MPU_Init()
{
    MPU_Service_Init();
    OLED_Clear_AreaBuf(0, 2 ,120);
    OLED_Buf(0,2,"pitch:",1);
    OLED_Buf(0,3,"roll :",1);
}
/* ---------------------------MPU改变显示----------------------------------------------- */
static void MPU_Show()
{
    if (show_flag!=1)return; 
    snprintf(roll_buf, sizeof(roll_buf), "%+06.1f", show.rol);
    snprintf(pitch_buf, sizeof(pitch_buf), "%+06.1f", show.pitch);
    OLED_Clear_AreaBuf(37, 2 ,8);
    OLED_Clear_AreaBuf(37, 3 ,8);
    OLED_Buf(37, 2, pitch_buf, 1);
    OLED_Buf(37, 3, roll_buf, 1);
    OLED_Refresh(2,4);
    show_flag=0;
}
/* ---------------------------MPU循环任务----------------------------------------------- */
void MPU_Task()
{
    MPU_Updata();
    MPU_Show();
}