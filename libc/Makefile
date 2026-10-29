TOP_DIR = ..
DIR = libc

include $(TOP_DIR)/Vars.mk

# Variables.

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
	$(BUILD_DIR)/file_fdclose_unlocked.o \
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
	$(BUILD_DIR)/file_popen.o \
	$(BUILD_DIR)/file_pclose.o \
	$(BUILD_DIR)/file_putc_unlocked.o \
	$(BUILD_DIR)/file_setvbuf.o \
	$(BUILD_DIR)/file_ungetc.o

STDIO_OBJS = \
	$(BUILD_DIR)/stdio_fgetc.o \
	$(BUILD_DIR)/stdio_fgets.o \
	$(BUILD_DIR)/stdio_fgetpos.o \
	$(BUILD_DIR)/stdio_fputc.o \
	$(BUILD_DIR)/stdio_fputs.o \
	$(BUILD_DIR)/stdio_fseek.o \
	$(BUILD_DIR)/stdio_fsetpos.o \
	$(BUILD_DIR)/stdio_ftell.o \
	$(BUILD_DIR)/stdio_getc.o \
	$(BUILD_DIR)/stdio_getchar.o \
	$(BUILD_DIR)/stdio_getchar_unlocked.o \
	$(BUILD_DIR)/stdio_gets.o \
	$(BUILD_DIR)/stdio_getw.o \
	$(BUILD_DIR)/stdio_perror.o \
	$(BUILD_DIR)/stdio_putc.o \
	$(BUILD_DIR)/stdio_putchar.o \
	$(BUILD_DIR)/stdio_putchar_unlocked.o \
	$(BUILD_DIR)/stdio_puts.o \
	$(BUILD_DIR)/stdio_putw.o \
	$(BUILD_DIR)/stdio_rewind.o \
	$(BUILD_DIR)/stdio_setbuf.o

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
	$(BUILD_DIR)/strtod.o \
	$(BUILD_DIR)/strtol.o \
	$(BUILD_DIR)/sys_errlist.o

DEPS = $(addsuffix .d, $(OBJS))
	
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

TEST_FILE_OBJS = \
	$(TEST_BUILD_DIR)/file_stdin.o \
	$(TEST_BUILD_DIR)/file_stdout.o \
	$(TEST_BUILD_DIR)/file_stderr.o \
	$(TEST_BUILD_DIR)/file_file_list.o \
	$(TEST_BUILD_DIR)/file_fflush_unlocked.o \
	$(TEST_BUILD_DIR)/file_fdclose_unlocked.o \
	$(TEST_BUILD_DIR)/file_mode_to_oflags.o \
	$(TEST_BUILD_DIR)/file_init_file.o \
	$(TEST_BUILD_DIR)/file_clearerr.o \
	$(TEST_BUILD_DIR)/file_fclose.o \
	$(TEST_BUILD_DIR)/file_fdopen.o \
	$(TEST_BUILD_DIR)/file_feof.o \
	$(TEST_BUILD_DIR)/file_ferror.o \
	$(TEST_BUILD_DIR)/file_fflush.o \
	$(TEST_BUILD_DIR)/file_fileno.o \
	$(TEST_BUILD_DIR)/file_flockfile.o \
	$(TEST_BUILD_DIR)/file_fopen.o \
	$(TEST_BUILD_DIR)/file_fread.o \
	$(TEST_BUILD_DIR)/file_freopen.o \
	$(TEST_BUILD_DIR)/file_fseeko.o \
	$(TEST_BUILD_DIR)/file_ftello.o \
	$(TEST_BUILD_DIR)/file_ftrylockfile.o \
	$(TEST_BUILD_DIR)/file_funlockfile.o \
	$(TEST_BUILD_DIR)/file_fwrite.o \
	$(TEST_BUILD_DIR)/file_getc_unlocked.o \
	$(TEST_BUILD_DIR)/file_popen.o \
	$(TEST_BUILD_DIR)/file_pclose.o \
	$(TEST_BUILD_DIR)/file_putc_unlocked.o \
	$(TEST_BUILD_DIR)/file_setvbuf.o \
	$(TEST_BUILD_DIR)/file_ungetc.o

