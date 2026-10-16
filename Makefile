DIRS = libc

all test run_test clean:
	hostcc="$(HOSTCC)"; \
	if [ "$$hostcc" = "" ]; then hostcc=cc; fi; \
	cc="$(CC)"; \
	if [ "$$cc" = "" ]; then cc=cc; fi; \
	host="$(HOST)"; \
	if [ "$$host" = "" ]; then host=`$$hostcc -dumpmachine`; fi; \
	target="$(TARGET)"; \
	if [ "$$target" = "" ]; then target=`$$cc -dumpmachine`; fi; \
	for d in $(DIRS); do \
		$(MAKE) -C $$d HOSTCC=$$hostcc CC=$$cc HOST=$$host TARGET=$$target $@; \
	done
