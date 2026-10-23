TOP_DIR = ..
DIR = libc

include $(TOP_DIR)/Vars.mk

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

FILE_OBJS = \
	$(BUILD_DIR)/file_stdin.o \
	$(BUILD_DIR)/file_stdout.o \
	$(BUILD_DIR)/file_stderr.o \
	$(BUILD_DIR)/file_file_list.o \
	$(BUILD_DIR)/file_fflush_unlocked.o \
	$(BUILD_DIR)/file_mode_to_oflags.o \
	$(BUILD_DIR)/file_init_file.o \
	$(BUILD_DIR)/file_clearerr.o \
	$(BUILD_DIR)/file_fclose.o \
	$(BUILD_DIR)/file_fdopen.o \
	$(BUILD_DIR)/file_feof.o \
	$(BUILD_DIR)/file_ferror.o \
	$(BUILD_DIR)/file_fflush.o \
	$(BUILD_DIR)/file_fileno.o \
	$(BUILD_DIR)/file_flockfile.o \
	$(BUILD_DIR)/file_fopen.o \
	$(BUILD_DIR)/file_fread.o \
	$(BUILD_DIR)/file_freopen.o \
	$(BUILD_DIR)/file_fseeko.o \
	$(BUILD_DIR)/file_ftello.o \
	$(BUILD_DIR)/file_ftrylockfile.o \
	$(BUILD_DIR)/file_funlockfile.o \
	$(BUILD_DIR)/file_fwrite.o \
	$(BUILD_DIR)/file_getc_unlocked.o \
	$(BUILD_DIR)/file_putc_unlocked.o \
	$(BUILD_DIR)/file_setvbuf.o \
	$(BUILD_DIR)/file_ungetc.o

STDIO_OBJS = \
	$(BUILD_DIR)/stdio_fgetc.o \
	$(BUILD_DIR)/stdio_fputc.o \
	$(BUILD_DIR)/stdio_fseek.o \
	$(BUILD_DIR)/stdio_ftell.o

STDLIB_OBJS = \
	$(BUILD_DIR)/stdlib_atoi.o \
	$(BUILD_DIR)/stdlib_atol.o \
	$(BUILD_DIR)/stdlib_atoll.o
	
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
	$(BUILD_DIR)/string_strdup.o \
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
	$(CTYPE_OBJS) \
	$(FILE_OBJS) \
	$(STDIO_OBJS) \
	$(STDLIB_OBJS) \
	$(STRING_OBJS) \
	$(BUILD_DIR)/bsearch.o \
	$(BUILD_DIR)/malloc.o \
	$(BUILD_DIR)/qsort.o \
	$(BUILD_DIR)/strerror.o \
	$(BUILD_DIR)/strtol.o \
	$(BUILD_DIR)/sys_errlist.o

TEST_CTYPE_OBJS = \
	$(TEST_BUILD_DIR)/ctype_isalnum.o \
	$(TEST_BUILD_DIR)/ctype_isalpha.o \
	$(TEST_BUILD_DIR)/ctype_isascii.o \
	$(TEST_BUILD_DIR)/ctype_iscntrl.o \
	$(TEST_BUILD_DIR)/ctype_isdigit.o \
	$(TEST_BUILD_DIR)/ctype_isgraph.o \
	$(TEST_BUILD_DIR)/ctype_islower.o \
	$(TEST_BUILD_DIR)/ctype_isprint.o \
	$(TEST_BUILD_DIR)/ctype_ispunct.o \
	$(TEST_BUILD_DIR)/ctype_isspace.o \
	$(TEST_BUILD_DIR)/ctype_isupper.o \
	$(TEST_BUILD_DIR)/ctype_isxdigit.o \
	$(TEST_BUILD_DIR)/ctype_toascii.o \
	$(TEST_BUILD_DIR)/ctype_tolower.o \
	$(TEST_BUILD_DIR)/ctype_toupper.o

