/*================================================================================================*/
// Part 1: Libraries
#include "INKPTR_OLED.h"
#include "INKPTR_I2C.h"
#include "ch32v00x.h"


/*================================================================================================*/
// Part 2: OLED mode parameters
static const uint8_t INKPTR_OLED_Model_Dat[3][5]=
{
    {0x3f, 0x12, 0x00, 7, 127}, // 128*64 Model
    {0x1f, 0x02, 0x00, 3, 127}, // 128*32 Model
    {0x27, 0x12, 0x1c, 4, 71}   // 72*40 Model
};
static const uint8_t INKPTR_OLED_InitCmd[]=
{
    0xd5,0x80,  // Clock
    0xd3,0x00,  // Column offset
    0x40,       // Page offset
    0x8d,0x14,  // Use the internal charge pump
    0xd9,0xf1,  // Pre-charge cycle
    0xdB,0x40,  // VCOMH voltage
    0xa4        // RAM display
};


/*================================================================================================*/
// Part 3: Boundary checking function
static uint8_t INKPTR_OLED_ValueCheck(uint8_t Page_min, uint8_t Page_max, uint8_t List_min, uint8_t List_max)
{
    if(Page_min > Page_max)                                     {return 1;}
    if(List_min > List_max)                                     {return 1;}
    if(Page_max > INKPTR_OLED_Model_Dat[INKPTR_OLED_Model][3])  {return 1;}
    if(List_max > INKPTR_OLED_Model_Dat[INKPTR_OLED_Model][4])  {return 1;}
    return 0;
}


/*================================================================================================*/
// Part 4: Data header packages
static void INKPTR_OLED_Cmd(void)
{
    INKPTR_I2C_Start();
    INKPTR_I2C_SendByte(INKPTR_OLED_ADD);   INKPTR_I2C_ReceiveACK();
    INKPTR_I2C_SendByte(0x00);              INKPTR_I2C_ReceiveACK();
}
static void INKPTR_OLED_Dat(void)
{
    INKPTR_I2C_Start();
    INKPTR_I2C_SendByte(INKPTR_OLED_ADD);   INKPTR_I2C_ReceiveACK();
    INKPTR_I2C_SendByte(0x40);              INKPTR_I2C_ReceiveACK();
}


/*================================================================================================*/
// Part 5: Functional function
/**
 * @fn      INKPTR_OLED_Set
 * 
 * @brief   Edit OLED display settings.
 * 
 * @param   | SetMode                           | Dat
 *          -----------------------------------------------------------------------------------------------------------
 *          | INKPTR_OLED_SetMode_Brightness    | (Value: 0 ~ 255)
 *          | INKPTR_OLED_SetMode_X_FlipMode    | INKPTR_OLED_SetMode_X_Flip_Normal / INKPTR_OLED_SetMode_X_Flip_Mirror
 *          | INKPTR_OLED_SetMode_Y_FlipMode    | INKPTR_OLED_SetMode_Y_Flip_Normal / INKPTR_OLED_SetMode_Y_Flip_Mirror
 *          | INKPTR_OLED_SetMode_ColorMode     | INKPTR_OLED_SetMode_Color_Normal / INKPTR_OLED_SetMode_Color_Invert
 *          | INKPTR_OLED_SetMode_RollSwitch    | INKPTR_OLED_SetMode_Roll_ENABLE / INKPTR_OLED_SetMode_Roll_DISABLE
 *          | INKPTR_OLED_SetMode_ShowSwitch    | INKPTR_OLED_SetMode_Show_ENABLE / INKPTR_OLED_SetMode_Show_DISABLE
 * 
 * @return  none
 */
void INKPTR_OLED_Set(INKPTR_OLED_SetMode SetMode, uint8_t Dat)
{
    INKPTR_OLED_Cmd();
    switch(SetMode) {
        case INKPTR_OLED_SetMode_Brightness:
        {
            INKPTR_I2C_SendByte(0x81);      INKPTR_I2C_ReceiveACK();
            INKPTR_I2C_SendByte(Dat);       INKPTR_I2C_ReceiveACK();
            break;
        }
        case INKPTR_OLED_SetMode_X_FlipMode:
        {
            if(Dat) {INKPTR_I2C_SendByte(0xa0); INKPTR_I2C_ReceiveACK();}
            else    {INKPTR_I2C_SendByte(0xa1); INKPTR_I2C_ReceiveACK();}
            break;
        }
        case INKPTR_OLED_SetMode_Y_FlipMode:
        {
            if(Dat) {INKPTR_I2C_SendByte(0xc0); INKPTR_I2C_ReceiveACK();}
            else    {INKPTR_I2C_SendByte(0xc8); INKPTR_I2C_ReceiveACK();}
            break;
        }
        case INKPTR_OLED_SetMode_ColorMode:
        {
            if(Dat) {INKPTR_I2C_SendByte(0xa6); INKPTR_I2C_ReceiveACK();}
            else    {INKPTR_I2C_SendByte(0xa7); INKPTR_I2C_ReceiveACK();}
            break;
        }
        case INKPTR_OLED_SetMode_RollSwitch:
        {
            if(Dat) {INKPTR_I2C_SendByte(0x2f); INKPTR_I2C_ReceiveACK();}
            else    {INKPTR_I2C_SendByte(0x2e); INKPTR_I2C_ReceiveACK();}
            break;
        }
        case INKPTR_OLED_SetMode_ShowSwitch:
        {
            if(Dat) {INKPTR_I2C_SendByte(0xaf); INKPTR_I2C_ReceiveACK();}
            else    {INKPTR_I2C_SendByte(0xae); INKPTR_I2C_ReceiveACK();}
            break;
        }
    }
    INKPTR_I2C_Stop();
}

