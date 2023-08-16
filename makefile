
OBJECTS   = $(CPPSRCS:.cpp=.o) ac_cab_uae_lib_libsys.a $(CORE)/sys.a

#CORE = ../core
CSRCS = $(wildcard $(CORE)/*.c) $(wildcard $(CORE)/stm32f4xx/src/*.c) $(CORE)/miIP/stm32f4x7_eth.c
CSRCS += $(wildcard $(CORE)/usbd/*.c) $(CORE)/usbotg/usb_dcd.c $(CORE)/usbotg/usb_dcd_int.c $(CORE)/usbotg/usb_core.c $(CORE)/usbd/Class/cdc/usbd_cdc_core.c $(CORE)/usbuart/usbd_desc.c
ASRCS =  $(wildcard $(CORE)/*.S)
CPPSRCS = $(wildcard $(CORE)/*.cpp) $(wildcard $(CORE)/uart/*.cpp) $(wildcard $(CORE)/miIP/*.cpp) $(wildcard $(CORE)/usbuart/*.cpp) $(wildcard $(CORE)/i2c/*.cpp) $(wildcard $(CORE)/timer/*.cpp)  $(wildcard $(CORE)/can/*.cpp)  
CPPSRCS += $(wildcard $(COMM_LIB)/*.cpp)  $(wildcard $(COMM_LIB)/mvb_hc/*.cpp)

INCDIRS = $(CORE) $(CORE)/stm32f4xx/inc $(CORE)/can $(CORE)/i2c $(CORE)/timer $(CORE)/uart $(CORE)/spi src  $(CORE)/canopen  $(CORE)/CP
INCDIRS += $(CORE)/miIP/include $(CORE)/mvb 
INCDIRS += $(CORE)/usbd $(CORE)/usbd/Class/cdc $(CORE)/usbotg $(CORE)/usbuart
INCDIRS += $(COMM_LIB)
INCDIRS += $(COMM_LIB)/mvb_hc

ifeq ($(BOOT),on)
CPPSRCS += src/cpu_can_iap.cpp src/main.cpp src/sys_init.cpp src/cpu_main.cpp src/lan_driver.cpp
BASE_DEF = -D_BOOT_ -D_TOS_  
else
CPPSRCS += $(wildcard src/*.cpp) $(wildcard src/mvb_gatewav/*.cpp)  $(wildcard src/W5500/*.cpp) 
BASE_DEF = -D_TOS_ -D_BTIM4_ -D_BTIM2_ -D_BTIM5_ -D_I2C1_ -D_UART3_ -D_EQU_MANAGEMENT_ -D_SPI_FRAM_ -D_TCP_ -D_UART4_ -D_WDT_ -D_UART4_ 
INCDIRS += src/mvb_gatewav src/W5500
INCDIRS += src/canopen
INCDIRS += src/canopen/OD
INCDIRS += src/CP


#trdp
CPPSRCS +=  $(wildcard $(CORE)/trdp/dg/common/src/*.cpp) $(wildcard $(CORE)/trdp/dg/target/src/*.cpp) 
INCDIRS += $(CORE)/trdp/dg/common/inc $(CORE)/trdp/dg/target/inc 
INCDIRS += src/logic src/CP/trdp
CPPSRCS += $(wildcard src/CP/trdp/*.cpp)



CPPSRCS += $(wildcard $(CORE)/g0/*.cpp)   $(wildcard $(CORE)/driver/*/*.cpp)  $(wildcard src/canopen/src/*.cpp) $(wildcard src/canopen/OD/*.cpp) $(wildcard src/CP/*.cpp)  
INCDIRS += $(CORE)/g0 $(CORE)/driver/MB85RS256B $(CORE)/driver/MCP23S17 $(CORE)/driver/MCP4728  $(CORE)/driver/PCA8574 $(CORE)/driver/SST26VF064B $(CORE)/driver/DS2411 $(CORE)/driver/PCA9629


#CPPSRCS +=  $(wildcard $(CORE)/trdp/dg/common/src/*.cpp) $(wildcard $(CORE)/trdp/dg/target/src/*.cpp) 
#INCDIRS += $(CORE)/trdp/dg/common/inc $(CORE)/trdp/dg/target/inc 
#CPPSRCS +=  $(wildcard src/trdp_dg/*.cpp)
#INCDIRS += src/trdp_dg src/trdp_dg


endif

#  Compiler Options

OPTIMIZATION = 1
CPU_TYPE = cortex-m4

GCFLAGS = -O$(OPTIMIZATION) -gdwarf-2 -mcpu=$(CPU_TYPE) -mthumb -mthumb-interwork -mlong-calls -ffunction-sections -fdata-sections  -Wall -Wextra -Wimplicit -Wcast-align -Wpointer-arith -Wredundant-decls -Wshadow -Wcast-qual -Wcast-align -Wnested-externs --std=gnu99
OCFLAGS = --strip-unneeded

GCFLAGS += $(patsubst %,-I%,$(INCDIRS)) -I.
DEFINED=
GPFLAGS = -O$(OPTIMIZATION) -gdwarf-2 -mcpu=$(CPU_TYPE) -mthumb -mthumb-interwork -mlong-calls -ffunction-sections -fdata-sections -Wextra -Wcast-align -Wpointer-arith -Wredundant-decls -Wshadow -Wcast-qual -Wcast-align -fno-rtti -fno-exceptions $(DEFINED)
#-Wall -Wextra -Wcast-align -Wpointer-arith -Wredundant-decls -Wshadow -Wcast-qual -Wcast-align

GPFLAGS += $(patsubst %,-I%,$(INCDIRS)) -I. 
LDFLAGS = -mcpu=$(CPU_TYPE) -mthumb -Wl,--gc-sections -mthumb-interwork

#GCFLAGS += -l $(CORE)/pcb_check_libsys2.a
#GPFLAGS += -l $(CORE)/pcb_check_libsys2.a

#-nostartfiles

ASFLAGS = -c -Os -I. -Wall -mthumb -ffunction-sections -fdata-sections -fmessage-length=0 -mcpu=$(CPU_TYPE) -D__ASSEMBLY__ -x assembler-with-cpp

#  Compiler/Assembler/Linker Paths
GCC = arm-none-eabi-gcc
GPP = arm-none-eabi-g++
AS = arm-none-eabi-g++
LD = arm-none-eabi-g++
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
REMOVE = del
SIZE = arm-none-eabi-size

#########################################################################
#可能用到的FLAG
# -D-D_BOOT_   BOOTLoader
# -D_TOS_          OS
# -D_WDT_          看门狗
#########################################################################


CPU:: DEF_CPU
DEF_CPU:GPFLAGS += $(BASE_DEF) $(APP_DEF)
DEF_CPU:GCFLAGS += $(BASE_DEF) $(APP_DEF)
DEF_CPU: all
	-@echo $(OBJECTS)

all:: $(PROJECT).hex $(PROJECT).bin


$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) $(OCFLAGS) -O binary $(PROJECT).elf $(PROJECT).bin


$(PROJECT).hex: $(PROJECT).elf
#	$(OBJCOPY) -R .stack -O ihex $(PROJECT).elf $(PROJECT).hex
	$(OBJCOPY) $(OCFLAGS) -O ihex $(PROJECT).elf $(PROJECT).hex

$(PROJECT).elf: $(LSCRIPT) $(OBJECTS)
	$(LD) $(LDFLAGS) -T$(LSCRIPT) $(LDOBJECTS) $(OBJECTS) -o $(PROJECT).elf
#	$(OBJDUMP) -D $(PROJECT).elf > $(PROJECT).S
	$(SIZE) $(PROJECT).elf

stats: $(PROJECT).elf
	$(SIZE) $(PROJECT).elf

clean:
	$(REMOVE) /s/q *.o *.elf

#########################################################################
#  Default rules to compile .c and .cpp file to .o
#  and assemble .s files to .o

.c.o :$@ $<
	$(GCC) $(GCFLAGS) -c $< -o $(<:.c=.o)
.cpp.o :
	$(GPP) $(GPFLAGS) -c $< -o $(<:.cpp=.o)


.S.o :
	$(AS) $(ASFLAGS) -c $< -o $(<:.S=.o)

#.S.o :
#	$(AS) $(ASFLAGS) -o $(<:.S=.o) $< > $(<:.t=.lst)
#	$(AS) $(ASFLAGS) -o $(PROJECT)_crt.o $< > $(PROJECT)_crt.lst

#########################################################################
