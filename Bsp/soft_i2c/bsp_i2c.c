#include "bsp_i2c.h"
#include "bsp_delay.h"
/* ---------------------------I2C实例化----------------------------------------------- */
bsp_i2c_t BSP_I2C1={I2C1_SDA_GPIO_Port,I2C1_SCL_GPIO_Port,I2C1_SDA_Pin,I2C1_SCL_Pin,DWT_delay_ns,500};
bsp_i2c_t BSP_I2C2={I2C2_SDA_GPIO_Port,I2C2_SCL_GPIO_Port,I2C2_SDA_Pin,I2C2_SCL_Pin,DWT_delay_ns,500};
/* ---------------------------I2C延时----------------------------------------------- */
static inline void I2C_DELAY(bsp_i2c_t *I2C)
{
    I2C->delay(I2C->delay_parm);
}
/* ---------------------------I2C起始信号----------------------------------------------- */
void I2C_Start(bsp_i2c_t *I2C)
{
    SDA_H(I2C);
    SCL_H(I2C);
    I2C_DELAY(I2C);
    SDA_L(I2C);
    I2C_DELAY(I2C);
    //拉低scl，准备数据
    SCL_L(I2C);
    I2C_DELAY(I2C);
}
/* ---------------------------I2C停止信号----------------------------------------------- */
void I2C_Stop(bsp_i2c_t *I2C)
{
    SCL_L(I2C);
    SDA_L(I2C);
    I2C_DELAY(I2C);
    SCL_H(I2C);
    I2C_DELAY(I2C);
    SDA_H(I2C);
    I2C_DELAY(I2C);  
}
/* ---------------------------I2C发送1字节数据----------------------------------------------- */
uint8_t I2C_Send_Byte(bsp_i2c_t *I2C,uint8_t byte)
{
    for(uint8_t i=0;i<8;i++)
    {
        SCL_L(I2C);
        if(byte&0x80)
        SDA_H(I2C);
        else
        SDA_L(I2C);
        byte<<=1;
        I2C_DELAY(I2C);
        SCL_H(I2C);
        I2C_DELAY(I2C);
    }
    //读取第九个时钟的ACK/NACK
    SCL_L(I2C);I2C_DELAY(I2C);
    //释放SDA,让从机拉低ACK或保持(NACK)
    SDA_H(I2C);I2C_DELAY(I2C);
    SCL_H(I2C);I2C_DELAY(I2C);
    uint8_t ACK=SDA_READ(I2C);
    SCL_L(I2C);I2C_DELAY(I2C);
    //返回0表示成功
    return ACK;
}
/* ---------------------------I2C读取1字节数据----------------------------------------------- */
uint8_t I2C_Read_Byte(bsp_i2c_t *I2C,uint8_t ack_nack)
{
    uint8_t byte=0;
    SDA_H(I2C);
    for(uint8_t i=0;i<8;i++)
    {
        byte<<=1;
        SCL_L(I2C);
        I2C_DELAY(I2C);
        SCL_H(I2C);
        if(SDA_READ(I2C))
        byte|=0X01;
        I2C_DELAY(I2C);   
    }
    //发送第九个时钟的ACK/NACK给从机
    SCL_L(I2C);
    I2C_DELAY(I2C);
    if (ack_nack)
    SDA_H(I2C);
    else
    SDA_L(I2C);
    I2C_DELAY(I2C);
    SCL_H(I2C);
    I2C_DELAY(I2C);
    SCL_L(I2C);
    I2C_DELAY(I2C);
    //释放总线
    SDA_H(I2C);
    return byte;
}