TOP_DIR = ..

BUILD_DIR = $(TOP_DIR)/build/$(TARGET)/libc

TEST_BUILD_DIR = $(TOP_DIR)/build/$(HOST)/libc/test

CFLAGS = -Wall -Os -nostdinc -I$(TOP_DIR)/include -I$(TOP_DIR)/arch/$(TARGET_ARCH)/include -I$(TOP_DIR)/sys/$(TARGET_SYS)/$(TARGET_ARCH)/include -I$(TOP_DIR)/sys/$(TARGET_SYS)/include -I. -I./arch/$(TARGET_ARCH) -I./sys/$(TARGET_SYS)/$(TARGET_ARCH) -I./sys/$(TARGET_SYS)
SFLAGS = $(CFLAGS) -D__ASSEMBLER__=1

HOSTCFLAGS = -Wall -DTEST=1 -g -I. -I./arch/$(HOST_ARCH) -I./sys/$(HOST_SYS)/$(HOST_ARCH) -I./sys/$(HOST_SYS)
HOSTSFLAGS = $(HOSTCFLAGS) -D__ASSEMBLER__=1
HOSTLDFLAGS = -g
HOSTLIBS = -lm -lcunit

SYS_OBJS = \
	$(BUILD_DIR)/sys_errlist.o

CTYPE_OBJS = \
	$(BUILD_DIR)/ctype_isalnum.o \
	$(BUILD_DIR)/ctype_isalpha.o \
	$(BUILD_DIR)/ctype_isascii.o \
	$(BUILD_DIR)/ctype_iscntrl.o \
	$(BUILD_DIR)/ctype_isdigit.o \
	$(BUILD_DIR)/ctype_isgraph.o \
	$(BUILD_DIR)/ctype_islower.o \
	$(BUILD_DIR)/ctype_isprint.o \
	$(BUILD_DIR)/ctype_ispunct.o \
	$(BUILD_DIR)/ctype_isspace.o \
	$(BUILD_DIR)/ctype_isupper.o \
	$(BUILD_DIR)/ctype_isxdigit.o \
	$(BUILD_DIR)/ctype_toascii.o \
	$(BUILD_DIR)/ctype_tolower.o \
	$(BUILD_DIR)/ctype_toupper.o

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
	$(SYS_OBJS) \
	$(CTYPE_OBJS) \
	$(STRING_OBJS) \
	$(BUILD_DIR)/bsearch.o \
	$(BUILD_DIR)/qsort.o \
	$(BUILD_DIR)/strerror.o

TEST_STRING_OBJS = \
	$(TEST_BUILD_DIR)/string_memccpy.o \
	$(TEST_BUILD_DIR)/string_memchr.o \
	$(TEST_BUILD_DIR)/string_memcmp.o \
	$(TEST_BUILD_DIR)/string_memcpy.o \
	$(TEST_BUILD_DIR)/string_memmove.o \
	$(TEST_BUILD_DIR)/string_memset.o \
	$(TEST_BUILD_DIR)/string_strcat.o \
	$(TEST_BUILD_DIR)/string_strchr.o \
	$(TEST_BUILD_DIR)/string_strcmp.o \
	$(TEST_BUILD_DIR)/string_strcoll.o \
	$(TEST_BUILD_DIR)/string_strcpy.o \
	$(TEST_BUILD_DIR)/string_strcspn.o \
	$(TEST_BUILD_DIR)/string_strlen.o \
	$(TEST_BUILD_DIR)/string_strncat.o \
	$(TEST_BUILD_DIR)/string_strncmp.o \
	$(TEST_BUILD_DIR)/string_strncpy.o \
	$(TEST_BUILD_DIR)/string_strpbrk.o \
	$(TEST_BUILD_DIR)/string_strrchr.o \
	$(TEST_BUILD_DIR)/string_strspn.o \
	$(TEST_BUILD_DIR)/string_strstr.o \
	$(TEST_BUILD_DIR)/string_strtok.o \
	$(TEST_BUILD_DIR)/string_strtok_r.o \
	$(TEST_BUILD_DIR)/string_strxfrm.o