/**
 * @fn      INKPTR_OLED_Brush
 * 
 * @brief   Display a block in a byte style.
 * 
 * @param   Page_Begin  - the beginning of page add.
 *          Page_End    - the ending of page add.
 *          List_Begin  - the beginning of list add.
 *          List_End    - the ending of list add.
 *          Style_Byte  - a byte as the style of brush.
 * 
 * @return  none
 */
void INKPTR_OLED_Brush(uint8_t Page_Begin, uint8_t Page_End, uint8_t List_Begin, uint8_t List_End, uint8_t Style_Byte)
{
    uint8_t x, y;

    if(INKPTR_OLED_ValueCheck(Page_Begin, Page_End, List_Begin, List_End))  {return;}

    INKPTR_OLED_Set(INKPTR_OLED_SetMode_RollSwitch, INKPTR_OLED_SetMode_Roll_DISABLE);

    for(y = Page_Begin ; y < Page_End+1 ; y++) {
        INKPTR_OLED_Cmd();
        INKPTR_I2C_SendByte(0xb0 + y);                                                                  INKPTR_I2C_ReceiveACK();
        INKPTR_I2C_SendByte(0x0f & (List_Begin + INKPTR_OLED_Model_Dat[INKPTR_OLED_Model][2]));         INKPTR_I2C_ReceiveACK();
        INKPTR_I2C_SendByte(0x10 | ((List_Begin + INKPTR_OLED_Model_Dat[INKPTR_OLED_Model][2])>>4));    INKPTR_I2C_ReceiveACK();
        INKPTR_I2C_Stop();
        INKPTR_OLED_Dat();
        for(x = List_Begin ; x < List_End+1 ; x++) {INKPTR_I2C_SendByte(Style_Byte);    INKPTR_I2C_ReceiveACK();}
        INKPTR_I2C_Stop();
    }
}

/**
 * @fn      INKPTR_OLED_Draw
 * 
 * @brief   Send bytes with "INKPTR_I2C_SendByte" & "INKPTR_I2C_ReceiveACK" after the function to display them.
 *          Pay attention to timing integrity, don't forget to end the sequence with "INKPTR_I2C_Stop"!
 * 
 * @param   Page_Begin  - the beginning of page add.
 *          List_Begin  - the beginning of list add.
 * 
 * @return  none
 */
void INKPTR_OLED_Draw(uint8_t Page_Begin, uint8_t List_Begin)
{
    if(INKPTR_OLED_ValueCheck(0, Page_Begin, 0, List_Begin))    {return;}

    INKPTR_OLED_Set(INKPTR_OLED_SetMode_RollSwitch, INKPTR_OLED_SetMode_Roll_DISABLE);

    INKPTR_OLED_Cmd();
    INKPTR_I2C_SendByte(0xB0 + Page_Begin);                                                         INKPTR_I2C_ReceiveACK();
    INKPTR_I2C_SendByte(0x0F & (List_Begin + INKPTR_OLED_Model_Dat[INKPTR_OLED_Model][2]));         INKPTR_I2C_ReceiveACK();
    INKPTR_I2C_SendByte(0x10 | ((List_Begin + INKPTR_OLED_Model_Dat[INKPTR_OLED_Model][2])>>4));    INKPTR_I2C_ReceiveACK();
    INKPTR_I2C_Stop();

    INKPTR_OLED_Dat();
}

/**
 * @fn      INKPTR_OLED_Roll
 * 
 * @brief   Set up and enable the scrolling.
 * 
 * @param   Page_Begin  - the beginning of page add.
 *          Page_End    - the ending of page add.
 *          List_Begin  - the beginning of list add.
 *          List_End    - the ending of list add.
 *          RollMode    - select one from the scrolling modes.
 *              Scroll Left:    INKPTR_OLED_RollMode_Left_1 ~ INKPTR_OLED_RollMode_Left_8
 *              Scroll Right:   INKPTR_OLED_RollMode_Right_1 ~ INKPTR_OLED_RollMode_Right_8
 *              (The bigger the suffix value, the faster the speed.)
 * 
 * @return  none
 */
