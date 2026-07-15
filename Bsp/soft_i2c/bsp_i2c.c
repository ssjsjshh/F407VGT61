#include "bsp_i2c.h"
#include "bsp_delay.h"
/* ---------------------------i2c实例化----------------------------------------------- */
bsp_i2c_t bsp_i2c1={I2C1_SDA_GPIO_Port,I2C1_SCL_GPIO_Port,I2C1_SDA_Pin,I2C1_SCL_Pin,DWT_delay_ns,500};
bsp_i2c_t bsp_i2c2={I2C2_SDA_GPIO_Port,I2C2_SCL_GPIO_Port,I2C2_SDA_Pin,I2C2_SCL_Pin,DWT_delay_ns,500};
/* ---------------------------i2c延时----------------------------------------------- */
static inline void i2c_delay(bsp_i2c_t *i2c)
{
    i2c->delay(i2c->delay_parm);
}
/* ---------------------------i2c起始信号----------------------------------------------- */
void i2c_start(bsp_i2c_t *i2c)
{
    SDA_H(i2c);
    SCL_H(i2c);
    i2c_delay(i2c);
    SDA_L(i2c);
    i2c_delay(i2c);
    //拉低scl，准备数据
    SCL_L(i2c);
    i2c_delay(i2c);
}
/* ---------------------------i2c停止信号----------------------------------------------- */
void i2c_stop(bsp_i2c_t *i2c)
{
    SCL_L(i2c);
    SDA_L(i2c);
    i2c_delay(i2c);
    SCL_H(i2c);
    i2c_delay(i2c);
    SDA_H(i2c);
    i2c_delay(i2c);  
}
/* ---------------------------i2c发送1字节数据----------------------------------------------- */
uint8_t i2c_send_byte(bsp_i2c_t *i2c,uint8_t byte)
{
    for(uint8_t i=0;i<8;i++)
    {
        SCL_L(i2c);
        if(byte&0x80)
        SDA_H(i2c);
        else
        SDA_L(i2c);
        byte<<=1;
        i2c_delay(i2c);
        SCL_H(i2c);
        i2c_delay(i2c);
    }
    //读取第九个时钟的ACK/NACK
    SCL_L(i2c);i2c_delay(i2c);
    //释放SDA,让从机拉低ACK或保持(NACK)
    SDA_H(i2c);i2c_delay(i2c);
    SCL_H(i2c);i2c_delay(i2c);
    uint8_t ACK=SDA_READ(i2c);
    SCL_L(i2c);i2c_delay(i2c);
    //返回0表示成功
    return ACK;
}
/* ---------------------------i2c读取1字节数据----------------------------------------------- */
uint8_t i2c_read_byte(bsp_i2c_t *i2c,uint8_t ack_nack)
{
    uint8_t byte=0;
    SDA_H(i2c);
    for(uint8_t i=0;i<8;i++)
    {
        byte<<=1;
        SCL_L(i2c);
        i2c_delay(i2c);
        SCL_H(i2c);
        if(SDA_READ(i2c))
        byte|=0X01;
        i2c_delay(i2c);   
    }
    //发送第九个时钟的ACK/NACK给从机
    SCL_L(i2c);
    i2c_delay(i2c);
    if (ack_nack)
    SDA_H(i2c);
    else
    SDA_L(i2c);
    i2c_delay(i2c);
    SCL_H(i2c);
    i2c_delay(i2c);
    SCL_L(i2c);
    i2c_delay(i2c);
    //释放总线
    SDA_H(i2c);
    return byte;
}