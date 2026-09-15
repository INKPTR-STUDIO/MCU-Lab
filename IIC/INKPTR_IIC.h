#ifndef INKPTR_SoftIIC_H
#define INKPTR_SoftIIC_H

/*================================================================================================*/
// Part 1: Library and interface function declarations
#include "ch32v00x.h"
void    SoftIIC_Init(void);
void    SoftIIC_Start(void);
void    SoftIIC_Stop(void);
void    SoftIIC_SendByte(uint8_t SendByte);
uint8_t SoftIIC_ReceiveACK(void);
uint8_t SoftIIC_ReceiveByte(void);
void    SoftIIC_SendACK(uint8_t SendACK);

/*================================================================================================*/
// Part 2: Adjustable parameter macro definition
// GPIO mapping register
#define SoftIIC_RCC_APB2Periph  (RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD)
// GPIO speed
#define SoftIIC_GPIO_Speed      GPIO_Speed_50MHz
// SCL pin
#define SoftIIC_SCL_Port        GPIOC
#define SoftIIC_SCL_Pin         GPIO_Pin_0
// SDA pin
#define SoftIIC_SDA_Port        GPIOD
#define SoftIIC_SDA_Pin         GPIO_Pin_0

#endif
