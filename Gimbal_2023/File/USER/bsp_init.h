#ifndef __BSP_INIT_H__
#define __BSP_INIT_H__
#include "tim.h"
#include "can.h"
#include "spi.h"
#include "nvic.h"
#include "gpio.h"
#include "usart.h"
#include "systick.h"
#include "bmi088_driver.h"

void BSP_Init(void);
#endif
