#ifndef __MOUDLE_OLED_H
#define __MOUDLE_OLED_H
#include <stdint.h>
/* ---------------------------屏幕尺寸参数----------------------------------------------- */
#define     OLED_WIDTH          128         //OLED宽字节数
#define     OLED_PAGES           8          //OLED页码数
/* ---------------------------统一上层接口----------------------------------------------- */
void OLED_WR_Cmd(uint8_t dat);
void OLED_WR_Dat(uint8_t dat);
void OLED_Set_Pos(uint8_t X,uint8_t y);
void OLED_WR_Dat_Burst(const uint8_t *dat,uint8_t len);
void OLED_Fill_Burst(uint8_t dat,uint8_t len);
void OLED_Fill(uint8_t Fill_dat);
#endif
