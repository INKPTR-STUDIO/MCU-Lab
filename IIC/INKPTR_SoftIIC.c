/*================================================================================================*/
// Part 1: Libraries
#include "INKPTR_SoftIIC.h"
#include "ch32v00x.h"


/*================================================================================================*/
// Part 2: Time delay function
static void SoftIIC_SCLDelay(void)
{
}


/*================================================================================================*/
// Part 3: Pin packages
static void SoftIIC_EditSCL(uint8_t Dat)
{
    if(Dat) {GPIO_WriteBit(SoftIIC_SCL_Port, SoftIIC_SCL_Pin, Bit_SET);}
    else    {GPIO_WriteBit(SoftIIC_SCL_Port, SoftIIC_SCL_Pin, Bit_RESET);}
    SoftIIC_SCLDelay();
}
static void SoftIIC_EditSDA(uint8_t Dat)
{
    if(Dat) {GPIO_WriteBit(SoftIIC_SDA_Port, SoftIIC_SDA_Pin, Bit_SET);}
    else    {GPIO_WriteBit(SoftIIC_SDA_Port, SoftIIC_SDA_Pin, Bit_RESET);}
}
static uint8_t SoftIIC_ReadSDA(void)
{
    uint8_t SDA_Bit = 0;
    if(GPIO_ReadInputDataBit(SoftIIC_SDA_Port, SoftIIC_SDA_Pin) == Bit_SET) {SDA_Bit = 1;}
    return SDA_Bit;
}


/*================================================================================================*/
// Part 4: Timing modules
/**
 * @fn      SoftIIC_Start
 * 
 * @brief   SoftIIC start timing.
 * 
 * @param   none
 * 
 * @return  none
 */
void SoftIIC_Start(void)
{
    SoftIIC_EditSDA(1);
    SoftIIC_EditSCL(1);
    SoftIIC_EditSDA(0);
    SoftIIC_EditSCL(0);
}

/**
 * @fn      SoftIIC_Stop
 * 
 * @brief   SoftIIC stop timing.
 * 
 * @param   none
 * 
 * @return  none
 */
void SoftIIC_Stop(void)
{
    SoftIIC_EditSCL(0);
    SoftIIC_EditSDA(0);
    SoftIIC_EditSCL(1);
    SoftIIC_EditSDA(1);
}

/**
 * @fn      SoftIIC_SendACK
 * 
 * @brief   SoftIIC send acknowledgment timing.
 * 
 * @param   SendACK - acknowledgment to send.
 * 
 * @return  none
 */
void SoftIIC_SendACK(uint8_t SendACK)
{
    SoftIIC_EditSDA(SendACK);
    SoftIIC_EditSCL(1);
    SoftIIC_EditSCL(0);
}

/**
 * @fn      SoftIIC_ReceiveACK
 * 
 * @brief   SoftIIC receive acknowledgment timing.
 * 
 * @param   none
 * 
 * @return  The received acknowledgment.
 */
uint8_t SoftIIC_ReceiveACK(void) 
{
    uint8_t ReceiveACK, TimeOut = 200;

    SoftIIC_EditSDA(1);
    SoftIIC_EditSCL(1);
    while(TimeOut--)
    {
        ReceiveACK = SoftIIC_ReadSDA();
        if(!ReceiveACK)    {break;}
        SoftIIC_SCLDelay();
    }
    SoftIIC_EditSCL(0);
    return ReceiveACK;
}

/**
 * @fn      SoftIIC_SendByte
 * 
 * @brief   SoftIIC send a byte data timing.
 * 
 * @param   SendByte    - byte data to send.
 * 
 * @return  none
 */
void SoftIIC_SendByte(uint8_t SendByte)
{
    uint8_t i;

    for(i = 0 ; i < 8 ; i++)
    {
        SoftIIC_EditSDA(SendByte & 0x80);
        SoftIIC_EditSCL(1);
        SoftIIC_EditSCL(0);
        SendByte = SendByte << 1;
    }
}

/**
 * @fn      SoftIIC_ReceiveByte
 * 
 * @brief   SoftIIC receive a byte data timing.
 * 
 * @param   none
 * 
 * @return  The received byte.
 */
uint8_t SoftIIC_ReceiveByte(void)
{
    uint8_t i, ReceiveByte=0;

    SoftIIC_EditSDA(1);
    for(i = 0 ; i < 8 ; i++)
    {
        ReceiveByte = ReceiveByte << 1;
        SoftIIC_EditSCL(1);
        if(SoftIIC_ReadSDA())   {ReceiveByte |= 0x01;}
        SoftIIC_EditSCL(0);
    }
    return ReceiveByte;
}


/*================================================================================================*/
// Part 5: Initialization function
/**
 * @fn      SoftIIC_Init
 * 
 * @brief   Initialize the corresponding pins.
 * 
 * @param   none
 * 
 * @return  none
 */
void SoftIIC_Init(void)
{
    RCC_APB2PeriphClockCmd(SoftIIC_RCC_APB2Periph, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    // SCL
    GPIO_InitStructure.GPIO_Pin     = SoftIIC_SCL_Pin;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed   = SoftIIC_GPIO_Speed;
    GPIO_Init(SoftIIC_SCL_Port, &GPIO_InitStructure);
    // SDA
    GPIO_InitStructure.GPIO_Pin     = SoftIIC_SDA_Pin;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed   = SoftIIC_GPIO_Speed;
    GPIO_Init(SoftIIC_SDA_Port, &GPIO_InitStructure);

    SoftIIC_Stop();
}
