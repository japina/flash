#include "flash.h"

SPI_InitTypeDef  SPI_InitStructure;

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
  while (SPI_GetReceptionFIFOStatus(SPI1) != SPI_ReceptionFIFOStatus_Empty)
      SPI_ReceiveData8(SPI1);
}

uint16_t send_and_read_byte(uint16_t cmd) {
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)==RESET);
  SPI_I2S_SendData16(SPI1,cmd);

  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)==RESET);
  return SPI_I2S_ReceiveData16(SPI1);
}


#define SPIx                             SPI1
#define SPIx_CLK                         RCC_APB2Periph_SPI1
#define SPIx_IRQn                        SPI1_IRQn
#define SPIx_IRQHandler                  SPI1_IRQHandler

#define SPIx_SCK_PIN                     GPIO_Pin_5
#define SPIx_SCK_GPIO_PORT               GPIOA
#define SPIx_SCK_GPIO_CLK                RCC_AHBPeriph_GPIOA
#define SPIx_SCK_SOURCE                  GPIO_PinSource5
#define SPIx_SCK_AF                      GPIO_AF_0

#define SPIx_MISO_PIN                    GPIO_Pin_6
#define SPIx_MISO_GPIO_PORT              GPIOA
#define SPIx_MISO_GPIO_CLK               RCC_AHBPeriph_GPIOA
#define SPIx_MISO_SOURCE                 GPIO_PinSource6
#define SPIx_MISO_AF                     GPIO_AF_0

#define SPIx_MOSI_PIN                    GPIO_Pin_7
#define SPIx_MOSI_GPIO_PORT              GPIOA
#define SPIx_MOSI_GPIO_CLK               RCC_AHBPeriph_GPIOA
#define SPIx_MOSI_SOURCE                 GPIO_PinSource7
#define SPIx_MOSI_AF                     GPIO_AF_0

#define SPIx_NSS_PIN                     GPIO_Pin_4
#define SPIx_NSS_GPIO_PORT               GPIOA
#define SPIx_NSS_GPIO_CLK                RCC_AHBPeriph_GPIOA
#define SPIx_NSS_SOURCE                  GPIO_PinSource4
#define SPIx_NSS_AF                      GPIO_AF_0


#define TXBUFFERSIZE                     (countof(TxBuffer) - 1)
#define RXBUFFERSIZE                     TXBUFFERSIZE

#define SPI_DATASIZE                     SPI_DataSize_8b
#define SPI_DATAMASK                     (uint8_t)0xFF


void SPI_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(SPIx_CLK, ENABLE);
  RCC_AHBPeriphClockCmd(SPIx_SCK_GPIO_CLK | SPIx_MISO_GPIO_CLK | SPIx_MOSI_GPIO_CLK |
                        SPIx_NSS_GPIO_CLK , ENABLE);
  GPIO_PinAFConfig(SPIx_SCK_GPIO_PORT, SPIx_SCK_SOURCE, SPIx_SCK_AF);
  GPIO_PinAFConfig(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_SOURCE, SPIx_MOSI_AF);
  GPIO_PinAFConfig(SPIx_MISO_GPIO_PORT, SPIx_MISO_SOURCE, SPIx_MISO_AF);
  GPIO_PinAFConfig(SPIx_NSS_GPIO_PORT, SPIx_NSS_SOURCE, SPIx_NSS_AF);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;
  GPIO_InitStructure.GPIO_Pin = SPIx_SCK_PIN;
  GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin =  SPIx_MOSI_PIN;
  GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = SPIx_MISO_PIN;
  GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = SPIx_NSS_PIN;
  GPIO_Init(SPIx_NSS_GPIO_PORT, &GPIO_InitStructure);
  SPI_I2S_DeInit(SPIx);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize = SPI_DATASIZE;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
}
