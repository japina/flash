#include "spi.h"
#include "flash.h"
#include "delay.h"

void init_flash(void){
	SPI_Config();
}

void flash_id(uint8_t* menu_id, uint8_t* mem_type, uint8_t* mem_cap){
  // read flash ID
  start_comm(A4);
	send_byte(RDID);
	*menu_id = read_byte();
	*mem_type = read_byte();
	*mem_cap = read_byte();
  stop_comm(A4);
	// stop read flash ID
}

void chip_erase(void){
  uint8_t data;
  // enable write + write data
  start_comm(A4);
  send_byte(WREN);
  stop_comm(A4);
  // stop enable write
  // start chip erase
  start_comm(A4);
  send_byte(CE);
  stop_comm(A4);
  // stop chip erase
  start_comm(A4);
  send_byte(RDSR);
  data = read_byte();
  while((data & 0x02) != 0x0){ // wait until the clear command is finished
    send_byte(RDSR);
    data = read_byte();
  }
  stop_comm(A4);
}

void write_2_memory(uint32_t start_address, uint8_t* data_list, uint8_t size){
  uint8_t i;
  //uint8_t addr;
  // enable write + write data
  start_comm(A4);
  send_byte(WREN);
    stop_comm(A4);
  // stop enable write
  // write data to address
    start_comm(A4);
    send_byte(PP);
    //addr = start_address & 0x0f;
    send_byte(start_address & 0x0f);
    send_byte((start_address & 0xf0)>>8);
    send_byte((start_address & 0xf00)>>16);
    i=0;
    while(i<size){
      send_byte(data_list[i++]);
    }
    stop_comm(A4);
}

void read_memory(uint32_t start_address, uint8_t *data, uint8_t size){
  uint8_t i;
  //uint32_t addr;
  // start read
  start_comm(A4);
  //i=0;
  send_byte(READ);
  //addr = start_address & 0x0f;
  send_byte(start_address & 0x0f);
  send_byte((start_address & 0xf0)>>8);
  send_byte((start_address & 0xf00)>>16);
  i=0;
  while(i<size){
    data[i] = read_byte();
    i++;
  }
  data[i]='\0';
  stop_comm(A4);
// start read - stop
}