TEST_STDLIB_OBJS = \
	$(TEST_BUILD_DIR)/stdlib_atoi.o \
	$(TEST_BUILD_DIR)/stdlib_atol.o \
	$(TEST_BUILD_DIR)/stdlib_atoll.o
	
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
	$(TEST_BUILD_DIR)/string_strdup.o \
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
	$(TEST_CTYPE_OBJS) \
	$(TEST_BUILD_DIR)/test_ctype_suite.o \
	$(TEST_STDLIB_OBJS) \
	$(TEST_BUILD_DIR)/test_stdlib_suite.o \
	$(TEST_STRING_OBJS) \
	$(TEST_BUILD_DIR)/test_string_suite.o \
	$(TEST_BUILD_DIR)/bsearch.o \
	$(TEST_BUILD_DIR)/test_bsearch_suite.o \
	$(TEST_BUILD_DIR)/malloc.o \
	$(TEST_BUILD_DIR)/test_malloc_suite.o \
	$(TEST_BUILD_DIR)/qsort.o \
	$(TEST_BUILD_DIR)/test_qsort_suite.o \
	$(TEST_BUILD_DIR)/strtol.o \
	$(TEST_BUILD_DIR)/test_strtol_suite.o \
	$(TEST_BUILD_DIR)/test.o

LIB = $(BUILD_DIR)/libc.a

TEST = $(TEST_BUILD_DIR)/test

$(BUILD_DIR)/ctype_%.o: ctype.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -DL_$* -c -o $@ $<

$(BUILD_DIR)/file_%.o: file.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -DL_$* -c -o $@ $<

$(BUILD_DIR)/stdio_%.o: stdio.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -DL_$* -c -o $@ $<
	
$(BUILD_DIR)/stdlib_%.o: stdlib.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -DL_$* -c -o $@ $<

$(BUILD_DIR)/string_%.o: string.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -DL_$* -c -o $@ $<

$(TEST_BUILD_DIR)/ctype_%.o: ctype.c retrolibc.h $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOSTCFLAGS) -DL_$* -c -o $@ $<

$(TEST_BUILD_DIR)/stdlib_%.o: stdlib.c retrolibc.h $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOSTCFLAGS) -DL_$* -c -o $@ $<

$(TEST_BUILD_DIR)/string_%.o: string.c retrolibc.h $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOSTCFLAGS) -DL_$* -c -o $@ $<

include $(TOP_DIR)/Rules.mk
	
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
	$(HOSTCC) $(HOSTLDFLAGS) -o $(TEST) $(TEST_OBJS) $(HOSTLIBS)

test: $(TEST)
	echo -n

run_test: test
	./$(TEST)

clean:
	rm -f $(LIB) $(OBJS)

clean_test:
	rm -f $(TEST) $(TEST_OBJS)

$(BUILD_DIR)/malloc.o: malloc.c priv_malloc.h
$(BUILD_DIR)/file_stdin.o: file.c priv_file.h
$(BUILD_DIR)/file_stdout.o: file.c priv_file.h
$(BUILD_DIR)/file_stderr.o: file.c priv_file.h
$(BUILD_DIR)/file_file_list.o: file.c priv_file.h
$(BUILD_DIR)/file_fflush_unlocked.o: file.c priv_file.h
$(BUILD_DIR)/file_mode_to_oflags.o: file.c priv_file.h
$(BUILD_DIR)/file_init_file.o: file.c priv_file.h
$(BUILD_DIR)/file_clearerr.o: file.c priv_file.h
$(BUILD_DIR)/file_fclose.o: file.c priv_file.h
$(BUILD_DIR)/file_fdopen.o: file.c priv_file.h
$(BUILD_DIR)/file_feof.o: file.c priv_file.h
$(BUILD_DIR)/file_ferror.o: file.c priv_file.h
$(BUILD_DIR)/file_fflush.o: file.c priv_file.h
$(BUILD_DIR)/file_fileno.o: file.c priv_file.h
$(BUILD_DIR)/file_flockfile.o: file.c priv_file.h
$(BUILD_DIR)/file_fopen.o: file.c priv_file.h
$(BUILD_DIR)/file_fread.o: file.c priv_file.h
$(BUILD_DIR)/file_fseeko.o: file.c priv_file.h
$(BUILD_DIR)/file_ftello.o: file.c priv_file.h
$(BUILD_DIR)/file_ftrylockfile.o: file.c priv_file.h
$(BUILD_DIR)/file_funlockfile.o: file.c priv_file.h
$(BUILD_DIR)/file_fwrite.o: file.c priv_file.h
$(BUILD_DIR)/file_getc_unlocked.o: file.c priv_file.h
$(BUILD_DIR)/file_putc_unlocked.o: file.c priv_file.h
$(BUILD_DIR)/file_setvbuf.o: file.c priv_file.h
$(BUILD_DIR)/file_ungetc.o: file.c priv_file.h
$(TEST_BUILD_DIR)/malloc.o: malloc.c priv_malloc.h
$(TEST_BUILD_DIR)/test_malloc_suite.o: test_malloc_suite.c priv_malloc.h
