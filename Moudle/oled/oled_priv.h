#ifndef __OLED_PRIV_H
#define __OLED_PRIV_H
#include "bsp_i2c.h"
/* ---------------------------寄存器宏定义----------------------------------------------- */
#define     OLED_ADDR               0X78        //OLED地址
#define     OLED_WR_ORDER           0X00        //写命令控制字节
#define     OLED_WR_DAT             0X40        //写数据控制字节
#define     I2C_OLED                bsp_i2c1    //OLED绑定I2C总线
#endif