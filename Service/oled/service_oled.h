#ifndef __SERVICE_OLED_H
#define __SERVICE_OLED_H
#include <stdint.h>
/*统一上层接口*/
void oled_area_clear(uint8_t x,uint8_t y, uint16_t len,uint8_t size );
void oled_show(uint8_t x,uint8_t y,const char* str,uint8_t size);
void oled_clear_areabuf(uint8_t x, uint8_t y ,uint8_t width);
void oled_clear_buf();
void oled_buf(uint8_t x,uint8_t y,const char* str,uint8_t size);
void oled_refresh(uint8_t start,uint8_t end);
void oled_service_init();
void oled_draw(uint8_t x, uint8_t y,const char* str,uint8_t size);
#endif