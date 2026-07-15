#ifndef __BSP_RTC_H
#define __BSP_RTC_H
#include "stm32f4xx_hal.h"
#include <stdint.h>
/* ---------------------------对外变量声明----------------------------------------------- */
extern volatile uint8_t rtc_flag;
extern RTC_TimeTypeDef stime;
extern RTC_DateTypeDef sdate;
/* ---------------------------统一上层接口----------------------------------------------- */
void rtc_read();
#endif