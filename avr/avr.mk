#AVR_MCU is used for specifying the controller for avr-gcc calls
AVR_MCU= atmega2560

#The AVRDUDE_* variables configure how avrdude is run
AVRDUDE_BAUD= -b 19200
AVRDUDE_PORT= -P /dev/ttyUSB0
AVRDUDE_PRG= -c stk500v1
AVRDUDE_PART= -p m2560

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
	@mkdir -p $(BIN_DIR)
	$(mv) $(BUILD_DIR)/$@ $(BIN_DIR)
endef

define avrdude
	@echo "    UPLOAD $(notdir $^)"
	$(quiet) $(AVRDUDE)  $(AVRDUDE_BAUD) $(AVRDUDE_PORT) $(AVRDUDE_PRG) $(AVRDUDE_PART) -U flash:w:$(BIN_DIR)/$(notdir $^)
endef

#If the target is a hex, we get there by first creating a matching object file
%.hex: %.o
	$(hex)

#auto target for any existing .hex target to use avrdude to upload hex file
up-%.hex: %.hex
	$(avrdude)

#also make hex files depend on their matching cpp files
%.hex: %.cpp
