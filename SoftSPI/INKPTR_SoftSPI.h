#ifndef INKPTR_SPI_H
#define INKPTR_SPI_H

/*================================================================================================*/
// Part 1: Library and interface function declarations
#include "ch32v00x.h"
void    INKPTR_SPI_Init(void);
void    INKPTR_SPI_Start(void);
void    INKPTR_SPI_Stop(void);
uint8_t INKPTR_SPI_SwapByte(uint8_t SendByte);

/*================================================================================================*/
// Part 2: Adjustable parameter macro definition
// GPIO mapping register
#define INKPTR_SPI_RCC_APB2Periph   RCC_APB2Periph_GPIOA
// GPIO speed
#define INKPTR_SPI_GPIO_Speed       GPIO_Speed_50MHz
// CS
#define INKPTR_SPI_CS_Port          GPIOA
#define INKPTR_SPI_CS_Pin           GPIO_Pin_0
// SCK
#define INKPTR_SPI_SCK_Port         GPIOA
#define INKPTR_SPI_SCK_Pin          GPIO_Pin_1
// MOSI
#define INKPTR_SPI_MOSI_Port        GPIOA
#define INKPTR_SPI_MOSI_Pin         GPIO_Pin_2
// MISO
#define INKPTR_SPI_MISO_Port        GPIOA
#define INKPTR_SPI_MISO_Pin         GPIO_Pin_3
// SPI mode - input value of 0 ~ 3.
#define INKPTR_SPI_Mode             0

#endif
