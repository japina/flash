#include "spi.h"

SPI_InitTypeDef  SPI_InitStructure;

void SPI_Config(void)
{
  SPI_InitTypeDef     SPI_InitStructure;
  GPIO_InitTypeDef    GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	/* Configure PA5, PA7 in AF pushpool mode */
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         // _PP:Push/Pull _OD:OpenDrain
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;          // _IN  _AF:Alternate  _AN:Analog
	/* Initialize GPIOA */
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         // _PP:Push/Pull _OD:OpenDrain
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	/* Initialize GPIOA */
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = SPI_CRCLength_8b;
	/* Initialize SPI1 */
	SPI_Init(SPI1, &SPI_InitStructure);
	/* enable SPI1 */
	SPI_Cmd(SPI1, ENABLE);

}


void start_comm(uint8_t nss_pin){
    digitalWrite(nss_pin, LOW);
}

void stop_comm(uint8_t nss_pin){
    digitalWrite(nss_pin, HIGH);
}

void send_byte(uint8_t cmd){
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); //wait buffer empty
  SPI_SendData8(SPI1, cmd);
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET); //wait finish sending
  while (SPI_GetReceptionFIFOStatus(SPI1) != SPI_ReceptionFIFOStatus_Empty) {
      SPI_ReceiveData8(SPI1);
  }
}

uint8_t read_byte(void) {
	SPI_RxFIFOThresholdConfig (SPI1, SPI_RxFIFOThreshold_QF); // to set FIFO flag on one byte
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)==RESET) {
    SPI_SendData8(SPI1, 0x0);
  }
  return SPI_ReceiveData8(SPI1);
}

uint16_t send_and_read_byte(uint16_t cmd) {
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)==RESET);
  SPI_I2S_SendData16(SPI1,cmd);

  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)==RESET);
  return SPI_I2S_ReceiveData16(SPI1);
}
