#include "moudle_mpu.h"
#include "mpu_priv.h"
#include "bsp_exti.h"
#include <stdint.h>
/* 变量结构体初始化 */
moudle_raw_data_t  raw_data={0};
volatile uint8_t mpu_flag=0;
/* MPU中断对应回调函数 */
static void mpu_exti_handler()
{
    mpu_flag=1;
}
/* MPU写寄存器 */
uint8_t mpu_wr_reg(uint8_t reg,uint8_t data)
{
    uint8_t ack;
    i2c_start(&I2C_MPU);
    ack=i2c_send_byte(&I2C_MPU, MPU_WR);
    if(ack)
    {
        i2c_stop(&I2C_MPU);
        return 0XFF;
    }
    ack=i2c_send_byte(&I2C_MPU, reg);
    if(ack)
    {
        i2c_stop(&I2C_MPU);
        return 0XFF;
    }
    ack=i2c_send_byte(&I2C_MPU, data);
    i2c_stop(&I2C_MPU);
    return ack;
}
/* MPU读寄存器 */
uint8_t mpu_rd_reg(uint8_t reg)
{
    uint8_t ack,val;
    i2c_start(&I2C_MPU);
    ack=i2c_send_byte(&I2C_MPU, MPU_WR);
    if(ack)
    {
        i2c_stop(&I2C_MPU);
        return 0XFF;
    }
    ack=i2c_send_byte(&I2C_MPU, reg);
    if(ack)
    {
        i2c_stop(&I2C_MPU);
        return 0XFF;
    }
    i2c_start(&I2C_MPU);
    ack=i2c_send_byte(&I2C_MPU, MPU_RD);
    if(ack)
    {
        i2c_stop(&I2C_MPU);
        return 0XFF;
    }
    val=i2c_read_byte(&I2C_MPU, 1);
    i2c_stop(&I2C_MPU);
    return val;
}
/* MPU读取全部原始数据 */
void mpu_rd_raw_data()
{
    uint8_t buf[14]={0};
    uint8_t ack,i;
    i2c_start(&I2C_MPU);
    ack=i2c_send_byte(&I2C_MPU, MPU_WR);
     if(ack)
    {
        i2c_stop(&I2C_MPU);
        return ;
    }
    ack=i2c_send_byte(&I2C_MPU, ACCEL_XOUT);
     if(ack)
    {
        i2c_stop(&I2C_MPU);
        return ;
    }
    i2c_start(&I2C_MPU);
    ack=i2c_send_byte(&I2C_MPU, MPU_RD);
    if(ack)
    {
        i2c_stop(&I2C_MPU);
        return ;
    }
    for (i=0; i<13; i++) 
    {
        buf[i]=i2c_read_byte(&I2C_MPU, 0);

    }
    buf[13]=i2c_read_byte(&I2C_MPU, 1);
    i2c_stop(&I2C_MPU);
    //数据对齐存入结构体
    raw_data.ax=(int16_t)(buf[0]<<8|buf[1]);
    raw_data.ay=(int16_t)(buf[2]<<8|buf[3]);
    raw_data.az=(int16_t)(buf[4]<<8|buf[5]);
    raw_data.temp=(int16_t)(buf[6]<<8|buf[7]);
    raw_data.gx=(int16_t)(buf[8]<<8|buf[9]);
    raw_data.gy=(int16_t)(buf[10]<<8|buf[11]);
    raw_data.gz=(int16_t)(buf[12]<<8|buf[13]);
}
/* MPU清除中断标志位 */
void mpu_clear_satus()
{
    mpu_rd_reg(INT_STATUS);
}/* MPU初始化 */
void mpu_moudle_init()
{
    mpu_wr_reg( PWR_1, 0x00);
    mpu_wr_reg(SMPLRT_DIV, 0X07);
    mpu_wr_reg(CONFIG, 0X03);
    mpu_wr_reg(GYRO_CONFIG, 0X00);
    mpu_wr_reg(ACCEL_CONFIG, 0X00);
    mpu_wr_reg(INT_CONFIG, 0X20);
    mpu_wr_reg(INT_ENABLE, 0X01);
    //重置中断状态，防卡死
    mpu_rd_reg(INT_STATUS);
    bsp_exti_register(MPU_INT_PIN, mpu_exti_handler);
}