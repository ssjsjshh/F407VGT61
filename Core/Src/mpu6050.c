#include "mpu6050.h"
#include <stdio.h>
#include <arm_math.h>
#include "delay.h"
#include "oled_api.h"
#include "usart.h"
#include "i2c.h"
/* ---------------------------MPU I2C句柄初始化----------------------------------------------- */
Soft_I2C_Typedef I2C_MPU={MPU_SDA_GPIO_Port,MPU_SCL_GPIO_Port,MPU_SDA_Pin,MPU_SCL_Pin};
/* ---------------------------结构体初始化----------------------------------------------- */
MPU_DataTypedef     MPU_Raw_Data={0};
MPU_GyroOffTypedef  MPU_GyroOff={0};
MPU_AngleTypedef    MPU_Angle={0};
/* ---------------------------变量初始化----------------------------------------------- */
volatile uint8_t MPU_Flag=0;
static char pitch_buf[10];
static char roll_buf[10];
/* ---------------------------MPU写寄存器----------------------------------------------- */
static uint8_t MPU_WR_REG(uint8_t reg,uint8_t data)
{
    uint8_t ack;
    I2C_Start(&I2C_MPU);
    ack=I2C_Send_Byte(&I2C_MPU, MPU_WR);
    if(ack)
    {
        I2C_Stop(&I2C_MPU);
        return 0XFF;
    }
    ack=I2C_Send_Byte(&I2C_MPU, reg);
    if(ack)
    {
        I2C_Stop(&I2C_MPU);
        return 0XFF;
    }
    ack=I2C_Send_Byte(&I2C_MPU, data);
    I2C_Stop(&I2C_MPU);
    return ack;
}
/* ---------------------------MPU读寄存器----------------------------------------------- */
static uint8_t MPU_RD_REG(uint8_t reg)
{
    uint8_t ack,val;
    I2C_Start(&I2C_MPU);
    ack=I2C_Send_Byte(&I2C_MPU, MPU_WR);
    if(ack)
    {
        I2C_Stop(&I2C_MPU);
        return 0XFF;
    }
    ack=I2C_Send_Byte(&I2C_MPU, reg);
    if(ack)
    {
        I2C_Stop(&I2C_MPU);
        return 0XFF;
    }
    I2C_Start(&I2C_MPU);
    ack=I2C_Send_Byte(&I2C_MPU, MPU_RD);
    if(ack)
    {
        I2C_Stop(&I2C_MPU);
        return 0XFF;
    }
    val=I2C_Read_Byte(&I2C_MPU, 1);
    I2C_Stop(&I2C_MPU);
    return val;
}
/* ---------------------------MPU读取全部数据----------------------------------------------- */
static void MPU_Read_AllData()
{
    uint8_t buf[14]={0};
    uint8_t ack,i;
    I2C_Start(&I2C_MPU);
    ack=I2C_Send_Byte(&I2C_MPU, MPU_WR);
     if(ack)
    {
        I2C_Stop(&I2C_MPU);
        return ;
    }
    ack=I2C_Send_Byte(&I2C_MPU, ACCEL_XOUT);
     if(ack)
    {
        I2C_Stop(&I2C_MPU);
        return ;
    }
    I2C_Start(&I2C_MPU);
    ack=I2C_Send_Byte(&I2C_MPU, MPU_RD);
    if(ack)
    {
        I2C_Stop(&I2C_MPU);
        return ;
    }
    for (i=0; i<13; i++) 
    {
        buf[i]=I2C_Read_Byte(&I2C_MPU, 0);

    }
    buf[13]=I2C_Read_Byte(&I2C_MPU, 1);
    I2C_Stop(&I2C_MPU);
    //数据对齐存入结构体
    MPU_Raw_Data.ax=(int16_t)(buf[0]<<8|buf[1]);
    MPU_Raw_Data.ay=(int16_t)(buf[2]<<8|buf[3]);
    MPU_Raw_Data.az=(int16_t)(buf[4]<<8|buf[5]);
    MPU_Raw_Data.temp=(int16_t)(buf[6]<<8|buf[7]);
    MPU_Raw_Data.gx=(int16_t)(buf[8]<<8|buf[9]);
    MPU_Raw_Data.gy=(int16_t)(buf[10]<<8|buf[11]);
    MPU_Raw_Data.gz=(int16_t)(buf[12]<<8|buf[13]);
}
/* ---------------------------MPU陀螺仪校准----------------------------------------------- */
static void MPU_GyroCalibrate(uint16_t sample_num)
{
    float sum_gx=0,sum_gy=0,sum_gz=0;
    uint16_t i=sample_num;
    while(i--)
    {
        MPU_Read_AllData();
        sum_gx+=MPU_Raw_Data.gx;
        sum_gy+=MPU_Raw_Data.gy;
        sum_gz+=MPU_Raw_Data.gz;
        DWT_delay_ms(8);
    }
    MPU_GyroOff.gx_off=sum_gx/sample_num;
    MPU_GyroOff.gy_off=sum_gy/sample_num;
    MPU_GyroOff.gz_off=sum_gz/sample_num;
    
}
/* ---------------------------MPU初始化----------------------------------------------- */
void MPU_Init()
{
    MPU_WR_REG( PWR_1, 0x00);
    MPU_WR_REG(SMPLRT_DIV, 0X07);
    MPU_WR_REG(CONFIG, 0X03);
    MPU_WR_REG(GYRO_CONFIG, 0X00);
    MPU_WR_REG(ACCEL_CONFIG, 0X00);
    MPU_WR_REG(INT_CONFIG, 0X20);
    MPU_WR_REG(INT_ENABLE, 0X01);
    MPU_GyroCalibrate(250);
    //重置中断状态，防卡死
    MPU_RD_REG(INT_STATUS);
    OLED_Clear_AreaBuf(0, 2, 108);
    OLED_Buf(0,2,"pitch:",1);
    OLED_Buf(0,3,"roll :",1);
}
/* ---------------------------MPU数据姿态解算----------------------------------------------- */
static void MPU_CompFilter(float dt)
{
    float ax,ay,az;
    float gx,gy,gz;
    float acc_rol,acc_pitch;
    const float alpha=0.9f;

    ax=MPU_Raw_Data.ax/ACCEL_SCALE;
    ay=MPU_Raw_Data.ay/ACCEL_SCALE;
    az=MPU_Raw_Data.az/ACCEL_SCALE;
    gx=(MPU_Raw_Data.gx-MPU_GyroOff.gx_off)/GYRO_SCALE;
    gy=(MPU_Raw_Data.gy-MPU_GyroOff.gy_off)/GYRO_SCALE;
    gz=(MPU_Raw_Data.gz-MPU_GyroOff.gz_off)/GYRO_SCALE;
    //只用加速度计计算静态倾角，atan2输出弧度 *57.2958转角度(180/π)
    arm_atan2_f32(ay, az, &acc_rol);
    acc_rol *= 57.2958f;
    arm_atan2_f32(-ax, sqrtf(ay*ay + az*az), &acc_pitch);
    acc_pitch *= 57.2958f;
    //互补滤波融合：陀螺仪积分提供动态，加速度修正长期漂移
    MPU_Angle.rol  = alpha * (MPU_Angle.rol + gx * dt) + (1-alpha)*acc_rol;
    MPU_Angle.pitch = alpha * (MPU_Angle.pitch + gy * dt) + (1-alpha)*acc_pitch;
    MPU_Angle.yaw  += gz * dt;
    
}
/* ---------------------------MPU数据更新显示----------------------------------------------- */
void MPU_Updata()
{

    static float roll_sum=0.0f;
    static float pitch_sum=0.0f;
    static float dt = 0.0f;
    static uint8_t cnt=0; 
    static uint32_t last_tick = 0;
    
    if (MPU_Flag == 1)
    {
        MPU_Flag = 0;
        uint32_t now_tick=DWT->CYCCNT;
        dt=(float)(now_tick-last_tick)/SystemCoreClock;
        last_tick=now_tick;
        MPU_Read_AllData();
        MPU_CompFilter(dt);
        MPU_RD_REG(INT_STATUS);
        //Usart_Print(&TX1_handle, "DEMO:%.2f , %.2f ,%.2f\r\n",MPU_Angle.pitch,MPU_Angle.rol,MPU_Angle.yaw);
        roll_sum += MPU_Angle.rol;
        pitch_sum += MPU_Angle.pitch;
        cnt++;
        if(cnt>=AVG_NUM)
        {
            float roll_avg  = roll_sum / AVG_NUM;
            float pitch_avg = pitch_sum / AVG_NUM;
            snprintf(roll_buf, sizeof(roll_buf), "%+07.2f", roll_avg);
            snprintf(pitch_buf, sizeof(pitch_buf), "%+07.2f", pitch_avg);
            OLED_Clear_AreaBuf(37, 2 ,10);
            OLED_Clear_AreaBuf(37, 3 ,10);
            OLED_Buf(37, 2, pitch_buf, 1);
            OLED_Buf(37, 3, roll_buf, 1);
            OLED_Refresh(2,4);
            roll_sum = 0;
            pitch_sum = 0;
            cnt = 0;    
        }
    }
}
