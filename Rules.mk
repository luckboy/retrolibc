# Rules for target objects.

$(BUILD_DIR)/%.o: arch/$(TARGET_ARCH)/%.S $(BUILD_DIR)/.empty
	$(CC) $(SFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: sys/$(TARGET_SYS)/$(TARGET_ARCH)/%.S $(BUILD_DIR)/.empty
	$(CC) $(SFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<
	
$(BUILD_DIR)/%.o: arch/$(TARGET_ARCH)/%.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: sys/$(TARGET_SYS)/$(TARGET_ARCH)/%.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: sys/$(TARGET_SYS)/%.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

ifdef TARGET_COMMON_ARCH
$(BUILD_DIR)/%.o: arch/$(TARGET_COMMON_ARCH)/%.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: sys/$(TARGET_SYS)/$(TARGET_COMMON_ARCH)/%.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<
endif

$(BUILD_DIR)/%.o: %.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

# Rules for target dependencies.

$(BUILD_DIR)/%.o.d: arch/$(TARGET_ARCH)/%.S $(BUILD_DIR)/.empty
	$(CC) $(SFLAGS) $(EXTRA_CFLAGS) -MM -MT $(BUILD_DIR)/$*.o $< > $@

$(BUILD_DIR)/%.o.d: sys/$(TARGET_SYS)/$(TARGET_ARCH)/%.S $(BUILD_DIR)/.empty
	$(CC) $(SFLAGS) $(EXTRA_CFLAGS) -MM -MT $(BUILD_DIR)/$*.o $< > $@
	
$(BUILD_DIR)/%.o.d: arch/$(TARGET_ARCH)/%.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -MM -MT $(BUILD_DIR)/$*.o $< > $@

$(BUILD_DIR)/%.o.d: sys/$(TARGET_SYS)/$(TARGET_ARCH)/%.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -MM -MT $(BUILD_DIR)/$*.o $< > $@

$(BUILD_DIR)/%.o.d: sys/$(TARGET_SYS)/%.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -MM -MT $(BUILD_DIR)/$*.o $< > $@

ifdef TARGET_COMMON_ARCH
$(BUILD_DIR)/%.o.d: arch/$(TARGET_COMMON_ARCH)/%.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -MM -MT $(BUILD_DIR)/$*.o $< > $@

$(BUILD_DIR)/%.o.d: sys/$(TARGET_SYS)/$(TARGET_COMMON_ARCH)/%.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -MM -MT $(BUILD_DIR)/$*.o $< > $@
endif

$(BUILD_DIR)/%.o.d: %.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -MM -MT $(BUILD_DIR)/$*.o $< > $@
	
# Rules for host objects.

$(TEST_BUILD_DIR)/%.o: arch/$(HOST_ARCH)/%.S $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOSTSFLAGS) -c -o $@ $<

$(TEST_BUILD_DIR)/%.o: sys/$(HOST_SYS)/$(HOST_ARCH)/%.S $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOSTSFLAGS) -c -o $@ $<
	
$(TEST_BUILD_DIR)/%.o: arch/$(HOST_ARCH)/%.c $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOSTCFLAGS) -c -o $@ $<

$(TEST_BUILD_DIR)/%.o: sys/$(HOST_SYS)/$(HOST_ARCH)/%.c $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOSTCFLAGS) -c -o $@ $<

$(TEST_BUILD_DIR)/%.o: sys/$(HOST_SYS)/%.c $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOSTCFLAGS) -c -o $@ $<

ifdef HOST_COMMON_ARCH
$(TEST_BUILD_DIR)/%.o: arch/$(HOST_COMMON_ARCH)/%.c $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOSTCFLAGS) -c -o $@ $<

$(TEST_BUILD_DIR)/%.o: sys/$(HOST_SYS)/$(HOST_COMMON_ARCH)/%.c $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOSTCFLAGS) -c -o $@ $<
endif
	
$(TEST_BUILD_DIR)/%.o: %.c $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOSTCFLAGS) -c -o $@ $<

# Rules for host dependencies.

$(TEST_BUILD_DIR)/%.o.d: arch/$(HOST_ARCH)/%.S $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOSTSFLAGS) -MM -MT $(TEST_BUILD_DIR)/$*.o $< > $@

$(TEST_BUILD_DIR)/%.o.d: sys/$(HOST_SYS)/$(HOST_ARCH)/%.S $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOSTSFLAGS) -MM -MT $(TEST_BUILD_DIR)/$*.o $< > $@
	
$(TEST_BUILD_DIR)/%.o.d: arch/$(HOST_ARCH)/%.c $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOSTCFLAGS) -MM -MT $(TEST_BUILD_DIR)/$*.o $< > $@

$(TEST_BUILD_DIR)/%.o.d: sys/$(HOST_SYS)/$(HOST_ARCH)/%.c $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOSTCFLAGS) -MM -MT $(TEST_BUILD_DIR)/$*.o $< > $@

$(TEST_BUILD_DIR)/%.o.d: sys/$(HOST_SYS)/%.c $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOSTCFLAGS) -MM -MT $(TEST_BUILD_DIR)/$*.o $< > $@

ifdef HOST_COMMON_ARCH
$(TEST_BUILD_DIR)/%.o.d: arch/$(HOST_COMMON_ARCH)/%.c $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOSTCFLAGS) -MM -MT $(TEST_BUILD_DIR)/$*.o $< > $@

$(TEST_BUILD_DIR)/%.o.d: sys/$(HOST_SYS)/$(HOST_COMMON_ARCH)/%.c $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOSTCFLAGS) -MM -MT $(TEST_BUILD_DIR)/$*.o $< > $@
endif
	
$(TEST_BUILD_DIR)/%.o.d: %.c $(TEST_BUILD_DIR)/.empty
	$(HOSTCC) $(HOSTCFLAGS) -MM -MT $(TEST_BUILD_DIR)/$*.o $< > $@
