
PREFIX := arm-none-eabi-

CC			= $(PREFIX)gcc
LD			= $(PREFIX)ld
AS        	= $(PREFIX)gcc -x assembler-with-cpp
OBJCOPY		= $(PREFIX)objcopy
OBJDUMP		= $(PREFIX)objdump
AR			= $(PREFIX)ar
SIZE		= $(PREFIX)size
HEX       	= $(OBJCOPY) -O ihex
BIN       	= $(OBJCOPY) -O binary -S

TOP_DIR = ../../..

TARGET = sharlock

BUILD_DIR = build

DEVICE = stm32f4xx

NETWORK = esp8266

USE_DTLS = yes

# mcu
# cpu
CPU = -mcpu=cortex-m4
# fpu
FPU = -mfpu=fpv4-sp-d16
# float-abi
FLOAT-ABI = -mfloat-abi=hard
# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

#file 
LD_SCRIPT = $(TOP_DIR)/targets/F429_fire/gcc/STM32F429IGTx.ld

#defines
C_DEFS = -Og -g -DWITH_AT_FRAMEWORK -DUSE_$(shell echo $(NETWORK)| tr a-z A-Z) -DLWM2M_LITTLE_ENDIAN \
		-DLWM2M_CLIENT_MODE -DAGENT_TINY_NOS -DSTM32F429_439xx  -DUSE_STDPERIPH_DRIVER\

ifeq ($(USE_DTLS), yes)
	C_DEFS += -DWITH_DTLS -DUSE_MBED_TLS -DMBEDTLS_CONFIG_FILE=\"los_mbedtls_config.h\"
endif		
		
ASM_SOURCE_s = $(TOP_DIR)/targets/F429_fire/driver/STM32F4xx_DSP_StdPeriph_Lib/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc_ride7/startup_stm32f429_439xx.s

#sources
DEIVCE_LIB_SRC = $(TOP_DIR)/targets/F429_fire/driver/STM32F4xx_DSP_StdPeriph_Lib/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_$(DEVICE).c \
		$(TOP_DIR)/targets/F429_fire/driver/STM32F4xx_DSP_StdPeriph_Lib/STM32F4xx_StdPeriph_Driver/src/misc.c \
		$(TOP_DIR)/targets/F429_fire/driver/STM32F4xx_DSP_StdPeriph_Lib/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c \
		$(TOP_DIR)/targets/F429_fire/driver/STM32F4xx_DSP_StdPeriph_Lib/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_tim.c \
		$(TOP_DIR)/targets/F429_fire/driver/STM32F4xx_DSP_StdPeriph_Lib/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c \
		$(TOP_DIR)/targets/F429_fire/driver/STM32F4xx_DSP_StdPeriph_Lib/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_usart.c
C_SOURCES += $(DEIVCE_LIB_SRC)