TEST_STDIO_OBJS = \
	$(TEST_BUILD_DIR)/stdio_fgetc.o \
	$(TEST_BUILD_DIR)/stdio_fgets.o \
	$(TEST_BUILD_DIR)/stdio_fgetpos.o \
	$(TEST_BUILD_DIR)/stdio_fputc.o \
	$(TEST_BUILD_DIR)/stdio_fputs.o \
	$(TEST_BUILD_DIR)/stdio_fseek.o \
	$(TEST_BUILD_DIR)/stdio_fsetpos.o \
	$(TEST_BUILD_DIR)/stdio_ftell.o \
	$(TEST_BUILD_DIR)/stdio_getc.o \
	$(TEST_BUILD_DIR)/stdio_getchar.o \
	$(TEST_BUILD_DIR)/stdio_getchar_unlocked.o \
	$(TEST_BUILD_DIR)/stdio_gets.o \
	$(TEST_BUILD_DIR)/stdio_getw.o \
	$(TEST_BUILD_DIR)/stdio_perror.o \
	$(TEST_BUILD_DIR)/stdio_putc.o \
	$(TEST_BUILD_DIR)/stdio_putchar.o \
	$(TEST_BUILD_DIR)/stdio_putchar_unlocked.o \
	$(TEST_BUILD_DIR)/stdio_puts.o \
	$(TEST_BUILD_DIR)/stdio_putw.o \
	$(TEST_BUILD_DIR)/stdio_rewind.o \
	$(TEST_BUILD_DIR)/stdio_setbuf.o
	
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
	$(TEST_BUILD_DIR)/test_file_helpers.o \
	$(TEST_CTYPE_OBJS) \
	$(TEST_BUILD_DIR)/test_ctype_suite.o \
	$(TEST_FILE_OBJS) \
	$(TEST_BUILD_DIR)/test_file_suite.o \
	$(TEST_STDIO_OBJS) \
	$(TEST_BUILD_DIR)/test_stdio_suite.o \
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
	$(TEST_BUILD_DIR)/strtod.o \
	$(TEST_BUILD_DIR)/test_strtod_suite.o \
	$(TEST_BUILD_DIR)/strtol.o \
	$(TEST_BUILD_DIR)/test_strtol_suite.o \
	$(TEST_BUILD_DIR)/test.o

TEST_DEPS = $(addsuffix .d, $(TEST_OBJS))

LIB = $(BUILD_DIR)/libc.a

TEST = $(TEST_BUILD_DIR)/test

# Rules for target objects.

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

# Rules for target dependencies.

$(BUILD_DIR)/ctype_%.o.d: ctype.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -DL_$* -MM -MT $(BUILD_DIR)/ctype_$*.o $< > $@

$(BUILD_DIR)/file_%.o.d: file.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -DL_$* -MM -MT $(BUILD_DIR)/file_$*.o $< > $@

$(BUILD_DIR)/stdio_%.o.d: stdio.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -DL_$* -MM -MT $(BUILD_DIR)/stdio_$*.o $< > $@
	
$(BUILD_DIR)/stdlib_%.o.d: stdlib.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -DL_$* -MM -MT $(BUILD_DIR)/stdlib_$*.o $< > $@

$(BUILD_DIR)/string_%.o.d: string.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -DL_$* -MM -MT $(BUILD_DIR)/string_$*.o $< > $@

# Rules for host objects.

$(TEST_BUILD_DIR)/ctype_%.o: ctype.c $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOSTCFLAGS) -DL_$* -c -o $@ $<

$(TEST_BUILD_DIR)/file_%.o: file.c $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOSTCFLAGS) -DL_$* -c -o $@ $<

$(TEST_BUILD_DIR)/stdio_%.o: stdio.c $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOSTCFLAGS) -DL_$* -c -o $@ $<

$(TEST_BUILD_DIR)/stdlib_%.o: stdlib.c $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOSTCFLAGS) -DL_$* -c -o $@ $<

$(TEST_BUILD_DIR)/string_%.o: string.c $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOSTCFLAGS) -DL_$* -c -o $@ $<

# Rules for host dependencies.

$(TEST_BUILD_DIR)/ctype_%.o.d: ctype.c $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOSTCFLAGS) -DL_$* -MM -MT $(TEST_BUILD_DIR)/ctype_$*.o $< > $@

$(TEST_BUILD_DIR)/file_%.o.d: file.c $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOSTCFLAGS) -DL_$* -MM -MT $(TEST_BUILD_DIR)/stdlib_$*.o $< > $@

$(TEST_BUILD_DIR)/stdio_%.o.d: file.c $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOSTCFLAGS) -DL_$* -MM -MT $(TEST_BUILD_DIR)/stdlib_$*.o $< > $@

$(TEST_BUILD_DIR)/stdlib_%.o.d: stdlib.c $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOSTCFLAGS) -DL_$* -MM -MT $(TEST_BUILD_DIR)/stdlib_$*.o $< > $@

$(TEST_BUILD_DIR)/string_%.o.d: string.c $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOSTCFLAGS) -DL_$* -MM -MT $(TEST_BUILD_DIR)/string_$*.o $< > $@

include $(TOP_DIR)/Rules.mk

# Rules.

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

gen_config:
	@echo -n

test: $(TEST)
	@echo -n

run_test: test
	./$(TEST)

clean:
	rm -f $(LIB) $(OBJS)

clean_test:
	rm -f $(TEST) $(TEST_OBJS)

clean_deps:
	rm -f $(DEPS)
	rm -f $(TEST_DEPS)

clean_config:
	@echo -n

include $(DEPS) $(TEST_DEPS)
