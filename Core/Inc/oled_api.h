#ifndef __OLED_API_H__
#define __OLED_API_H__
#include <stdint.h>
#include "oled_font.h"
/* ---------------------------屏幕尺寸参数----------------------------------------------- */
#define     OLED_WIDTH          128         //OLED宽字节数
#define     OLED_PAGES           8          //OLED页码数
/* ---------------------------统一上层接口----------------------------------------------- */
void OLED_Init();
void OLED_Fill(uint8_t Fill_dat);
void OLED_Clear(void);
void OLED_Show(uint8_t x,uint8_t y,const char* str,uint8_t size);
void OLED_Area_Clear(uint8_t x,uint8_t y, uint16_t len,uint8_t size );
void OLED_Clear_AreaBuf(uint8_t x, uint8_t y ,uint8_t width);
void OLED_Buf(uint8_t x,uint8_t y,const char* str,uint8_t size);
void OLED_Refresh(uint8_t start,uint8_t end);
void OLED_Init_Frame();
#endif