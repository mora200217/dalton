# *************************************************
# IMPORTANTE PARA PODER COMPILAR ESTE EJEMPLO:
# En el aechivo:litex/litex/soc/software/common.mak
# cambiar de:
# ifeq ($(LTO), 1)
# COMMONFLAGS += -flto
# endif
# a:
# COMMONFLAGS += -flto
# *************************************************

TARGET=
BUILD_DIR=../build/$(TARGET)/
include $(BUILD_DIR)/software/include/generated/variables.mak
include $(SOC_DIRECTORY)/software/common.mak

MODULE_SOURCES := $(wildcard modules/*.c)
MODULE_OBJECTS := $(MODULE_SOURCES:modules/%.c=%.o)

UTILS_SOURCES := $(wildcard utils/*.c)
UTILS_OBJECTS := $(UTILS_SOURCES:utils/%.c=%.o)

CFLAGS += -I$(BUILD_DIR)/software/include

OBJECTS= main.o crt0.o $(MODULE_OBJECTS) $(UTILS_OBJECTS)


CFLAGS += -Iinclude
CFLAGS += -Imodules
CFLAGS+=-Iutils


ifdef WITH_CXX
	CFLAGS += -DWITH_CXX
endif


all: hello_world.bin
%.bin: %.elf
	$(OBJCOPY) -O binary $< $@
	chmod -x $@

hello_world.elf: $(OBJECTS)
	$(CC) $(LDFLAGS) \
		-T linker.ld \
		-N -o $@ \
		$(OBJECTS) \
		-L$(BUILD_DIR)/software/libbase \
		-L$(BUILD_DIR)/software/libcompiler_rt \
		-L$(BUILD_DIR)/software/libc/ \
		-lc -lbase -lcompiler_rt
	chmod -x $@

main.o: main.c
	$(compile)



# kalman.o: kalman/kalman_filter.c
# 	$(compile)


crt0.o: $(CPU_DIRECTORY)/crt0.S
	$(assemble)
%.o: %.c
	$(compile)

%.o: modules/%.c
	$(compile)

%.o: utils/%.c
	$(compile)

%.o: %.S
	$(assemble)
clean:
	$(RM) $(OBJECTS) *.elf *.bin .*~ *~ *.map *.d
.PHONY: clean
