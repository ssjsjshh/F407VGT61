#include "service_oled.h"
#include "moudle_oled.h"
#include "bsp_delay.h"
#include <string.h>
/* ---------------------------1KB刷新缓存----------------------------------------------- */
static uint8_t OLED_GRAM[OLED_PAGES][OLED_WIDTH]={0};
/* ---------------------------OLED局部刷新清除----------------------------------------------- */
void OLED_Area_Clear(uint8_t x,uint8_t y, uint16_t len,uint8_t size )
{
    if(x>=OLED_WIDTH||y>=OLED_PAGES)return;
    switch (size) 
    {
        case 1:
        {
            OLED_Set_Pos(x, y);
            OLED_Fill_Burst(0x00,len);
        }
        break;
        case 2:
        {
            OLED_Set_Pos(x, y);
            OLED_Fill_Burst(0x00,len);
            if(y==OLED_PAGES-1)return;
            OLED_Set_Pos(x, y+1);
            OLED_Fill_Burst(0x00,len);
        }
    }
}
/* ---------------------------OLED刷新显示----------------------------------------------- */
void OLED_Show(uint8_t x,uint8_t y,const char* str,uint8_t size)
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
                OLED_Set_Pos(x,y);
                OLED_WR_Dat_Burst(asc2_0608[ch],6);
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
                OLED_Set_Pos(x,y);
                OLED_WR_Dat_Burst(&asc2_0812[ch*16],8);
                OLED_Set_Pos(x,y+1);
                OLED_WR_Dat_Burst(&asc2_0812[ch*16+8],8);
                x+=8;
                i++;
            }
        }
        break;
    }
}
/* ---------------------------OLED缓存局部清除----------------------------------------------- */
void OLED_Clear_AreaBuf(uint8_t x, uint8_t y ,uint8_t width)
{
    if(x>=OLED_WIDTH||y>=OLED_PAGES)return;
    memset(&OLED_GRAM[y][x], 0x00, width);
}
/* ---------------------------OLED缓存全部清除----------------------------------------------- */
void OLED_Clear_Buf(void)
{
    memset(OLED_GRAM, 0x00, sizeof(OLED_GRAM));
}
/* ---------------------------OLED缓存更新----------------------------------------------- */
void OLED_Buf(uint8_t x,uint8_t y,const char* str,uint8_t size)
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
                OLED_GRAM[y][x + j] = asc2_0608[ch][j];
            x += 6;
        }
        else if (size == 2)
        {
            if (x > OLED_WIDTH - 8) { x = 0; y += 2; }
            if (y + 1 >= OLED_PAGES) break;
            for (uint8_t j = 0; j < 8; j++)
            {
                OLED_GRAM[y][x + j]     = asc2_0812[ch * 16 + j];
                OLED_GRAM[y + 1][x + j] = asc2_0812[ch * 16 + j + 8];
            }
            x += 8;
        }
        i++;
    }
}
/* ---------------------------OLED缓存刷新显示----------------------------------------------- */
void OLED_Refresh(uint8_t start,uint8_t end)
{
    if(start>OLED_PAGES)return;
    for (uint8_t page = start; page < end; page++)
    {
        OLED_Set_Pos(0, page);
        OLED_WR_Dat_Burst(OLED_GRAM[page], OLED_WIDTH);
    }
}
/* ---------------------------OLED初始化----------------------------------------------- */
void OLED_Init(void)
{
    DWT_delay_ms(100);

    OLED_WR_Cmd(0xAE);
    OLED_WR_Cmd(0x00); 
    OLED_WR_Cmd(0x10); 
    OLED_WR_Cmd(0xB0); 
    OLED_WR_Cmd(0x40); 
    OLED_WR_Cmd(0x81); 
    OLED_WR_Cmd(0xFF); 
    OLED_WR_Cmd(0xA1); 
    OLED_WR_Cmd(0xA6); 
    OLED_WR_Cmd(0xA8); 
    OLED_WR_Cmd(0x3F); 
    OLED_WR_Cmd(0xC8); 
    OLED_WR_Cmd(0xD3); 
    OLED_WR_Cmd(0x00);
    OLED_WR_Cmd(0xD5); 
    OLED_WR_Cmd(0x80);
    OLED_WR_Cmd(0xD9); 
    OLED_WR_Cmd(0xF1);
    OLED_WR_Cmd(0xDA); 
    OLED_WR_Cmd(0x12);
    OLED_WR_Cmd(0xDB); 
    OLED_WR_Cmd(0x40);
    OLED_WR_Cmd(0x8D); 
    OLED_WR_Cmd(0x14);
    OLED_WR_Cmd(0xAF);
    OLED_Fill(0x00);
}