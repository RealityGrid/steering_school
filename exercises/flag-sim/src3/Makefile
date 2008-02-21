CC=gcc

TARGET=flag

LIBS=-lm

include $(REG_STEER_HOME)/Makefile.include
include $(REG_STEER_HOME)/make/Makefile.$(ARCH)

.c.o:
	$(CC) $(C_FLAGS) $(STEER_INCLUDES) $(GLOBUS_INCLUDES) -c $< -o $@

$(TARGET): $(REG_STEER_HOME)/lib$(NBIT)/$(LIB_NAME) $(TARGET).o
	$(CC) $(C_FLAGS) flag_utils.c $(TARGET).o -o $@ $(STEER_LIBS) \
	-lReG_Steer -lReG_Steer_SOAP $(STEER_LDFLAGS)
	
clean:
	rm *.o *~

clean_bin:
	flag
