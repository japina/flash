#include "stm32f0xx.h"
#include "stm32f0xx_spi.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx.h"
#include "flash.h"
#include "delay.h"
#include "pins.h"
#include "spi.h"

#define WRITE_PROTECT A2

void blinker (void){
	pinMode(A10, OUT);
	while(1){
		digitalWrite(A10, HIGH);
		Delay_ms(1000);
		digitalWrite(A10, LOW);
		Delay_ms(1000);
	}
}

int main(void)
{

	uint8_t* data;
	uint8_t menu_id;
	uint8_t mem_type;
	uint8_t mem_cap;

  init_timer();
	init_flash();

	pinMode(A10, OUT);

	flash_id(&menu_id, &mem_type, &mem_cap);
	chip_erase();

	uint8_t wdata[] = {170, 180, 190};

	write_2_memory(0x0, wdata, 3);
	data = malloc(3);
	read_memory(0x0, data, 3);
	blinker();
}
