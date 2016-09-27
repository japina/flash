# put your *.o targets here, make should handle the rest!

SRCS = main.c flash.c spi.c
LIBS_PATH = /Users/bostjan/Documents/n-ka/libs33

# all the files will be generated with this name (main.elf, main.bin, main.hex, etc)

PROJ_NAME=main

# that's it, no need to change anything below this line!

###################################################

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

CFLAGS  = -g -O0 -Wall -T$(LIBS_PATH)/stm32f030c6_flash.ld --specs=nosys.specs
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m0 -mthumb-interwork

###################################################

vpath %.c src

ROOT=$(shell pwd)

CFLAGS += -Iinc -I$(LIBS_PATH) -I$(LIBS_PATH)/inc
CFLAGS += -I$(LIBS_PATH)/inc/core -I$(LIBS_PATH)/inc/peripherals

SRCS += $(LIBS_PATH)/startup_stm32f030xc.s  # add startup file to build
SRCS += $(LIBS_PATH)/system_stm32f0xx.c

OBJS = $(SRCS:.c=.o)

###################################################

.PHONY: proj

#all: proj	 lib
all: proj

#lib:
#	$(MAKE) -C lib


proj: 	$(PROJ_NAME).elf

$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^  -o $@ -L$(LIBS_PATH) -ltft -lstm32f0 -ltiming -lpins
	#$(CC) $(CFLAGS) $^ -o $@ -L../libs -lstm32f0
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin

clean:
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin

upload:
	st-flash --reset write main.bin 0x8000000
