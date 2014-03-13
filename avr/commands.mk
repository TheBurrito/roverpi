#The following capitalized variables store the command-line commands to perform various functions
#These can be edited to customize how/which commands are called.

CC=$(CROSS_COMPILE)gcc $(CFLAGS)
CXX=$(CROSS_COMPILE)g++ $(CXXFLAGS)
LD=$(CROSS_COMPILE)g++ $(LDFLAGS)
AR=$(CROSS_COMPILE)ar $(ARFLAGS)
HEX=$(CROSS_COMPILE)objcopy $(HEXFLAGS)
AVRDUDE=avrdude
RM=rm -rf
CP=cp -r
MV=mv
MKDIR=mkdir -p

#These lower cased commands wrap the above calls to control verbosity of the
#output. In general, none of the lower cased variables or commands should be
#changed.

rm=$(quiet)$(RM)
cp=$(quiet)$(CP)
mv=$(quiet)$(MV)
mkdir=$(quiet)$(MKDIR)

makeopts=
make=@$(MAKE) $(makeopts)

log=$(if $(quiet),,$(warning $1))
local=$(subst $(CURDIR)/,,$1)

#These functions provide pretty output for build steps and optionally display
#the full command used in the build step.

define cc
	@echo "    CC $(notdir $@)"
	$(quiet)  $(CC) -o $(BUILD_DIR)/$(subst .c,.o,$(notdir $^)) -c $(call local,$^)
endef

define cxx
	@echo "    CXX $(notdir $@)"
	$(quiet)  $(CXX) -o $(BUILD_DIR)/$(subst .cpp,.o,$(notdir $^)) -c $(filter-out %.a,$(call local,$^))
endef

define ld
	@echo "    LD $@"
	$(quiet)  $(LD) -o $(BUILD_DIR)/$@ $(foreach file,$(call local,$^),$(BUILD_DIR)/$(notdir $(file)))
	$(mv) $(BUILD_DIR)/$@ $(BIN_DIR)
endef

define ar
	@echo "    AR $@"
	$(quiet)  $(AR) $(BUILD_DIR)/$@ $(foreach file,$(call local,$^),$(BUILD_DIR)/$(notdir $(file)))
	$(cp) $(BUILD_DIR)/$@ $(LIB_DIR)
endef
