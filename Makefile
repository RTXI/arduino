
PLUGIN_NAME = Arduino

HEADERS =  arduino_serial.h Arduino.h 

SOURCES = arduino_serial.cpp Arduino.cpp 

LIBS = 

### Do not edit below this line ###

include $(shell rtxi_plugin_config --pkgdata-dir)/Makefile.plugin_compile
