#ifndef __USART_H
#define __USART_H

#include "stm32f4xx.h"
#include "global_declare.h"
#define buf_size 200
void usart2_init(u32 bound);
void usart3_init(u32 bound);
void uart4_init(u32 bound);
#endif

