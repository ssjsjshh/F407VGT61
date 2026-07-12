#include "moudle_oled.h"
#include "bsp_delay.h"
#include <string.h>
/* ---------------------------OLED写单字节命令----------------------------------------------- */
void OLED_WR_Cmd(uint8_t dat)
{
    I2C_Start(&I2C_OLED);
    I2C_Send_Byte(&I2C_OLED,OLED_ADDR);
    I2C_Send_Byte(&I2C_OLED,OLED_WR_ORDER);
    I2C_Send_Byte(&I2C_OLED,dat);
    I2C_Stop(&I2C_OLED);
}
/* ---------------------------OLED写单字节数据----------------------------------------------- */
void OLED_WR_Dat(uint8_t dat)
{
    I2C_Start(&I2C_OLED);
    I2C_Send_Byte(&I2C_OLED,OLED_ADDR);
    I2C_Send_Byte(&I2C_OLED,OLED_WR_DAT);
    I2C_Send_Byte(&I2C_OLED,dat);
    I2C_Stop(&I2C_OLED);
}
/* ---------------------------OLED寻址----------------------------------------------- */
void OLED_Set_Pos(uint8_t X,uint8_t y)
{
    OLED_WR_Cmd(0xb0|y);
    OLED_WR_Cmd(((X&0xf0)>>4)|0x10);
    OLED_WR_Cmd((X&0x0f));
}
/* ---------------------------OLED连续写入数据----------------------------------------------- */
void OLED_WR_Dat_Burst(const uint8_t *dat,uint8_t len)
{
    if(len==0||dat==NULL||len>OLED_WIDTH)return;
    I2C_Start(&I2C_OLED);
    I2C_Send_Byte(&I2C_OLED,OLED_ADDR);
    I2C_Send_Byte(&I2C_OLED,OLED_WR_DAT);
    for(uint8_t i=0;i<len;i++)
    {
        I2C_Send_Byte(&I2C_OLED,dat[i]);
    }
    I2C_Stop(&I2C_OLED);
}
/* ---------------------------OLED连续写入固定值----------------------------------------------- */
void OLED_Fill_Burst(uint8_t dat,uint8_t len)
{
    if(len == 0)return;
    I2C_Start(&I2C_OLED);
    I2C_Send_Byte(&I2C_OLED,OLED_ADDR);
    I2C_Send_Byte(&I2C_OLED,OLED_WR_DAT);
    for(uint8_t i = 0; i < len; i++)
    {
        I2C_Send_Byte(&I2C_OLED,dat);
    }
}
/* ---------------------------OLED全屏写入固定值----------------------------------------------- */
void OLED_Fill(uint8_t Fill_dat)
{
    uint8_t m;
    for(m=0;m<8;m++)
    {
        OLED_Set_Pos(0,m);
        OLED_Fill_Burst(Fill_dat,OLED_WIDTH);
    }
}
