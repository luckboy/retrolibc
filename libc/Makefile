TOP_DIR = ..

BUILD_DIR = $(TOP_DIR)/build/$(TARGET)/libc

TEST_BUILD_DIR = $(TOP_DIR)/build/$(HOST)/libc/test

CFLAGS = -Wall -nostdinc -I$(TOP_DIR)/include -I.

STRING_OBJS = \
	$(BUILD_DIR)/string_memccpy.o \
	$(BUILD_DIR)/string_memchr.o \
	$(BUILD_DIR)/string_memcmp.o \
	$(BUILD_DIR)/string_memcpy.o \
	$(BUILD_DIR)/string_memmove.o \
	$(BUILD_DIR)/string_memset.o \
	$(BUILD_DIR)/string_strcat.o \
	$(BUILD_DIR)/string_strchr.o \
	$(BUILD_DIR)/string_strcmp.o \
	$(BUILD_DIR)/string_strcoll.o \
	$(BUILD_DIR)/string_strcpy.o \
	$(BUILD_DIR)/string_strcspn.o \
	$(BUILD_DIR)/string_strlen.o \
	$(BUILD_DIR)/string_strncat.o \
	$(BUILD_DIR)/string_strncmp.o \
	$(BUILD_DIR)/string_strncpy.o \
	$(BUILD_DIR)/string_strpbrk.o \
	$(BUILD_DIR)/string_strrchr.o \
	$(BUILD_DIR)/string_strspn.o \
	$(BUILD_DIR)/string_strstr.o \
	$(BUILD_DIR)/string_strtok.o \
	$(BUILD_DIR)/string_strtok_r.o \
	$(BUILD_DIR)/string_strxfrm.o 

OBJS = \
	$(STRING_OBJS) \
	$(BUILD_DIR)/bsearch.o \
	$(BUILD_DIR)/qsort.o

LIB = $(BUILD_DIR)/libc.a

$(BUILD_DIR)/string_%.o: string.c $(BUILD_DIR)/.empty
	$(CC) $(TARGET_CFLAGS) $(CFLAGS) $(EXTRA_CFLAGS) -DL_$* -c -o $@ $<	

$(BUILD_DIR)/%.o: %.c $(BUILD_DIR)/.empty
	$(CC) $(TARGET_CFLAGS) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

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
