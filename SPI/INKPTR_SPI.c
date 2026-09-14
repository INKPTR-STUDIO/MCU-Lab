/*================================================================================================*/
// Part 1: Libraries
#include "INKPTR_SPI.h"
#include "ch32v00x.h"


/*================================================================================================*/
// Part 2: Time delay function
static void INKPTR_SPI_CSDelay(void)
{
}
static void INKPTR_SPI_SCKDelay(void)
{
}


/*================================================================================================*/
// Part 3: Pin packages
static void INKPTR_SPI_EditSCK(uint8_t Dat)
{
	if(Dat) {GPIO_WriteBit(INKPTR_SPI_SCK_Port, INKPTR_SPI_SCK_Pin, Bit_SET);}
    else    {GPIO_WriteBit(INKPTR_SPI_SCK_Port, INKPTR_SPI_SCK_Pin, Bit_RESET);}
	INKPTR_SPI_SCKDelay();
}
static void INKPTR_SPI_EditMOSI(uint8_t Dat)
{
	if(Dat) {GPIO_WriteBit(INKPTR_SPI_MOSI_Port, INKPTR_SPI_MOSI_Pin, Bit_SET);}
    else    {GPIO_WriteBit(INKPTR_SPI_MOSI_Port, INKPTR_SPI_MOSI_Pin, Bit_RESET);}
}
static uint8_t INKPTR_SPI_ReadMISO(void)
{
    if(GPIO_ReadInputDataBit(INKPTR_SPI_MISO_Port, INKPTR_SPI_MISO_Pin) == Bit_SET)	{return 1;}
    else																			{return 0;}
}

/*================================================================================================*/
// Part 4: The swap bytes timing modules for each mode
static uint8_t INKPTR_SPI_SwapBytes_Mode0(uint8_t SendByte)
{
	uint8_t i, ReceiveByte = 0x00;

	for(i = 0 ; i < 8 ; i++)
	{
		INKPTR_SPI_EditMOSI(SendByte & (0x80>>i));
		INKPTR_SPI_EditSCK(1);
		if(INKPTR_SPI_ReadMISO())	{ReceiveByte |= 0x80>>i;}
		INKPTR_SPI_EditSCK(0);
	}
	return ReceiveByte;
}
static uint8_t INKPTR_SPI_SwapBytes_Mode1(uint8_t SendByte)
{
	uint8_t i, ReceiveByte = 0x00;

	for(i = 0 ; i < 8 ; i++)
	{
		INKPTR_SPI_EditSCK(1);
		INKPTR_SPI_EditMOSI(SendByte & (0x80>>i));
		INKPTR_SPI_EditSCK(0);
		if(INKPTR_SPI_ReadMISO())	{ReceiveByte |= 0x80>>i;}
	}
	return ReceiveByte;
}
static uint8_t INKPTR_SPI_SwapBytes_Mode2(uint8_t SendByte)
{
	uint8_t i, ReceiveByte = 0x00;

	for(i = 0 ; i < 8 ; i++)
	{
		INKPTR_SPI_EditMOSI(SendByte & (0x80>>i));
		INKPTR_SPI_EditSCK(0);
		if(INKPTR_SPI_ReadMISO())	{ReceiveByte |= 0x80>>i;}
		INKPTR_SPI_EditSCK(1);
	}
	return ReceiveByte;
}
static uint8_t INKPTR_SPI_SwapBytes_Mode3(uint8_t SendByte)
{
	uint8_t i, ReceiveByte = 0x00;

	for(i = 0 ; i < 8 ; i++)
	{
		INKPTR_SPI_EditSCK(0);
		INKPTR_SPI_EditMOSI(SendByte&(0x80>>i));
		INKPTR_SPI_EditSCK(1);
		if(INKPTR_SPI_ReadMISO())	{ReceiveByte |= 0x80>>i;}
	}
	return ReceiveByte;
}


/*================================================================================================*/
// Part 5: Timing modules
/**
 * @fn      INKPTR_SPI_Start
 * 
 * @brief   SPI start timing.
 * 
 * @param   none
 * 
 * @return  none
 */
void INKPTR_SPI_Start(void)
{
	GPIO_WriteBit(INKPTR_SPI_CS_Port, INKPTR_SPI_CS_Pin, Bit_RESET);
	INKPTR_SPI_CSDelay();
}

/**
 * @fn      INKPTR_SPI_Stop
 * 
 * @brief   SPI stop timing.
 * 
 * @param   none
 * 
 * @return  none
 */
void INKPTR_SPI_Stop(void)
{
	GPIO_WriteBit(INKPTR_SPI_CS_Port, INKPTR_SPI_CS_Pin, Bit_SET);
	INKPTR_SPI_CSDelay();
}

/**
 * @fn      INKPTR_SPI_SwapBytes
 * 
 * @brief   Initialize the corresponding pins.
 * 
 * @param   SendByte	- byte data to send.
 * 
 * @return  The received acknowledgment.
 */
uint8_t INKPTR_SPI_SwapBytes(uint8_t SendByte)
{
	uint8_t ReceiveByte;

	switch (INKPTR_SPI_Mode)
	{
		case 0:	{ReceiveByte = INKPTR_SPI_SwapBytes_Mode0(SendByte);	break;}
		case 1:	{ReceiveByte = INKPTR_SPI_SwapBytes_Mode1(SendByte);	break;}
		case 2:	{ReceiveByte = INKPTR_SPI_SwapBytes_Mode2(SendByte);	break;}
		case 3:	{ReceiveByte = INKPTR_SPI_SwapBytes_Mode3(SendByte);	break;}
	}

	return ReceiveByte;
}


/*================================================================================================*/
// Part 6: Initialization function
/**
 * @fn      INKPTR_SPI_Init
 * 
 * @brief   Initialize the corresponding pins.
 * 
 * @param   none
 * 
 * @return  none
 */
void INKPTR_SPI_Init(void)
{
    RCC_APB2PeriphClockCmd(INKPTR_SPI_RCC_APB2Periph, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    // CS
    GPIO_InitStructure.GPIO_Pin		= INKPTR_SPI_CS_Pin;
    GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed	= INKPTR_SPI_GPIO_Speed;
    GPIO_Init(INKPTR_SPI_CS_Port, &GPIO_InitStructure);
    // SCK
    GPIO_InitStructure.GPIO_Pin		= INKPTR_SPI_SCK_Pin;
    GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed	= INKPTR_SPI_GPIO_Speed;
    GPIO_Init(INKPTR_SPI_SCK_Port, &GPIO_InitStructure);
    // MOSI
    GPIO_InitStructure.GPIO_Pin		= INKPTR_SPI_MOSI_Pin;
    GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed	= INKPTR_SPI_GPIO_Speed;
    GPIO_Init(INKPTR_SPI_MOSI_Port, &GPIO_InitStructure);
    // MISO
    GPIO_InitStructure.GPIO_Pin		= INKPTR_SPI_MISO_Pin;
    GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed	= INKPTR_SPI_GPIO_Speed;
    GPIO_Init(INKPTR_SPI_MISO_Port, &GPIO_InitStructure);

	INKPTR_SPI_Stop();
	if(INKPTR_SPI_Mode < 2)	{INKPTR_SPI_EditSCK(0);}
	else					{INKPTR_SPI_EditSCK(1);}
}

