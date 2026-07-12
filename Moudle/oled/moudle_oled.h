#ifndef __MOUDLE_OLED_H
#define __MOUDLE_OLED_H
#include "bsp_i2c.h"
#include <stdint.h>
/* ---------------------------屏幕尺寸参数----------------------------------------------- */
#define     OLED_WIDTH          128         //OLED宽字节数
#define     OLED_PAGES           8          //OLED页码数
/* ---------------------------寄存器宏定义----------------------------------------------- */
#define     OLED_ADDR               0X78        //OLED地址
#define     OLED_WR_ORDER           0X00        //写命令控制字节
#define     OLED_WR_DAT             0X40        //写数据控制字节
#define     I2C_OLED                BSP_I2C1    //OLED绑定I2C总线
/* ---------------------------统一上层接口----------------------------------------------- */
void OLED_WR_Cmd(uint8_t dat);
void OLED_WR_Dat(uint8_t dat);
void OLED_Set_Pos(uint8_t X,uint8_t y);
void OLED_WR_Dat_Burst(const uint8_t *dat,uint8_t len);
void OLED_Fill_Burst(uint8_t dat,uint8_t len);
void OLED_Fill(uint8_t Fill_dat);
#endif
