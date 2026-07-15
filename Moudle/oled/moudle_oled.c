#include "moudle_oled.h"
#include "oled_priv.h"
#include "bsp_delay.h"
#include <string.h>
/* OLED写单字节命令 */
void oled_wr_cmd(uint8_t dat)
{
    i2c_start(&I2C_OLED);
    i2c_send_byte(&I2C_OLED,OLED_ADDR);
    i2c_send_byte(&I2C_OLED,OLED_WR_ORDER);
    i2c_send_byte(&I2C_OLED,dat);
    i2c_stop(&I2C_OLED);
}
/* OLED写单字节数据 */
void oled_wr_dat(uint8_t dat)
{
    i2c_start(&I2C_OLED);
    i2c_send_byte(&I2C_OLED,OLED_ADDR);
    i2c_send_byte(&I2C_OLED,OLED_WR_DAT);
    i2c_send_byte(&I2C_OLED,dat);
    i2c_stop(&I2C_OLED);
}
/* OLED寻址 */
void oled_set_pos(uint8_t X,uint8_t y)
{
    oled_wr_cmd(0xb0|y);
    oled_wr_cmd(((X&0xf0)>>4)|0x10);
    oled_wr_cmd((X&0x0f));
}
/* OLED连续写入数据 */
void oled_wr_dat_burst(const uint8_t *dat,uint8_t len)
{
    if(len==0||dat==NULL||len>OLED_WIDTH)return;
    i2c_start(&I2C_OLED);
    i2c_send_byte(&I2C_OLED,OLED_ADDR);
    i2c_send_byte(&I2C_OLED,OLED_WR_DAT);
    for(uint8_t i=0;i<len;i++)
    {
        i2c_send_byte(&I2C_OLED,dat[i]);
    }
    i2c_stop(&I2C_OLED);
}
/* OLED连续写入固定值 */
void oled_fill_burst(uint8_t dat,uint8_t len)
{
    if(len == 0)return;
    i2c_start(&I2C_OLED);
    i2c_send_byte(&I2C_OLED,OLED_ADDR);
    i2c_send_byte(&I2C_OLED,OLED_WR_DAT);
    for(uint8_t i = 0; i < len; i++)
    {
        i2c_send_byte(&I2C_OLED,dat);
    }
}
/* OLED全屏写入固定值 */
void oled_fill(uint8_t Fill_dat)
{
    uint8_t m;
    for(m=0;m<8;m++)
    {
        oled_set_pos(0,m);
        oled_fill_burst(Fill_dat,OLED_WIDTH);
    }
}
