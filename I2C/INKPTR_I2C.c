/*================================================================================================*/
// Part 1: Libraries
#include "INKPTR_I2C.h"
#include "ch32v00x.h"


/*================================================================================================*/
// Part 2: Time delay function
static void INKPTR_I2C_SCLDelay(void)
{
}


/*================================================================================================*/
// Part 3: Pin packages
static void INKPTR_I2C_EditSCL(uint8_t Dat)
{
    if(Dat) {GPIO_WriteBit(INKPTR_I2C_SCL_Port, INKPTR_I2C_SCL_Pin, Bit_SET);}
    else    {GPIO_WriteBit(INKPTR_I2C_SCL_Port, INKPTR_I2C_SCL_Pin, Bit_RESET);}
    INKPTR_I2C_SCLDelay();
}
static void INKPTR_I2C_EditSDA(uint8_t Dat)
{
    if(Dat) {GPIO_WriteBit(INKPTR_I2C_SDA_Port, INKPTR_I2C_SDA_Pin, Bit_SET);}
    else    {GPIO_WriteBit(INKPTR_I2C_SDA_Port, INKPTR_I2C_SDA_Pin, Bit_RESET);}
}
static uint8_t INKPTR_I2C_ReadSDA(void)
{
    uint8_t SDA_Bit = 0;
    if(GPIO_ReadInputDataBit(INKPTR_I2C_SDA_Port, INKPTR_I2C_SDA_Pin) == Bit_SET) {SDA_Bit = 1;}
    return SDA_Bit;
}


/*================================================================================================*/
// Part 4: Timing modules
/**
 * @fn      INKPTR_I2C_Start
 * 
 * @brief   I2C start timing.
 * 
 * @param   none
 * 
 * @return  none
 */
void INKPTR_I2C_Start(void)
{
    INKPTR_I2C_EditSDA(1);
    INKPTR_I2C_EditSCL(1);
    INKPTR_I2C_EditSDA(0);
    INKPTR_I2C_EditSCL(0);
}

/**
 * @fn      INKPTR_I2C_Stop
 * 
 * @brief   I2C stop timing.
 * 
 * @param   none
 * 
 * @return  none
 */
void INKPTR_I2C_Stop(void)
{
    INKPTR_I2C_EditSCL(0);
    INKPTR_I2C_EditSDA(0);
    INKPTR_I2C_EditSCL(1);
    INKPTR_I2C_EditSDA(1);
}

/**
 * @fn      INKPTR_I2C_SendACK
 * 
 * @brief   I2C send acknowledgment timing.
 * 
 * @param   SendACK - acknowledgment to send.
 * 
 * @return  none
 */
void INKPTR_I2C_SendACK(uint8_t SendACK)
{
    INKPTR_I2C_EditSDA(SendACK);
    INKPTR_I2C_EditSCL(1);
    INKPTR_I2C_EditSCL(0);
}

/**
 * @fn      INKPTR_I2C_ReceiveACK
 * 
 * @brief   I2C receive acknowledgment timing.
 * 
 * @param   none
 * 
 * @return  The received acknowledgment.
 */
uint8_t INKPTR_I2C_ReceiveACK(void) 
{
    uint8_t ReceiveACK, TimeOut = 200;

    INKPTR_I2C_EditSDA(1);
    INKPTR_I2C_EditSCL(1);
    while(TimeOut--)
    {
        ReceiveACK = INKPTR_I2C_ReadSDA();
        if(!ReceiveACK)    {break;}
        INKPTR_I2C_SCLDelay();
    }
    INKPTR_I2C_EditSCL(0);
    return ReceiveACK;
}

/**
 * @fn      INKPTR_I2C_SendByte
 * 
 * @brief   I2C send a byte data timing.
 * 
 * @param   SendByte    - byte data to send.
 * 
 * @return  none
 */
void INKPTR_I2C_SendByte(uint8_t SendByte)
{
    uint8_t i;

    for(i = 0 ; i < 8 ; i++)
    {
        INKPTR_I2C_EditSDA(SendByte & 0x80);
        INKPTR_I2C_EditSCL(1);
        INKPTR_I2C_EditSCL(0);
        SendByte = SendByte << 1;
    }
}

/**
 * @fn      INKPTR_I2C_ReceiveByte
 * 
 * @brief   I2C receive a byte data timing.
 * 
 * @param   none
 * 
 * @return  The received byte.
 */
uint8_t INKPTR_I2C_ReceiveByte(void)
{
    uint8_t i, ReceiveByte=0;

    INKPTR_I2C_EditSDA(1);
    for(i = 0 ; i < 8 ; i++)
    {
        ReceiveByte = ReceiveByte << 1;
        INKPTR_I2C_EditSCL(1);
        if(INKPTR_I2C_ReadSDA())   {ReceiveByte |= 0x01;}
        INKPTR_I2C_EditSCL(0);
    }
    return ReceiveByte;
}


/*================================================================================================*/
// Part 5: Initialization function
/**
 * @fn      INKPTR_I2C_Init
 * 
 * @brief   Initialize the corresponding pins.
 * 
 * @param   none
 * 
 * @return  none
 */
void INKPTR_I2C_Init(void)
{
    RCC_APB2PeriphClockCmd(INKPTR_I2C_RCC_APB2Periph, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    // SCL
    GPIO_InitStructure.GPIO_Pin     = INKPTR_I2C_SCL_Pin;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed   = INKPTR_I2C_GPIO_Speed;
    GPIO_Init(INKPTR_I2C_SCL_Port, &GPIO_InitStructure);
    // SDA
    GPIO_InitStructure.GPIO_Pin     = INKPTR_I2C_SDA_Pin;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed   = INKPTR_I2C_GPIO_Speed;
    GPIO_Init(INKPTR_I2C_SDA_Port, &GPIO_InitStructure);

    INKPTR_I2C_Stop();
}
