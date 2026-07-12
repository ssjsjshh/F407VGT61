#ifndef __RC522_CONFIG_H
#define __RC522_CONFIG_H
#include <stdint.h>
#define     TRY_COUNT_MAX           10
typedef struct {
    uint8_t type[2];
    uint8_t uid[4];
    uint8_t satuts;
    
} RC522Card_t;
void RC522_Init();
void Find_Card_Task(void);
#endif