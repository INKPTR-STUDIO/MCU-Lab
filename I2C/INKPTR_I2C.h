#ifndef INKPTR_I2C_H
#define INKPTR_I2C_H

/*================================================================================================*/
// Part 1: Library and interface function declarations
#include "ch32v00x.h"
void    INKPTR_I2C_Init(void);
void    INKPTR_I2C_Start(void);
void    INKPTR_I2C_Stop(void);
void    INKPTR_I2C_SendByte(uint8_t SendByte);
uint8_t INKPTR_I2C_ReceiveACK(void);
uint8_t INKPTR_I2C_ReceiveByte(void);
void    INKPTR_I2C_SendACK(uint8_t SendACK);

/*================================================================================================*/
// Part 2: Adjustable parameter macro definition
// GPIO mapping register
#define INKPTR_I2C_RCC_APB2Periph  (RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD)
// GPIO speed
#define INKPTR_I2C_GPIO_Speed      GPIO_Speed_50MHz
// SCL pin
#define INKPTR_I2C_SCL_Port        GPIOC
#define INKPTR_I2C_SCL_Pin         GPIO_Pin_0
// SDA pin
#define INKPTR_I2C_SDA_Port        GPIOD
#define INKPTR_I2C_SDA_Pin         GPIO_Pin_0

#endif
