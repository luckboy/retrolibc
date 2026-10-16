TOP_DIR = ..

BUILD_DIR = $(TOP_DIR)/build/$(TARGET)/libc

TEST_BUILD_DIR = $(TOP_DIR)/build/$(HOST)/libc/test

CFLAGS = -Wall -nostdinc -I$(TOP_DIR)/include -I.

STRING_OBJS = \
	$(BUILD_DIR)/memccpy.o \
	$(BUILD_DIR)/memchr.o \
	$(BUILD_DIR)/memcmp.o \
	$(BUILD_DIR)/memcpy.o \
	$(BUILD_DIR)/memmove.o \
	$(BUILD_DIR)/memset.o \
	$(BUILD_DIR)/strcat.o \
	$(BUILD_DIR)/strchr.o \
	$(BUILD_DIR)/strcmp.o \
	$(BUILD_DIR)/strcoll.o \
	$(BUILD_DIR)/strcpy.o \
	$(BUILD_DIR)/strcspn.o \
	$(BUILD_DIR)/strlen.o \
	$(BUILD_DIR)/strncat.o \
	$(BUILD_DIR)/strncmp.o \
	$(BUILD_DIR)/strncpy.o \
	$(BUILD_DIR)/strpbrk.o \
	$(BUILD_DIR)/strrchr.o \
	$(BUILD_DIR)/strspn.o \
	$(BUILD_DIR)/strstr.o \
	$(BUILD_DIR)/strtok.o \
	$(BUILD_DIR)/strtok_r.o \
	$(BUILD_DIR)/strxfrm.o 

OBJS = \
	$(STRING_OBJS)

LIB = $(BUILD_DIR)/libc.a

$(BUILD_DIR)/%.o: string.c $(BUILD_DIR)/.empty
	$(CC) $(TARGET_CFLAGS) $(CFLAGS) $(EXTRA_CFLAGS) -DL_$* -c -o $@ $<

$(BUILD_DIR)/%.o: %.c
	$(CC) $(TARGET_CFLAGS) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $<

$(LIB): $(OBJS)
	$(AR) -rc $(LIB) $(OBJS)
	$(AR) -ts $(LIB)
	
all: $(LIB)

$(BUILD_DIR)/.empty:
	mkdir -p $(BUILD_DIR)
	touch $(BUILD_DIR)/.empty

$(TEST_BUILD_DIR)/.empty:
	mkdir -p $(TEST_BUILD_DIR)
	touch $(TEST_BUILD_DIR)/.empty

clean:
	rm -f $(LIB) $(OBJS)
