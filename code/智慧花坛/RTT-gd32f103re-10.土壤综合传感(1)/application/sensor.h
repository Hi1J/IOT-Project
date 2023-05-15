#ifndef __SENSOR_H
#define __SENSOR_H

#include <app.h>

#define Sensor_rx_buf UART3_RX_BUF

#define Sensor_send_data UART3_Send_Data




extern void Take_A_Data(uint16_t Type);


#endif 

