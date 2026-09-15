#ifndef INKPTR_IIC_H
#define INKPTR_IIC_H

/*================================================================================================*/
// Part 1: Library and interface function declarations
#include "ch32v00x.h"
void    IIC_Init(void);
void    IIC_Start(void);
void    IIC_Stop(void);
void    IIC_SendByte(uint8_t SendByte);
uint8_t IIC_ReceiveACK(void);
uint8_t IIC_ReceiveByte(void);
void    IIC_SendACK(uint8_t SendACK);

/*================================================================================================*/
// Part 2: Adjustable parameter macro definition
// GPIO mapping register
#define IIC_RCC_APB2Periph  (RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD)
// GPIO speed
#define IIC_GPIO_Speed      GPIO_Speed_50MHz
// SCL pin
#define IIC_SCL_Port        GPIOC
#define IIC_SCL_Pin         GPIO_Pin_0
// SDA pin
#define IIC_SDA_Port        GPIOD
#define IIC_SDA_Pin         GPIO_Pin_0

#endif
