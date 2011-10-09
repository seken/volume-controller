CROSS_COMPILE = arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy

noinst_PROGRAMS = firmware.flash firmware.ram firmware.jtag firmware.flash.bin firmware.ram.bin firmware.jtag.bin

MCU := STM32F103RB
BOARD ?= maple
DENSITY = STM32_MEDIUM_DENSITY

SOURCES = \
	main.cpp

ALL_CXXFLAGS = $(FLAGS) $(MAPLE_CXXFLAGS)

ALL_CFLAGS = $(FLAGS) -std=gnu99 $(MAPLE_CFLAGS)

FLAGS = \
	-Os -g3 -gdwarf-2 -Wall \
	-ffunction-sections -fdata-sections -Wl,--gc-sections \
	-mcpu=cortex-m3 -mthumb -fshort-enums -mfloat-abi=soft \
	-DBOARD_$(BOARD) -DMCU_$(MCU) -D$(DENSITY) -DVECT_TAB_BASE

firmware_flash_LDFLAGS = $(MAPLE_LDADD) $(MAPLE_FLASH) -T$(MAPLE_DIR)/support/ld/$(BOARD)/flash.ld --gc-sections \
	-Map,main.flash.map

firmware_flash_CFLAGS = $(ALL_CFLAGS)

firmware_flash_CXXFLAGS = $(ALL_CXXFLAGS)

firmware_flash_SOURCES = $(SOURCES)

firmware_ram_LDFLAGS = $(MAPLE_LDADD) $(MAPLE_FLASH) -T$(MAPLE_DIR)/support/ld/$(BOARD)/ram.ld --gc-sections \
	-Map,main.ram.map

firmware_ram_CFLAGS = $(ALL_CFLAGS)

firmware_ram_CXXFLAGS = $(ALL_CXXFLAGS)

firmware_ram_SOURCES = $(SOURCES)

firmware_jtag_LDFLAGS = $(MAPLE_LDADD) $(MAPLE_FLASH) -T$(MAPLE_DIR)/support/ld/$(BOARD)/jtag.ld --gc-sections \
	-Map,main.jtag.map

firmware_jtag_CFLAGS = $(ALL_CFLAGS)

firmware_jtag_CXXFLAGS = $(ALL_CXXFLAGS)

firmware_jtag_SOURCES = $(SOURCES)

firmware.flash.bin: firmware.flash
	$(OBJCOPY) -Obinary $< $@

firmware.jtag.bin: firmware.jtag
	$(OBJCOPY) -Obinary $< $@

firmware.ram.bin: firmware.ram
	$(OBJCOPY) -Obinary $< $@
