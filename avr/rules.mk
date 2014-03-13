#These are the command-line targets that are specialized for the non-recursive
#make used with this build system.

.PHONY: all

all:

.PHONY: debug
debug: CXXFLAGS+= -DDEBUG -ggdb
debug: all

.PHONY: clean
clean: scratch=$(shell find ./* -name "*~")
clean:
	$(rm) $(BUILD_DIR)/* $(scratch)

.PHONY: realclean
realclean: clean
	$(rm) $(LIB_DIR)/* $(BIN_DIR)/*

#generic rules for working on individual target files
%.o: %.c
	$(cc)
	
%.o: %.cpp
	$(cxx)

%: %.o
	$(ld)

%: %.cpp

%.a:
	$(ar)
