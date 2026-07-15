#include "service_oled.h"
#include "moudle_oled.h"
#include "oled_font.h"
#include "bsp_delay.h"
#include <stdint.h>
#include <string.h>
/* 1KB刷新缓存 */
static uint8_t oled_gram[OLED_PAGES][OLED_WIDTH]={0};
/* OLED局部刷新清除 */
void oled_area_clear(uint8_t x,uint8_t y, uint16_t len,uint8_t size )
{
    if(x>=OLED_WIDTH||y>=OLED_PAGES)return;
    switch (size) 
    {
        case 1:
        {
            oled_set_pos(x, y);
            oled_fill_burst(0x00,len);
        }
        break;
        case 2:
        {
            oled_set_pos(x, y);
            oled_fill_burst(0x00,len);
            if(y==OLED_PAGES-1)return;
            oled_set_pos(x, y+1);
            oled_fill_burst(0x00,len);
        }
    }
}
/* OLED刷新显示 */
void oled_show(uint8_t x,uint8_t y,const char* str,uint8_t size)
{
    if(x>=OLED_WIDTH||y>=OLED_PAGES)return;
    uint8_t i=0,ch=0;
    switch(size) 
    {
        case 1://6x8小字
        {
            while(str[i]!='\0')
            {
                if(str[i]<32 || str[i]>126){i++;continue;}
                ch=str[i]-32;
                if(x>OLED_WIDTH-6)
                {
                    x=0;
                    y++;
                }
                if(y>=8) break;
                oled_set_pos(x,y);
                oled_wr_dat_burst(asc2_0608[ch],6);
                x+=6;
                i++;
            }
        }
        break;
        case 2://8x12大字
        {
            while(str[i]!='\0')
            {
                if(str[i]<32 || str[i]>126){i++;continue;}
                ch=str[i]-32;
                if(x>OLED_WIDTH-8)
                {
                    x=0;
                    y++;
                }
                if(y+1>=8) break;
                oled_set_pos(x,y);
                oled_wr_dat_burst(&asc2_0812[ch*16],8);
                oled_set_pos(x,y+1);
                oled_wr_dat_burst(&asc2_0812[ch*16+8],8);
                x+=8;
                i++;
            }
        }
        break;
    }
}
/* OLED缓存局部清除 */
void oled_clear_areabuf(uint8_t x, uint8_t y ,uint8_t width)
{
    if(x>=OLED_WIDTH||y>=OLED_PAGES)return;
    memset(&oled_gram[y][x], 0x00, width);
}
/* OLED缓存全部清除 */
void oled_clear_buf()
{
    memset(oled_gram, 0x00, sizeof(oled_gram));
}
/* OLED缓存更新 */
void oled_buf(uint8_t x,uint8_t y,const char* str,uint8_t size)
{
    if(x>=OLED_WIDTH||y>=OLED_PAGES)return;
    if (str == NULL) return;
    uint8_t i = 0, ch = 0;
    while (str[i] != '\0')
    {
        if (str[i] < 32 || str[i] > 126) { i++; continue; }
        ch = str[i] - 32;
        if (size == 1)
        {
            if (x > OLED_WIDTH - 6) { x = 0; y++; }
            if (y >= OLED_PAGES) break;
            for (uint8_t j = 0; j < 6; j++)
                oled_gram[y][x + j] = asc2_0608[ch][j];
            x += 6;
        }
        else if (size == 2)
        {
            if (x > OLED_WIDTH - 8) { x = 0; y += 2; }
            if (y + 1 >= OLED_PAGES) break;
            for (uint8_t j = 0; j < 8; j++)
            {
                oled_gram[y][x + j]     = asc2_0812[ch * 16 + j];
                oled_gram[y + 1][x + j] = asc2_0812[ch * 16 + j + 8];
            }
            x += 8;
        }
        i++;
    }
}
/* OLED缓存刷新显示 */
void oled_refresh(uint8_t start,uint8_t end)
{
    if(start>OLED_PAGES)return;
    for (uint8_t page = start; page < end; page++)
    {
        oled_set_pos(0, page);
        oled_wr_dat_burst(oled_gram[page], OLED_WIDTH);
    }
}
/* OLED初始化 */
void oled_service_init()
{
    DWT_delay_ms(100);

    oled_wr_cmd(0xAE);
    oled_wr_cmd(0x00); 
    oled_wr_cmd(0x10); 
    oled_wr_cmd(0xB0); 
    oled_wr_cmd(0x40); 
    oled_wr_cmd(0x81); 
    oled_wr_cmd(0xFF); 
    oled_wr_cmd(0xA1); 
    oled_wr_cmd(0xA6); 
    oled_wr_cmd(0xA8); 
    oled_wr_cmd(0x3F); 
    oled_wr_cmd(0xC8); 
    oled_wr_cmd(0xD3); 
    oled_wr_cmd(0x00);
    oled_wr_cmd(0xD5); 
    oled_wr_cmd(0x80);
    oled_wr_cmd(0xD9); 
    oled_wr_cmd(0xF1);
    oled_wr_cmd(0xDA); 
    oled_wr_cmd(0x12);
    oled_wr_cmd(0xDB); 
    oled_wr_cmd(0x40);
    oled_wr_cmd(0x8D); 
    oled_wr_cmd(0x14);
    oled_wr_cmd(0xAF);
    oled_fill(0x00);
}
void oled_draw(uint8_t x, uint8_t y,const char* str,uint8_t size)
{
    if (str == NULL || *str == '\0')
    oled_clear_areabuf(x,y,OLED_WIDTH);
    uint16_t len=strlen(str);          
    if (x>=OLED_WIDTH||y>=OLED_PAGES) return;
    switch (size) 
    {
        case 1:
        {
            oled_clear_areabuf(x,y,len);
            oled_buf(x, y, str, 1);
            oled_refresh(y,y+1);
        }
        break;
        case 2:
        {
            oled_clear_areabuf(x,y,len);
            if(y<7)
            oled_clear_areabuf(x,y+1,len);
            oled_buf(x, y, str, size);
            oled_refresh(y,y+(size==1?1:2));
        }
        break;
    }
}