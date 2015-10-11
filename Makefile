
PLUGIN_NAME = arduino

HEADERS =  arduino_serial.h \
					 arduino.h 

SOURCES = arduino_serial.cpp \
					arduino.cpp \
					moc_arduino.cpp

LIBS = 

### Do not edit below this line ###

include $(shell rtxi_plugin_config --pkgdata-dir)/Makefile.plugin_compile
