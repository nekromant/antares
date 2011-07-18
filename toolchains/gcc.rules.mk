
$(BUILD_DIR)%.o: $(BUILD_DIR)%.c $(subst .,_,$(subst /,_,$()))
	$(call colorize,$(t_ylw))
	@echo "Building object: $(@)"
	$(call colorize,$(col_rst))
	$(Q) $(CC) $(INCLUDES) $(CFLAGS)  -c  -o $(@) $< 


%.elf: $(build_objects)	
	$(Q) $(CC) $(LDFLAGS) -o $(@) $^ 
	$(call colorize,$(t_cyn))
	@echo "Linked ELF: $(@)"
	$(call colorize,$(col_rst))

%.lss: %.elf
	$(call colorize,$(t_cyn))
	$(Q) $(OBJDUMP) -h -S $< > $@
	@echo "Created disassembly file: $(@)"
	$(call colorize,$(col_rst))
