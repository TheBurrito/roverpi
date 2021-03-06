This sub project contains the code for programming the ATmega2560 on the RoverPi

Source code is available at https://github.com/TheBurrito/roverpi

The project is organized into:
-base libraries: ./roverpi, ./include/roverpi
-test programs: ./test
-prewritten firmware: ./firmware

Other folders have varying uses:

./build - Contains all of the intermediate files for compiling
./lib - Contains archived library targets (*.a files)
./bin - Contains all compiled executable code (*.hex files)


***BUILDING

Running make with no parameters will build all targets. Alternatively targets
can be specified by name (see the targets.txt file for targets and info). Make
can also be invoked from any sub directory containing a Makefile. Note that if
targets in the sub directory and below depend on other targets above the current
path that have not been built yet then building will fail.


***UPLOADING

The Makefile build system used here provides an easy mechanism for uploading any
*.hex targets built. A special rule in the form of up-*.hex will use the avrdude
parameters found in avr.mk to upload to the AVR. This rule will also result in
the hex file be compiled if any of the source files are newer.

Use the AVRDUDE_* variables in the avr.mk file to configure how avrdude uploads
the hex file to the board.

- AVRDUDE_PRG specifies the programmer used. This should be "stk500v2" for using
    the Arduino bootloader and a serial port (FTDI).
- AVRDUDE_PORT specifies the port to use for programming


***BUILD SYSTEM (Makefiles)

Build system available at https://github.com/TheBurrito/buildsys

Check there for information regarding the manner in which the makefiles work.