void INKPTR_OLED_Roll(uint8_t Page_Begin, uint8_t Page_End, uint8_t List_Begin, uint8_t List_End, INKPTR_OLED_RollMode RollMode)
{
    uint8_t SpeedTable[] = {3, 2, 1, 0, 6, 5, 4, 7};

    if(INKPTR_OLED_ValueCheck(Page_Begin, Page_End, List_Begin, List_End))  {return;}

    INKPTR_OLED_Set(INKPTR_OLED_SetMode_RollSwitch, INKPTR_OLED_SetMode_Roll_DISABLE);

    INKPTR_OLED_Cmd();
    if(RollMode & 0x80) {INKPTR_I2C_SendByte(0x27); INKPTR_I2C_ReceiveACK();}
    else                {INKPTR_I2C_SendByte(0x26); INKPTR_I2C_ReceiveACK();}
    INKPTR_I2C_SendByte(0);                                 INKPTR_I2C_ReceiveACK();
    INKPTR_I2C_SendByte(Page_Begin);                        INKPTR_I2C_ReceiveACK();
    INKPTR_I2C_SendByte(SpeedTable[RollMode & (~0x80)]);    INKPTR_I2C_ReceiveACK();
    INKPTR_I2C_SendByte(Page_End);                          INKPTR_I2C_ReceiveACK();
    INKPTR_I2C_SendByte(List_Begin);                        INKPTR_I2C_ReceiveACK();
    INKPTR_I2C_SendByte(List_End);                          INKPTR_I2C_ReceiveACK();
    INKPTR_I2C_Stop();

    INKPTR_OLED_Set(INKPTR_OLED_SetMode_RollSwitch, INKPTR_OLED_SetMode_Roll_ENABLE);
}


/*================================================================================================*/
// Part 6: Initialization function
/**
 * @fn      INKPTR_OLED_Init
 * 
 * @brief   Complete the display setup initialization.
 * 
 * @param   AddressingMode  - select one to set addressing mode for display.
 *              INKPTR_OLED_AddressingMode_HorizontalMode
 *              INKPTR_OLED_AddressingMode_VerticalMode
 *              INKPTR_OLED_AddressingMode_PageMode
 *          X_FlipMode      - select one to set initial horizontal mirror flip.
 *              INKPTR_OLED_SetMode_X_Flip_Normal / INKPTR_OLED_SetMode_X_Flip_Mirror
 *          Y_FlipMode      - select one to set initial vertical mirror flip.
 *              INKPTR_OLED_SetMode_Y_Flip_Normal / INKPTR_OLED_SetMode_Y_Flip_Mirror
 *          ColorMode       - select one to set initial color mode.
 *              INKPTR_OLED_SetMode_Color_Normal / INKPTR_OLED_SetMode_Color_Invert
 *          Brightness      - input one value of 0~255 to set initial brightness value.
 * 
 * @return  none
 */
void INKPTR_OLED_Init(INKPTR_OLED_AddressingMode AddressingMode, INKPTR_OLED_SetMode_X_Flip X_FlipMode, INKPTR_OLED_SetMode_Y_Flip Y_FlipMode, INKPTR_OLED_SetMode_Color ColorMode, uint8_t Brightness)
{
    uint8_t i;

    INKPTR_OLED_Set(INKPTR_OLED_SetMode_ShowSwitch, INKPTR_OLED_SetMode_Show_DISABLE);
    INKPTR_OLED_Set(INKPTR_OLED_SetMode_RollSwitch, INKPTR_OLED_SetMode_Roll_DISABLE);

    INKPTR_OLED_Cmd();
    for(i = 0 ; i < sizeof(INKPTR_OLED_InitCmd) ; i++) {INKPTR_I2C_SendByte(INKPTR_OLED_InitCmd[i]);    INKPTR_I2C_ReceiveACK();}
    INKPTR_I2C_SendByte(0xa8);                                          INKPTR_I2C_ReceiveACK();
    INKPTR_I2C_SendByte(INKPTR_OLED_Model_Dat[INKPTR_OLED_Model][0]);   INKPTR_I2C_ReceiveACK();
    INKPTR_I2C_SendByte(0xda);                                          INKPTR_I2C_ReceiveACK();
    INKPTR_I2C_SendByte(INKPTR_OLED_Model_Dat[INKPTR_OLED_Model][1]);   INKPTR_I2C_ReceiveACK();
    INKPTR_I2C_SendByte(0x20);                                          INKPTR_I2C_ReceiveACK();
    INKPTR_I2C_SendByte(AddressingMode);                                INKPTR_I2C_ReceiveACK();
    INKPTR_I2C_Stop();

    INKPTR_OLED_Set(INKPTR_OLED_SetMode_Brightness, Brightness);
    INKPTR_OLED_Set(INKPTR_OLED_SetMode_X_FlipMode, X_FlipMode);
    INKPTR_OLED_Set(INKPTR_OLED_SetMode_Y_FlipMode, Y_FlipMode);
    INKPTR_OLED_Set(INKPTR_OLED_SetMode_ColorMode, ColorMode);
    INKPTR_OLED_Brush(0, INKPTR_OLED_Model_Dat[INKPTR_OLED_Model][3], 0, INKPTR_OLED_Model_Dat[INKPTR_OLED_Model][4], 0);

    INKPTR_OLED_Set(INKPTR_OLED_SetMode_ShowSwitch, INKPTR_OLED_SetMode_Show_ENABLE);
}
