#include "esp32.h"
#include "usart.h"
void Connect_Cloud()
{
    Usart_Print(&TX3_handle, "AT+ATKCLDSTA=\"20755178682121420805\",\"12345678\"\r\n");
}
void Close_Cloud()
{
    Usart_Print(&TX3_handle, "AT+ATKCLDCLS\r\n");
}
