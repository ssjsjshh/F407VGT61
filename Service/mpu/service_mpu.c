#include "service_mpu.h"
#include "moudle_mpu.h"
#include "bsp_delay.h"
#include "arm_math.h"
#include <stdint.h>
/* ---------------------------结构体实例化----------------------------------------------- */
service_gyroff_t    gyroff={0};
service_angle_t     angle={0};
service_show_t      show={0};
volatile uint8_t show_flag=0;
/* ---------------------------MPU陀螺仪校准----------------------------------------------- */
static void MPU_GyroCalibrate(uint16_t sample_num)
{
    float sum_gx=0,sum_gy=0,sum_gz=0;
    uint16_t i=sample_num;
    while(i--)
    {
        MPU_RD_Raw_Data();
        sum_gx+=raw_data.gx;
        sum_gy+=raw_data.gy;
        sum_gz+=raw_data.gz;
        DWT_delay_ms(8);
    }
    gyroff.gx_off=sum_gx/sample_num;
    gyroff.gy_off=sum_gy/sample_num;
    gyroff.gz_off=sum_gz/sample_num;
}
void MPU_Service_Init()
{
    MPU_Moudle_Init();
    MPU_GyroCalibrate(200);
}
/* ---------------------------MPU数据姿态解算----------------------------------------------- */
static void MPU_CompFilter(float dt)
{
    float ax,ay,az;
    float gx,gy,gz;
    float acc_rol,acc_pitch;
    const float alpha=0.9f;

    ax=raw_data.ax/ACCEL_SCALE;
    ay=raw_data.ay/ACCEL_SCALE;
    az=raw_data.az/ACCEL_SCALE;
    gx=(raw_data.gx-gyroff.gx_off)/GYRO_SCALE;
    gy=(raw_data.gy-gyroff.gy_off)/GYRO_SCALE;
    gz=(raw_data.gz-gyroff.gz_off)/GYRO_SCALE;
    //只用加速度计计算静态倾角，atan2输出弧度 *57.2958转角度(180/π)
    arm_atan2_f32(ay, az, &acc_rol);
    acc_rol *= 57.2958f;
    arm_atan2_f32(-ax, sqrtf(ay*ay + az*az), &acc_pitch);
    acc_pitch *= 57.2958f;
    //互补滤波融合：陀螺仪积分提供动态，加速度修正长期漂移
    angle.rol  = alpha * (angle.rol + gx * dt) + (1-alpha)*acc_rol;
    angle.pitch = alpha * (angle.pitch + gy * dt) + (1-alpha)*acc_pitch;
    angle.yaw  += gz * dt;
}
void MPU_Updata()
{
    static float roll_sum=0.0f;
    static float pitch_sum=0.0f;
    static float dt = 0.0f;
    static uint8_t cnt=0; 
    static uint32_t last_tick = 0;
    if (mpu_flag==1) 
    {
        mpu_flag = 0;
        uint32_t now_tick=DWT_get_tick();
        dt=DWT_Deltas(now_tick, last_tick);
        last_tick=now_tick;
        MPU_RD_Raw_Data();
        MPU_CompFilter(dt);
        MPU_Clear_SATUS();
        roll_sum += angle.rol;
        pitch_sum += angle.pitch;
        cnt++;
        if(cnt>=AVG_NUM)
        {
            show_flag=1;
            show.rol=roll_sum/AVG_NUM;
            show.pitch=pitch_sum/AVG_NUM;
            roll_sum=0;
            pitch_sum=0;
            cnt=0;
        }
    } 
}