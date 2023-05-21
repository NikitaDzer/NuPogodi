#-----------------------
# Compiler/linker flags
#-----------------------

CC = arm-none-eabi-gcc

CFLAGS = 				\
	-std=c18 			\
	-march=armv6-m 		\
	-mcpu=cortex-m0 	\
	-flto=auto 			\
	-Ofast

LDFLAGS = 																\
	-flto=auto 															\
	-Wl,--start-group -lgcc -lc -lg -Wl,--end-group -Wl,--gc-sections 	\
	-march=armv6-m 														\
	-mcpu=cortex-m0 													\
	-Wl,--warn-common 													\
	-Wl,--fatal-warnings 												\
	-Wl,-z,max-page-size=8 												\
	-Wl,-T,entry.lds

ifeq ($(DEBUG),1)
	CFLAGS += -g
endif

#-------
# Files
#-------

SOURCES = 	\
	entry.S \
	main.c

SOURCES_SRC = 		\
	src/bit.c       \
	src/lib.c 		\
	src/init.c 		\
	src/driver.c 	\
	src/window.c 	\
	src/utils.c  	\
	src/button.c 	\
	src/scene.c 	\
	src/assets.c 	\
	src/game.c 		\
	src/seg7.c 		\
	src/delay.c

OBJECTS_HALFWAY_DONE = $(SOURCES:%.c=build/%.o)
OBJECTS_FROM_SRC 	 = $(SOURCES_SRC:src/%.c=build/%.o)
OBJECTS              = $(OBJECTS_HALFWAY_DONE:%.S=build/%.o)


EXECUTABLE_FLASH = build/reaction.elf
BINARY_FLASH     = build/reaction.bin

#---------------
# Build scripts
#---------------

all: $(EXECUTABLE_FLASH) $(BINARY_FLASH) $(SOURCES) $(SOURCES_SRC)

$(EXECUTABLE_FLASH): $(OBJECTS) $(OBJECTS_FROM_SRC)
	$(CC) $(LDFLAGS) $(OBJECTS) $(OBJECTS_FROM_SRC) -o $@

$(BINARY_FLASH): $(EXECUTABLE_FLASH)
	arm-none-eabi-objcopy -O binary $< $@

build/%.o: %.c
	@mkdir -p build
	$(CC) $(CFLAGS) -o $@ -c $<

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -o $@ -c $<

build/%.o: %.S
	@mkdir -p build
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf build

#----------------------
# Hardware interaction
#----------------------

flash: $(BINARY_FLASH)
	st-flash write $(BINARY_FLASH) 0x08000000

GDB_FLAGS = \
	--eval-command="set architecture arm" \
	--eval-command="set gnutarget elf32-littlearm" \
	--eval-command="target remote localhost:1234" \
	--eval-command="file $(EXECUTABLE_FLASH)"

hardware: $(EXECUTABLE_FLASH)
	st-util -p 1234

gdb: $(BINARY_FLASH)
	arm-none-eabi-gdb $(GDB_FLAGS)

.PHONY: all clean flash hardware gdb
