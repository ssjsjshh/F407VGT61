#ifndef __MOUDLE_OLED_H
#define __MOUDLE_OLED_H
#include <stdint.h>
/* 对外声明变量 */
#define         OLED_WIDTH          128
#define         OLED_PAGES          8
/* 统一上层接口 */
void oled_wr_cmd(uint8_t dat);
void oled_wr_dat(uint8_t dat);
void oled_set_pos(uint8_t x,uint8_t y);
void oled_wr_dat_burst(const uint8_t *dat,uint8_t len);
void oled_fill_burst(uint8_t dat,uint8_t len);
void oled_fill(uint8_t fill_dat);
#endif
