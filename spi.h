#ifndef __SPI_H_
#define __SPI_H_

#include <stdlib.h>
#include "stm32f0xx.h"
#include "peripherals/stm32f0xx_spi.h"
#include "peripherals/stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "pins.h"

SPI_InitTypeDef  SPI_InitStructure;

void SPI_Config(void);
void start_comm(uint8_t nss_pin);
void stop_comm(uint8_t nss_pin);
void send_byte(uint8_t cmd);
uint8_t read_byte(void);
uint16_t send_and_read_byte(uint16_t cmd);

#endif
