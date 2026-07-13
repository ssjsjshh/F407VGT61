#ifndef __SERVICE_OLED_H
#define __SERVICE_OLED_H
#include "oled_font.h"
#include <stdint.h>
/* ---------------------------统一上层接口----------------------------------------------- */
void OLED_Area_Clear(uint8_t x,uint8_t y, uint16_t len,uint8_t size );
void OLED_Show(uint8_t x,uint8_t y,const char* str,uint8_t size);
void OLED_Clear_AreaBuf(uint8_t x, uint8_t y ,uint8_t width);
void OLED_Clear_Buf(void);
void OLED_Buf(uint8_t x,uint8_t y,const char* str,uint8_t size);
void OLED_Refresh(uint8_t start,uint8_t end);
void OLED_Init(void);
#endif