TEST_OBJS = \
	$(TEST_STRING_OBJS) \
	$(TEST_BUILD_DIR)/test_string_suite.o \
	$(TEST_BUILD_DIR)/bsearch.o \
	$(TEST_BUILD_DIR)/test_bsearch_suite.o \
	$(TEST_BUILD_DIR)/qsort.o \
	$(TEST_BUILD_DIR)/test_qsort_suite.o \
	$(TEST_BUILD_DIR)/test.o
	
LIB = $(BUILD_DIR)/libc.a

TEST = $(TEST_BUILD_DIR)/test

$(BUILD_DIR)/ctype_%.o: ctype.c $(BUILD_DIR)/.empty
	$(CC) $(TARGET_FLAGS) $(CFLAGS) $(EXTRA_CFLAGS) -DL_$* -c -o $@ $<

$(BUILD_DIR)/string_%.o: string.c $(BUILD_DIR)/.empty
	$(CC) $(TARGET_FLAGS) $(CFLAGS) $(EXTRA_CFLAGS) -DL_$* -c -o $@ $<

$(BUILD_DIR)/%.o: arch/$(TARGET_ARCH)/%.S $(BUILD_DIR)/.empty
	$(CC) $(TARGET_FLAGS) $(SFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: sys/$(TARGET_SYS)/$(TARGET_ARCH)/%.S $(BUILD_DIR)/.empty
	$(CC) $(TARGET_FLAGS) $(SFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<
	
$(BUILD_DIR)/%.o: arch/$(TARGET_ARCH)/%.c $(BUILD_DIR)/.empty
	$(CC) $(TARGET_FLAGS) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: sys/$(TARGET_SYS)/$(TARGET_ARCH)/%.c $(BUILD_DIR)/.empty
	$(CC) $(TARGET_FLAGS) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: sys/$(TARGET_SYS)/%.c $(BUILD_DIR)/.empty
	$(CC) $(TARGET_FLAGS) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: %.c $(BUILD_DIR)/.empty
	$(CC) $(TARGET_FLAGS) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

$(TEST_BUILD_DIR)/string_%.o: string.c retrolibc.h $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOST_FLAGS) $(HOSTCFLAGS) -DL_$* -c -o $@ $<

$(TEST_BUILD_DIR)/%.o: arch/$(HOST_ARCH)/%.S $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOST_FLAGS) $(HOSTSFLAGS) -c -o $@ $<

$(TEST_BUILD_DIR)/%.o: sys/$(HOST_SYS)/$(HOST_ARCH)/%.S $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOST_FLAGS) $(HOSTSFLAGS) -c -o $@ $<
	
$(TEST_BUILD_DIR)/%.o: arch/$(HOST_ARCH)/%.c retrolibc.h $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOST_FLAGS) $(HOSTCFLAGS) -c -o $@ $<

$(TEST_BUILD_DIR)/%.o: sys/$(HOST_SYS)/$(HOST_ARCH)/%.c retrolibc.h $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOST_FLAGS) $(HOSTCFLAGS) -c -o $@ $<

$(TEST_BUILD_DIR)/%.o: sys/$(HOST_SYS)/%.c retrolibc.h $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOST_FLAGS) $(HOSTCFLAGS) -c -o $@ $<

$(TEST_BUILD_DIR)/%.o: %.c retrolibc.h $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOST_FLAGS) $(HOSTCFLAGS) -c -o $@ $<

all: $(LIB)

$(BUILD_DIR)/.empty:
	mkdir -p $(BUILD_DIR)
	touch $(BUILD_DIR)/.empty

$(TEST_BUILD_DIR)/.empty:
	mkdir -p $(TEST_BUILD_DIR)
	touch $(TEST_BUILD_DIR)/.empty

$(LIB): $(OBJS)
	$(AR) -rc $(LIB) $(OBJS)
	$(AR) -ts $(LIB)

$(TEST): $(TEST_OBJS)
	$(HOSTCC) $(HOST_FLAGS) $(HOSTLDFLAGS) -o $(TEST) $(TEST_OBJS) $(HOSTLIBS)
	
run_test: $(TEST)
	./$(TEST)

clean:
	rm -f $(LIB) $(OBJS)
	rm -f $(TEST) $(TEST_OBJS)
