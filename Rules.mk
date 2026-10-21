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

ifdef TARGET_COMMON_ARCH
$(BUILD_DIR)/%.o: arch/$(TARGET_COMMON_ARCH)/%.c $(BUILD_DIR)/.empty
	$(CC) $(TARGET_FLAGS) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: sys/$(TARGET_SYS)/$(TARGET_COMMON_ARCH)/%.c $(BUILD_DIR)/.empty
	$(CC) $(TARGET_FLAGS) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<
endif

$(BUILD_DIR)/%.o: %.c $(BUILD_DIR)/.empty
	$(CC) $(TARGET_FLAGS) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

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

ifdef HOST_COMMON_ARCH
$(TEST_BUILD_DIR)/%.o: arch/$(HOST_COMMON_ARCH)/%.c retrolibc.h $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOST_FLAGS) $(HOSTCFLAGS) -c -o $@ $<

$(TEST_BUILD_DIR)/%.o: sys/$(HOST_SYS)/$(HOST_COMMON_ARCH)/%.c retrolibc.h $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOST_FLAGS) $(HOSTCFLAGS) -c -o $@ $<
endif
	
$(TEST_BUILD_DIR)/%.o: %.c retrolibc.h $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOST_FLAGS) $(HOSTCFLAGS) -c -o $@ $<
