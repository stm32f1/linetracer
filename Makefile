# Please specify the path to STM32F10x_StdPeriph_Driver/Libraries
LIBRARIES_PATH	:= ../../libraries

ARMGNU		:= arm-none-eabi
CC		:= $(ARMGNU)-gcc
CXX		:= $(ARMGNU)-g++
LD		:= $(ARMGNU)-ld
DEBUGGER	:= $(ARMGNU)-gdb
SIZE		:= $(ARMGNU)-size
READELF		:= $(ARMGNU)-readelf
OBJCOPY		:= $(ARMGNU)-objcopy
OBJDUMP		:= $(ARMGNU)-objdump

buildtype	:= debug

DEBUG_FLAGS	:= -g -O0 -DDEBUG
RELEASE_FLAGS	:= -O0 -DNDEBUG

SUB_DIRECTORY	:= ./common
TOF_DIRECTORY	:= ./$(SUB_DIRECTORY)/tof
MUSIC_DIRECTORY	:= ./$(SUB_DIRECTORY)/music

CFLAGS		:= -Wall -Wextra -Wno-unused-parameter -Wconversion -std=c99
CFLAGS		+= -mthumb -mcpu=cortex-m3 -march=armv7-m -mfix-cortex-m3-ldrd -lc -lm -lg
CFLAGS		+= -D_GNU_SOURCE
CFLAGS		+= -DSTM32F10X_HD

CXXFLAGS	:= -Wall -Wextra -Wno-unused-parameter -Wconversion -std=c++14
CXXFLAGS	+= -mthumb -mcpu=cortex-m3 -march=armv7-m -mfix-cortex-m3-ldrd -lc -lm -lg
CXXFLAGS	+= -D_GNU_SOURCE
CXXFLAGS	+= -DSTM32F10X_HD

SOURCES_DIR	:= ./
SOURCES_C	:= $(wildcard $(SOURCES_DIR)*.c)
SOURCES_C	+= $(wildcard $(SUB_DIRECTORY)/*.c)
SOURCES_C	+= $(wildcard $(TOF_DIRECTORY)/*.c)
SOURCES_C	+= $(wildcard $(MUSIC_DIRECTORY)/*.c)
SOURCES_ASM	:= $(wildcard $(SOURCES_DIR)*.s)
SOURCES_ASM	+= $(wildcard $(SUB_DIRECTORY)/*.s)


OBJ_DIR		:= ./obj
OBJECTS		:= $(addprefix $(OBJ_DIR)/$(buildtype)/, $(subst ./,,$(patsubst %.c,%.o,$(SOURCES_C))))
OBJECTS		+= $(addprefix $(OBJ_DIR)/$(buildtype)/, $(subst ./,,$(patsubst %.s,%.o,$(SOURCES_ASM))))

DEP_DIR		:= ./obj
DEPENDS		:= $(addprefix $(DEP_DIR)/$(buildtype)/, $(subst ./,,$(patsubst %.c,%.d,$(SOURCES_C))))


STARTUP_DIR	:= $(LIBRARIES_PATH)/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/TrueSTUDIO
STARTUP_ASM	:= $(STARTUP_DIR)/startup_stm32f10x_hd.s
STARTUP_OBJ	:= $(OBJ_DIR)/startup_stm32f10x_hd.o

INCLUDE		:= -I./
INCLUDE		+= -I./$(SUB_DIRECTORY)
INCLUDE		+= -I./$(TOF_DIRECTORY)
INCLUDE		+= -I./$(MUSIC_DIRECTORY)
INCLUDE		+= -I$(LIBRARIES_PATH)/CMSIS/CM3/DeviceSupport/ST/STM32F10x
INCLUDE		+= -I$(LIBRARIES_PATH)/STM32F10x_StdPeriph_Driver/inc
INCLUDE		+= -I$(LIBRARIES_PATH)/CMSIS/CM3/CoreSupport

LIBS_DIR	:= -L$(LIBRARIES_PATH)/STM32F10x_StdPeriph_Driver/lib
LIBS_DIR	+= -L /usr/arm-none-eabi/lib/armv7-m
LIBS		:= -lstmperiph


TARGET_DIR	:= ./bin
TARGET_NAME	?= target
TARGET_ELF	:= $(TARGET_DIR)/$(buildtype)/$(TARGET_NAME).elf
TARGET_BIN	:= $(TARGET_DIR)/$(buildtype)/$(TARGET_NAME).bin
TARGET_HEX	:= $(TARGET_DIR)/$(buildtype)/$(TARGET_NAME).hex
TARGET_LIST	:= $(TARGET_DIR)/$(buildtype)/$(TARGET_NAME).list
TARGET		:= $(TARGET_ELF)
TARGETS		:= $(TARGET_ELF) $(TARGET_BIN) $(TARGET_HEX) $(TARGET_LIST)

GDB_COMMAND_FILE	:= ./$(SUB_DIRECTORY)/autowrite
GDB_DEBUG_FILE		:= ./$(SUB_DIRECTORY)/autodebug


ifeq ($(buildtype), debug)
	CFLAGS += $(DEBUG_FLAGS)
else ifeq ($(buildtype), release)
	CFLAGS += $(RELEASE_FLAGS)
else
	$(echo buildtype must be debug or release)
	$(exit 1)
endif


all : build size

build : $(TARGETS)

run : $(TARGET) $(GDB_COMMAND_FILE)
	$(DEBUGGER) $< -batch -nx -x $(GDB_COMMAND_FILE)

write : $(TARGET) $(GDB_COMMAND_FILE)
	$(DEBUGGER) $< -batch -nx -x $(GDB_COMMAND_FILE)

debug : $(TARGET) $(GDB_DEBUG_FILE)
	$(DEBUGGER) $(TARGET) -nx -x $(GDB_DEBUG_FILE)

-include $(DEPENDS)


$(TARGET_ELF) : $(OBJECTS) $(STARTUP_OBJ)
	mkdir -p `dirname $@`
	$(CC) $^ $(CFLAGS) $(INCLUDE) $(LIBS_DIR) $(LIBS) -o $@ -Wl,-T$(SUB_DIRECTORY)/linker_script.ld -Wl,-M=$(TARGET).map


$(OBJ_DIR)/$(buildtype)/%.o : $(SOURCES_DIR)/%.c Makefile
	mkdir -p `dirname $@`
	$(CC) -c -MMD -MP $< $(CFLAGS) $(INCLUDE) $(LIBS_DIR) $(LIBS) -o $@

$(OBJ_DIR)/$(buildtype)/%.o : $(SOURCES_DIR)/%.s Makefile
	mkdir -p `dirname $@`
	$(CC) -c -MMD -MP $< $(CFLAGS) $(INCLUDE) $(LIBS_DIR) $(LIBS) -o $@

$(STARTUP_OBJ) : $(STARTUP_ASM) Makefile
	mkdir -p `dirname $@`
	$(CC) -c $< $(CFLAGS) -o $@

%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

%.list: %.elf
	$(OBJDUMP) -x -S $< > $@

size: $(TARGET)
	@echo ""
	$(SIZE) $<
	@echo ""

list : $(TARGET_LIST)

clean :		
	rm -f -r $(TARGETS) $(OBJECTS) $(DEPENDS) $(TARGET_DIR) $(OBJ_DIR)

.PHONY: all build run write debug size list clean