HW_DRV_SRC = ${wildcard $(TOP_DIR)/targets/F429_fire/driver/*.c}
C_SOURCES += $(HW_DRV_SRC)

CORE_LIB_SRC = ${wildcard $(TOP_DIR)/core/hal_drv/*.c} \
			${wildcard $(TOP_DIR)/core/libs/*.c}
C_SOURCES += $(CORE_LIB_SRC)

AGENTTINY_SRC =  ${wildcard $(TOP_DIR)/agent_tiny/examples/*.c} \
				 ${wildcard $(TOP_DIR)/agent_tiny/osadapter/*.c} \
				 $(TOP_DIR)/agent_tiny/lwm2m_client/agenttiny.c \
				 $(TOP_DIR)/agent_tiny/lwm2m_client/atiny_log.c \
				 $(TOP_DIR)/agent_tiny/lwm2m_client/atiny_rpt.c \
				 $(TOP_DIR)/agent_tiny/lwm2m_client/commandline.c \
				 $(TOP_DIR)/agent_tiny/lwm2m_client/connection.c \
				 $(TOP_DIR)/agent_tiny/lwm2m_client/object_access_control.c \
				 $(TOP_DIR)/agent_tiny/lwm2m_client/object_binary_app_data_container.c \
				 $(TOP_DIR)/agent_tiny/lwm2m_client/object_device.c \
				 $(TOP_DIR)/agent_tiny/lwm2m_client/object_security.c \
				 $(TOP_DIR)/agent_tiny/lwm2m_client/object_server.c \
				 $(TOP_DIR)/agent_tiny/lwm2m_client/platform_adapter.c 
C_SOURCES += $(AGENTTINY_SRC)

LWM2M_SRC = ${wildcard $(TOP_DIR)/lwm2m/core/*.c} \
			${wildcard $(TOP_DIR)/lwm2m/core/er-coap-13/*.c} 
C_SOURCES += $(LWM2M_SRC)

USER_SRC = ${wildcard $(TOP_DIR)/targets/F429_fire/src/*.c}
C_SOURCES += $(USER_SRC)

NETWORK_SRC = $(TOP_DIR)/network/at_api_interface.c \
				$(TOP_DIR)/network/$(NETWORK).c
C_SOURCES += $(NETWORK_SRC)

ifeq ($(USE_DTLS),yes)
MBEDTLS_SRC = ${wildcard $(TOP_DIR)/security/mbedtls/mbedtls-2.6.0/library/*.c }
C_SOURCES += $(MBEDTLS_SRC)

MBEDTLS_PORT_SRC = ${wildcard $(TOP_DIR)/security/mbedtls/mbedtls_port/*.c }
C_SOURCES += $(MBEDTLS_PORT_SRC)
endif


#includes
DEIVCE_LIB_INC = -I$(TOP_DIR)/targets/F429_fire/driver/STM32F4xx_DSP_StdPeriph_Lib/CMSIS/Include \
				-I$(TOP_DIR)/targets/F429_fire/driver/STM32F4xx_DSP_StdPeriph_Lib/CMSIS/Device/ST/STM32F4xx/Include \
				-I$(TOP_DIR)/targets/F429_fire/driver/STM32F4xx_DSP_StdPeriph_Lib/STM32F4xx_StdPeriph_Driver/inc
C_INCLUDES += $(DEIVCE_LIB_INC)

HW_DRV_INC = -I$(TOP_DIR)/targets/F429_fire/inc 
C_INCLUDES += $(HW_DRV_INC)

CORE_LIB_INC = -I$(TOP_DIR)/core/inc
C_INCLUDES += $(CORE_LIB_INC)

AGENTTINY_INCLUDES = -I$(TOP_DIR)/agent_tiny/examples \
						-I$(TOP_DIR)/agent_tiny/lwm2m_client \
						-I$(TOP_DIR)/agent_tiny/osadapter/ \
						-I$(TOP_DIR)/agent_tiny/comm/include
C_INCLUDES += $(AGENTTINY_INCLUDES)

LWM2M_INCLUDES = -I$(TOP_DIR)/lwm2m/core/ \
				-I$(TOP_DIR)/lwm2m/core/er-coap-13
C_INCLUDES += $(LWM2M_INCLUDES)

NETWORK_INC = -I$(TOP_DIR)/network/inc
C_INCLUDES += $(NETWORK_INC)

ifeq ($(USE_DTLS),yes)
MBEDTLS_INC = -I$(TOP_DIR)/security/mbedtls/mbedtls-2.6.0/include
C_INCLUDES += $(MBEDTLS_INC)

MBEDTLS_PORT_INC = -I$(TOP_DIR)/security/mbedtls/mbedtls_port
C_INCLUDES += $(MBEDTLS_PORT_INC)
endif

#build
# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

# libraries
LIBS = -lc -lm -lnosys
LIBDIR =
LDFLAGS = $(MCU) -specs=nosys.specs -T$(LD_SCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET).dis 


OBJECTS = $(addprefix $(BUILD_DIR)/, $(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

OBJECTS += $(addprefix $(BUILD_DIR)/, $(notdir $(ASM_SOURCE_s:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCE_s)))

$(BUILD_DIR)/%.o:%.c $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@
	
$(BUILD_DIR)/%.o:%.s $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@
	
$(BUILD_DIR)/$(TARGET).elf :$(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SIZE) $@
	
$(BUILD_DIR)/%.hex:$(BUILD_DIR)/%.elf $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin:$(BUILD_DIR)/%.elf $(BUILD_DIR)
	$(BIN) $< $@
	
$(BUILD_DIR)/%.dis:$(BUILD_DIR)/%.elf $(BUILD_DIR)
	$(OBJDUMP) -D $< > $@	
	
$(BUILD_DIR):
	mkdir $@
	
clean:
	rm -rf .dep $(BUILD_DIR)
	
	
	
