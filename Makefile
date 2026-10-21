DIRS += libc

all test run_test clean clean_test:
	for d in $(DIRS); do $(MAKE) -C $$d $@; done
