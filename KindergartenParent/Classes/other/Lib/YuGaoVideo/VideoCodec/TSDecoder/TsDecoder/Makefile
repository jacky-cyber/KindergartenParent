CC=i686-apple-darwin10-gcc-4.2.1
CFLAGS= -g -DUNIX -lstdc++ -march=i386

OBJECTS = ./TsDecoder.o ./TsDecoderInterface.o ./TsDecoderManager.o

#-I$(LINUX_INC) -I$(CAPPATH)
#CFLAGS += -I./include -I../include -I./common -I./channel -I./camera -I./tools -I./viewer -I./snmp

# Name of the executable
EXENAME = ./libTsDecoder.a

EXTLIBS = -lstdc++  -lpthread #-lVigoLog
EXTLIBSPATH = 
 
.cpp.o:
	$(CC) -c  $(CFLAGS)  -D__cplusplus $< -o $@
.c.o:
	$(CC) -c  $(CFLAGS)   $< -o $@

# Builds all object files and executable
${EXENAME} : $(OBJECTS) 
	$(AR) rc $@ $(OBJECTS)
	
# Clean up
clean :
	rm -f $(OBJECTS) 
	rm -f $(EXENAME)


