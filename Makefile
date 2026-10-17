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
	host_arch=`echo "$$host" | cut -d - -f 1`; \
	host_sys=`echo "$$host" | cut -d - -f 2`; \
	target_arch=`echo "$$target" | cut -d - -f 1`; \
	target_sys=`echo "$$target" | cut -d - -f 2`; \
	for d in $(DIRS); do \
		$(MAKE) -C $$d \
			HOSTCC=$$hostcc \
			CC=$$cc \
			HOST=$$host \
			TARGET=$$target \
			HOST_ARCH=$$host_arch \
			HOST_SYS=$$host_sys \
			TARGET_ARCH=$$host_arch \
			TARGET_SYS=$$host_sys \
			$@; \
	done
