#ifndef __SERVICE_OLED_H
#define __SERVICE_OLED_H
#include "bsp_i2c.h"
#include "oled_font.h"
#include <stdint.h>
/* ---------------------------屏幕尺寸参数----------------------------------------------- */
#define     OLED_WIDTH          128         //OLED宽字节数
#define     OLED_PAGES           8          //OLED页码数
/* ---------------------------寄存器宏定义----------------------------------------------- */
#define     OLED_ADDR               0X78        //OLED地址
#define     OLED_WR_ORDER           0X00        //写命令控制字节
#define     OLED_WR_DAT             0X40        //写数据控制字节
#define     I2C_OLED                BSP_I2C1        //OLED绑定I2C总线
/* ---------------------------统一上层接口----------------------------------------------- */
void OLED_Init();
void OLED_Fill(uint8_t Fill_dat);
void OLED_Clear(void);
void OLED_Show(uint8_t x,uint8_t y,const char* str,uint8_t size);
void OLED_Area_Clear(uint8_t x,uint8_t y, uint16_t len,uint8_t size );
void OLED_Clear_AreaBuf(uint8_t x, uint8_t y ,uint8_t width);
void OLED_Buf(uint8_t x,uint8_t y,const char* str,uint8_t size);
void OLED_Refresh(uint8_t start,uint8_t end);
#endif
