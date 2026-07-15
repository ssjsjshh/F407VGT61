#include "app_mpu.h"
#include "service_mpu.h"
#include "service_oled.h"
#include <stdio.h>
/* MPU显示缓存初始化 */
static char pitch_buf[8];
static char roll_buf[8];
/* MPU改变显示 */
static void mpu_show()
{
    if (show_flag!=1)return; 
    snprintf(roll_buf, sizeof(roll_buf), "%+06.1f", show.rol);
    snprintf(pitch_buf, sizeof(pitch_buf), "%+06.1f", show.pitch);
    oled_clear_areabuf(37, 2 ,8);
    oled_clear_areabuf(37, 3 ,8);
    oled_buf(37, 2, pitch_buf, 1);
    oled_buf(37, 3, roll_buf, 1);
    oled_refresh(2,4);
    show_flag=0;
}
/* MPU初始化框架构建 */
void mpu_init()
{
    mpu_service_init();
    oled_clear_areabuf(0, 2 ,120);
    oled_buf(0,2,"pitch:",1);
    oled_buf(0,3,"roll :",1);
}
/* MPU循环任务 */
void mpu_task()
{
    mpu_updata();
    mpu_show();
}