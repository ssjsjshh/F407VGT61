#include "i2c.h"
/* ---------------------------I2C起始信号----------------------------------------------- */
void I2C_Start(Soft_I2C_Typedef *I2C)//I2C起始信号
{
    SDA_H(I2C);
    SCL_H(I2C);
    I2C_DELAY();
    SDA_L(I2C);
    I2C_DELAY();
    SCL_L(I2C);//拉低scl，准备数据
    I2C_DELAY();
}
/* ---------------------------I2C停止信号----------------------------------------------- */
void I2C_Stop(Soft_I2C_Typedef *I2C)
{
    SCL_L(I2C);
    SDA_L(I2C);
    I2C_DELAY();
    SCL_H(I2C);
    I2C_DELAY();
    SDA_H(I2C);
    I2C_DELAY();  
}
/* ---------------------------I2C发送1字节数据----------------------------------------------- */
uint8_t I2C_Send_Byte(Soft_I2C_Typedef *I2C,uint8_t byte)
{
    for(uint8_t i=0;i<8;i++)
    {
        SCL_L(I2C);
        if(byte&0x80)
        SDA_H(I2C);
        else
        SDA_L(I2C);
        byte<<=1;
        I2C_DELAY();
        SCL_H(I2C);
        I2C_DELAY();
    }
    //读取第九个时钟的ACK/NACK
    SCL_L(I2C);I2C_DELAY();
    //释放SDA,让从机拉低ACK或保持(NACK)
    SDA_H(I2C);I2C_DELAY();
    SCL_H(I2C);I2C_DELAY();
    uint8_t ACK=SDA_READ(I2C);
    SCL_L(I2C);I2C_DELAY();
    //返回0表示成功
    return ACK;
}
/* ---------------------------I2C读取1字节数据----------------------------------------------- */
uint8_t I2C_Read_Byte(Soft_I2C_Typedef *I2C,uint8_t ack_nack)
{
    uint8_t byte=0;
    SDA_H(I2C);
    for(uint8_t i=0;i<8;i++)
    {
        byte<<=1;
        SCL_L(I2C);
        I2C_DELAY();
        SCL_H(I2C);
        if(SDA_READ(I2C))
        byte|=0X01;
        I2C_DELAY();   
    }
    //发送第九个时钟的ACK/NACK给从机
    SCL_L(I2C);
    I2C_DELAY();
    if (ack_nack)
    SDA_H(I2C);
    else
    SDA_L(I2C);
    I2C_DELAY();
    SCL_H(I2C);
    I2C_DELAY();
    SCL_L(I2C);
    I2C_DELAY();
    //释放总线
    SDA_H(I2C);
    return byte;
}