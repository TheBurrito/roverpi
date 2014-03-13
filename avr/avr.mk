#AVR_MCU is used for specifying the controller for avr-gcc calls
AVR_MCU= atmega2560

#The AVRDUDE_* variables configure how avrdude is run
AVRDUDE_BAUD= 115200
AVRDUDE_PORT= /dev/ttyACM0
AVRDUDE_PRG= stk500v2
AVRDUDE_PART= m2560

#These flags override the normal flags and ensure a properly compile AVR hex
CXXFLAGS= -g -Os -Wall -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=$(AVR_MCU)
CFLAGS= -Wall
LDFLAGS= -Os -Wl,--gc-sections,--relax -mmcu=$(AVR_MCU) -lm
HEXFLAGS= -R .eeprom -O ihex


#Lines below this point should not be edited and ensure hex compiling and
#uploading work appropriately.

CLEANFILES+= *.elf 

#AVR project, establish CROSS_COMPILE variable for AVR-*
CROSS_COMPILE= avr-

define hex
	@echo "    HEX $@"
	$(quiet)  $(LD) -o $(BUILD_DIR)/$(subst .hex,.elf,$@) $(foreach file,$(call local,$^),$(BUILD_DIR)/$(notdir $(file)))
	$(quiet)  $(HEX) $(BUILD_DIR)/$(subst .hex,.elf,$@) $(BUILD_DIR)/$@
	$(mv) $(BUILD_DIR)/$@ $(BIN_DIR)
endef

define avrdude
	@echo "    UPLOAD $(notdir $^)"
	$(quiet) $(AVRDUDE) -b$(AVRDUDE_BAUD) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PRG) -p $(AVRDUDE_PART) -U flash:w:$(BIN_DIR)/$(notdir $^)
endef

%.hex: %.o
	$(hex)

up-%: %
	$(avrdude)

%.hex: %.cpp
