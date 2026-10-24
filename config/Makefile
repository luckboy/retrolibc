TOP_DIR = ..
DIR = config

include $(TOP_DIR)/Vars.mk

all: $(CONFIG_BUILD_DIR)/include/bits/config.h
	echo -n

$(CONFIG_BUILD_DIR)/include/bits/config.h:
	mkdir -p $(CONFIG_BUILD_DIR)/include/bits
	chmod 755 ./$(TARGET_SYS)/config.sh
	./$(TARGET_SYS)/config.sh target $(TARGET_KERNEL_VERSION) >> $(CONFIG_BUILD_DIR)/include/bits/config.h

$(CONFIG_BUILD_DIR)/.empty:
	mkdir -p $(BUILD_DIR)
	touch $(BUILD_DIR)/.empty

gen_config: $(CONFIG_BUILD_DIR)/include/bits/config.h $(CONFIG_BUILD_DIR)/retrolibc_config.h
	echo -n

test: $(CONFIG_BUILD_DIR)/retrolibc_config.h
	echo -n

$(CONFIG_BUILD_DIR)/retrolibc_config.h:
	mkdir -p $(CONFIG_BUILD_DIR)/include/bits
	chmod 755 ./$(TARGET_SYS)/config.sh
	./$(HOST_SYS)/config.sh host $(HOST_KERNEL_VERSION) >> $(CONFIG_BUILD_DIR)/retrolibc_config.h

run_test: test
	echo -n

clean:
	echo -n

clean_test:
	echo -n

clean_config:
	rm -f $(CONFIG_BUILD_DIR)/include/bits/config.h
	rm -f $(CONFIG_BUILD_DIR)/retrolibc_config.h
