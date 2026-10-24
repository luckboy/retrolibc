DIRS += config libc

all gen_config test run_test clean clean_test clean_config:
	for d in $(DIRS); do $(MAKE) -C $$d $@; done
