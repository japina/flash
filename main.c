 #include <string.h>
#include "stm32f0xx.h"
#include "stm32f0xx_spi.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_adc.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx.h"
#include "flash.h"
#include "delay.h"
#include "pins.h"
#include "spi.h"


#define START_BUTTON B12
#define TRANSFER_BUTTON B13
#define START_LED B0
#define TRANSFERING_LED B2

void send_buffer(char* buffer){
  uint8_t i;
  uint8_t size = strlen(buffer);
  for(i=0;i<size;i++){
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, buffer[i]);
  }

}

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_1); // USART1 CTS
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_1); // USART1 RTS
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1); // USART1 TX
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);

}

void USART_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  //USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);

  USART_Cmd(USART1,ENABLE);
}


void transferData(uint16_t end_address){
  uint8_t buffer[2];
  uint16_t address = 0;
  GPIO_Configuration();
  USART_Configuration();
  while (address < end_address){
    read_memory(address, &buffer[0], 2);
    send_buffer((char*)buffer);
  }
}

int main(void)
{
	uint16_t adc;
  uint8_t flag = 0;
  uint16_t address = 0;
  uint8_t start_button_in = LOW;
  uint8_t transfer_button_in = LOW;  
	pinMode(START_BUTTON, IN);
	pinMode(TRANSFER_BUTTON, IN);
  pinMode(START_LED, OUT);
  pinMode(TRANSFERING_LED, OUT);	
	digitalWrite(START_LED, LOW);
	digitalWrite(TRANSFERING_LED, LOW);	
	init_flash();

  GPIO_InitTypeDef GPIO_InitStructure;
  ADC_InitTypeDef  ADC_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
  ADC_Init(ADC1, &ADC_InitStructure);

  ADC_ChannelConfig(ADC1, ADC_Channel_9, ADC_SampleTime_239_5Cycles);

  // Calibrate ADC before enabling
  ADC_GetCalibrationFactor(ADC1);
  //(#) Activate the ADC peripheral using ADC_Cmd() function.
  ADC_Cmd(ADC1, ENABLE);
  // Wait until ADC enabled
  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN) == RESET);	

  while(1){
    start_button_in = digitalRead(START_BUTTON);
    if (start_button_in != LOW) {
      flag = 1;
      digitalWrite(START_LED, HIGH);
      while(flag == 1){
        //start conversion
        ADC_StartOfConversion(ADC1);

        // wait until the conversion is finished
        while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET){;}

        digitalWrite(TRANSFERING_LED, HIGH);

        // result of conversion
        adc = ADC_GetConversionValue(ADC1);
        uint8_t wdata[] = {adc && 0xff, adc << 8};
        write_2_memory(address, wdata, 2);
        address += 2;
        transfer_button_in = digitalRead(TRANSFER_BUTTON);
        if (transfer_button_in == HIGH){
          transferData(address);
        }
        start_button_in = digitalRead(START_BUTTON);      
        if (start_button_in == LOW){
          digitalWrite(START_LED, LOW);        
          flag = 0;
        }
      }
      transfer_button_in = digitalRead(TRANSFER_BUTTON);
      if (transfer_button_in != LOW){
        digitalWrite(TRANSFERING_LED, HIGH);
        transferData(address);
      } else {
        digitalWrite(TRANSFERING_LED, LOW);      
      }
    }
  }
	//digitalWrite(A4, HIGH);    	
}
