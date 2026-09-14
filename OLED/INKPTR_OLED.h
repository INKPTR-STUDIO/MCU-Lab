#ifndef INKPTR_OLED_H
#define INKPTR_OLED_H

/*================================================================================================*/
// Part 1: Library and interface function declarations
#include "ch32v00x.h"
void    INKPTR_OLED_Init(INKPTR_OLED_AddressingMode AddressingMode, INKPTR_OLED_SetMode_X_Flip X_FlipMode, INKPTR_OLED_SetMode_Y_Flip Y_FlipMode, INKPTR_OLED_SetMode_Color ColorMode, uint8_t Brightness);
void    INKPTR_OLED_Set(INKPTR_OLED_SetMode SetMode, uint8_t Dat);
void    INKPTR_OLED_Brush(uint8_t Page_Begin, uint8_t Page_End, uint8_t List_Begin, uint8_t List_End, uint8_t Style_Byte);
void    INKPTR_OLED_Draw(uint8_t Page_Begin, uint8_t List_Begin);
void    INKPTR_OLED_Roll(uint8_t Page_Begin, uint8_t Page_End, uint8_t List_Begin, uint8_t List_End, INKPTR_OLED_RollMode RollMode);

/*================================================================================================*/
// Part 2: Adjustable parameter macro definition
#define INKPTR_OLED_Model 0
// 128*64	> 0
// 128*32	> 1
// 72*40	> 2

/*================================================================================================*/
// Part 3: Fixed parameter macro definitions and enumerations
#define INKPTR_OLED_ADD 0x78
typedef enum {INKPTR_OLED_AddressingMode_HorizontalMode = 0, INKPTR_OLED_AddressingMode_VerticalMode, INKPTR_OLED_AddressingMode_PageMode} INKPTR_OLED_AddressingMode;
typedef enum {INKPTR_OLED_SetMode_Brightness = 0, INKPTR_OLED_SetMode_X_FlipMode, INKPTR_OLED_SetMode_Y_FlipMode, INKPTR_OLED_SetMode_ColorMode, INKPTR_OLED_SetMode_RollSwitch, INKPTR_OLED_SetMode_ShowSwitch} INKPTR_OLED_SetMode;
typedef enum {INKPTR_OLED_SetMode_X_Flip_Mirror = 0, INKPTR_OLED_SetMode_X_Flip_Normal} INKPTR_OLED_SetMode_X_Flip;
typedef enum {INKPTR_OLED_SetMode_Y_Flip_Mirror = 0, INKPTR_OLED_SetMode_Y_Flip_Normal} INKPTR_OLED_SetMode_Y_Flip;
typedef enum {INKPTR_OLED_SetMode_Color_Invert = 0, INKPTR_OLED_SetMode_Color_Normal} INKPTR_OLED_SetMode_Color;
typedef enum {INKPTR_OLED_SetMode_Roll_DISABLE = 0, INKPTR_OLED_SetMode_Roll_ENABLE} INKPTR_OLED_SetMode_Roll;
typedef enum {INKPTR_OLED_SetMode_Show_DISABLE = 0, INKPTR_OLED_SetMode_Show_ENABLE} INKPTR_OLED_SetMode_Show;
typedef enum
{
    INKPTR_OLED_RollMode_Right_1 = 0x00, INKPTR_OLED_RollMode_Right_2 = 0x01, INKPTR_OLED_RollMode_Right_3 = 0x02, INKPTR_OLED_RollMode_Right_4 = 0x03, INKPTR_OLED_RollMode_Right_5 = 0x04, INKPTR_OLED_RollMode_Right_6 = 0x05, INKPTR_OLED_RollMode_Right_7 = 0x06, INKPTR_OLED_RollMode_Right_8 = 0x07,
    INKPTR_OLED_RollMode_Left_1 = 0x80, INKPTR_OLED_RollMode_Left_2 = 0x81, INKPTR_OLED_RollMode_Left_3 = 0x82, INKPTR_OLED_RollMode_Left_4 = 0x83, INKPTR_OLED_RollMode_Left_5 = 0x84, INKPTR_OLED_RollMode_Left_6 = 0x85, INKPTR_OLED_RollMode_Left_7 = 0x86, INKPTR_OLED_RollMode_Left_8 = 0x87
} INKPTR_OLED_RollMode;

#endif
