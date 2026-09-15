/*================================================================================================*/
// Part 1: Libraries
#include "INKPTR_IIC.h"
#include "ch32v00x.h"


/*================================================================================================*/
// Part 2: Time delay function
static void IIC_SCLDelay(void)
{
}


/*================================================================================================*/
// Part 3: Pin packages
static void IIC_EditSCL(uint8_t Dat)
{
    if(Dat) {GPIO_WriteBit(IIC_SCL_Port, IIC_SCL_Pin, Bit_SET);}
    else    {GPIO_WriteBit(IIC_SCL_Port, IIC_SCL_Pin, Bit_RESET);}
    IIC_SCLDelay();
}
static void IIC_EditSDA(uint8_t Dat)
{
    if(Dat) {GPIO_WriteBit(IIC_SDA_Port, IIC_SDA_Pin, Bit_SET);}
    else    {GPIO_WriteBit(IIC_SDA_Port, IIC_SDA_Pin, Bit_RESET);}
}
static uint8_t IIC_ReadSDA(void)
{
    uint8_t SDA_Bit = 0;
    if(GPIO_ReadInputDataBit(IIC_SDA_Port, IIC_SDA_Pin) == Bit_SET) {SDA_Bit = 1;}
    return SDA_Bit;
}


/*================================================================================================*/
// Part 4: Timing modules
/**
 * @fn      IIC_Start
 * 
 * @brief   IIC start timing.
 * 
 * @param   none
 * 
 * @return  none
 */
void IIC_Start(void)
{
    IIC_EditSDA(1);
    IIC_EditSCL(1);
    IIC_EditSDA(0);
    IIC_EditSCL(0);
}

/**
 * @fn      IIC_Stop
 * 
 * @brief   IIC stop timing.
 * 
 * @param   none
 * 
 * @return  none
 */
void IIC_Stop(void)
{
    IIC_EditSCL(0);
    IIC_EditSDA(0);
    IIC_EditSCL(1);
    IIC_EditSDA(1);
}

/**
 * @fn      IIC_SendACK
 * 
 * @brief   IIC send acknowledgment timing.
 * 
 * @param   SendACK - acknowledgment to send.
 * 
 * @return  none
 */
void IIC_SendACK(uint8_t SendACK)
{
    IIC_EditSDA(SendACK);
    IIC_EditSCL(1);
    IIC_EditSCL(0);
}

/**
 * @fn      IIC_ReceiveACK
 * 
 * @brief   IIC receive acknowledgment timing.
 * 
 * @param   none
 * 
 * @return  The received acknowledgment.
 */
uint8_t IIC_ReceiveACK(void) 
{
    uint8_t ReceiveACK, TimeOut = 200;

    IIC_EditSDA(1);
    IIC_EditSCL(1);
    while(TimeOut--)
    {
        ReceiveACK = IIC_ReadSDA();
        if(!ReceiveACK)    {break;}
        IIC_SCLDelay();
    }
    IIC_EditSCL(0);
    return ReceiveACK;
}

/**
 * @fn      IIC_SendByte
 * 
 * @brief   IIC send a byte data timing.
 * 
 * @param   SendByte    - byte data to send.
 * 
 * @return  none
 */
void IIC_SendByte(uint8_t SendByte)
{
    uint8_t i;

    for(i = 0 ; i < 8 ; i++)
    {
        IIC_EditSDA(SendByte & 0x80);
        IIC_EditSCL(1);
        IIC_EditSCL(0);
        SendByte = SendByte << 1;
    }
}

/**
 * @fn      IIC_ReceiveByte
 * 
 * @brief   IIC receive a byte data timing.
 * 
 * @param   none
 * 
 * @return  The received byte.
 */
uint8_t IIC_ReceiveByte(void)
{
    uint8_t i, ReceiveByte=0;

    IIC_EditSDA(1);
    for(i = 0 ; i < 8 ; i++)
    {
        ReceiveByte = ReceiveByte << 1;
        IIC_EditSCL(1);
        if(IIC_ReadSDA())   {ReceiveByte |= 0x01;}
        IIC_EditSCL(0);
    }
    return ReceiveByte;
}


/*================================================================================================*/
// Part 5: Initialization function
/**
 * @fn      IIC_Init
 * 
 * @brief   Initialize the corresponding pins.
 * 
 * @param   none
 * 
 * @return  none
 */
void IIC_Init(void)
{
    RCC_APB2PeriphClockCmd(IIC_RCC_APB2Periph, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    // SCL
    GPIO_InitStructure.GPIO_Pin     = IIC_SCL_Pin;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed   = IIC_GPIO_Speed;
    GPIO_Init(IIC_SCL_Port, &GPIO_InitStructure);
    // SDA
    GPIO_InitStructure.GPIO_Pin     = IIC_SDA_Pin;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed   = IIC_GPIO_Speed;
    GPIO_Init(IIC_SDA_Port, &GPIO_InitStructure);

    IIC_Stop();
}
