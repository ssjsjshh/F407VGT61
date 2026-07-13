#include "moudle_mpu.h"
#include "mpu_priv.h"
#include "bsp_exti.h"
#include <stdint.h>
/* ---------------------------变量结构体初始化----------------------------------------------- */
moudle_raw_data_t  raw_data={0};
volatile uint8_t mpu_flag=0;
/* ---------------------------MPU中断对应回调函数----------------------------------------------- */
static void MPU_EXTI_HANDLER()
{
    mpu_flag=1;
}
/* ---------------------------MPU写寄存器----------------------------------------------- */
uint8_t MPU_WR_REG(uint8_t reg,uint8_t data)
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
uint8_t MPU_RD_REG(uint8_t reg)
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
/* ---------------------------MPU读取全部原始数据----------------------------------------------- */
void MPU_RD_Raw_Data()
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
    raw_data.ax=(int16_t)(buf[0]<<8|buf[1]);
    raw_data.ay=(int16_t)(buf[2]<<8|buf[3]);
    raw_data.az=(int16_t)(buf[4]<<8|buf[5]);
    raw_data.temp=(int16_t)(buf[6]<<8|buf[7]);
    raw_data.gx=(int16_t)(buf[8]<<8|buf[9]);
    raw_data.gy=(int16_t)(buf[10]<<8|buf[11]);
    raw_data.gz=(int16_t)(buf[12]<<8|buf[13]);
}
/* ---------------------------MPU清除中断标志位----------------------------------------------- */
void MPU_Clear_SATUS()
{
    MPU_RD_REG(INT_STATUS);
}/* ---------------------------MPU初始化----------------------------------------------- */
void MPU_Moudle_Init()
{
    MPU_WR_REG( PWR_1, 0x00);
    MPU_WR_REG(SMPLRT_DIV, 0X07);
    MPU_WR_REG(CONFIG, 0X03);
    MPU_WR_REG(GYRO_CONFIG, 0X00);
    MPU_WR_REG(ACCEL_CONFIG, 0X00);
    MPU_WR_REG(INT_CONFIG, 0X20);
    MPU_WR_REG(INT_ENABLE, 0X01);
    //重置中断状态，防卡死
    MPU_RD_REG(INT_STATUS);
    bsp_exti_register(MPU_INT_PIN, MPU_EXTI_HANDLER);
}