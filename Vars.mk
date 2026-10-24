HOSTCC = gcc
HOST = $(shell $(HOSTCC) -dumpmachine)
HOST_ARCH = $(shell echo $(HOST) | cut -d - -f 1)
HOST_SYS = $(shell echo $(HOST) | cut -d - -f 2)

CC = gcc
TARGET = $(shell $(CC) -dumpmachine)
TARGET_ARCH = $(shell echo $(TARGET) | cut -d - -f 1)
TARGET_SYS = $(shell echo $(TARGET) | cut -d - -f 2)

ifeq ($(HOST_ARCH), i386)
HOST_COMMON_ARCH = x86
else
ifeq ($(HOST_ARCH), x86_64)
HOST_COMMON_ARCH = x86
endif
endif

ifeq ($(TARGET_ARCH), i386)
TARGET_COMMON_ARCH = x86
else
ifeq ($(TARGET_ARCH), x86_64)
TARGET_COMMON_ARCH = x86
endif
endif

LIB_TYPE = static

CONFIG_BUILD_DIR = $(TOP_DIR)/build/$(TARGET)/config

BUILD_DIR = $(TOP_DIR)/build/$(TARGET)/$(DIR)/$(LIB_TYPE)
TEST_BUILD_DIR = $(TOP_DIR)/build/$(HOST)/$(DIR)/test

CFLAGS += $(TARGET_FLAGS) -Wall -Os -nostdinc
CFLAGS += -I$(TOP_DIR)/include -I$(TOP_DIR)/arch/$(TARGET_ARCH)/include -I$(TOP_DIR)/sys/$(TARGET_SYS)/$(TARGET_ARCH)/include -I$(TOP_DIR)/sys/$(TARGET_SYS)/include
CFLAGS += -I. -I./arch/$(TARGET_ARCH) -I./sys/$(TARGET_SYS)/$(TARGET_ARCH) -I./sys/$(TARGET_SYS)
ifdef TARGET_COMMON_ARCH
CFLAGS += -I$(TOP_DIR)/arch/$(TARGET_COMMON_ARCH)/include -I$(TOP_DIR)/sys/$(TARGET_SYS)/$(TARGET_COMMON_ARCH)/include
CFLAGS += -I./arch/$(TARGET_COMMON_ARCH) -I./sys/$(TARGET_SYS)/$(TARGET_COMMON_ARCH)
endif
CFLAGS += -I$(CONFIG_BUILD_DIR)/include
SFLAGS += $(CFLAGS) -D__ASSEMBLER__=1

HOSTCFLAGS += $(HOST_FLAGS) -Wall -DTEST=1 -D_XOPEN_SOURCE=1 -D_GNU_SOURCE=1 -g
HOSTCFLAGS += -I. -I./arch/$(HOST_ARCH) -I./sys/$(HOST_SYS)/$(HOST_ARCH) -I./sys/$(HOST_SYS)
ifdef HOST_COMMON_ARCH
HOSTCFLAGS += -I./arch/$(HOST_COMMON_ARCH) -I./sys/$(HOST_SYS)/$(HOST_COMMON_ARCH)
endif
HOSTCFLAGS += -I$(CONFIG_BUILD_DIR)
HOSTSFLAGS += $(HOSTCFLAGS) -D__ASSEMBLER__=1
HOSTLDFLAGS += $(HOST_FLAGS) -g
HOSTLIBS += -lm -lpthread -lcunit
