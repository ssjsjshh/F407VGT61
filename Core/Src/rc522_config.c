#include "rc522_config.h"
#include "rc522.h"
#include "usart.h"
#include <string.h>
static RC522Card_t card={0};
/* ---------------------------RC522读卡----------------------------------------------- */
void Find_Card_Task(void)
{
    uint8_t card_type[2] = {0};
    uint8_t card_id[4] = {0};
    
    if (PcdRequest(card_type,PICC_REQIDL) == MI_OK) 
    {
        if (Pcd_Anticoll(card_id) == MI_OK) 
        {
            card.satuts=1;
            memcpy(card.type, card_type, 2);
            memcpy(card.uid, card_id, 4);
            Usart_Print(&TX1_handle, "找到卡！类型 %02X%02X 卡号 %02X%02X%02X%02X\r\n",
                        card_type[0], card_type[1],
                        card_id[0], card_id[1], card_id[2], card_id[3]);

            if (Pcd_Select(card_id) == MI_OK) 
            {   
                Pcd_Halt();  // 休眠
            }
        }
    }
}