


$(BUILD_DIR)%.o: $(BUILD_DIR)%.c $(subst .,_,$(subst /,_,$()))
	$(SILENT_CC) $(CC) $(INCLUDES) $(CFLAGS)  -c  -o $(@) $< 

$(BUILD_DIR)%.o: $(BUILD_DIR)%.S $(subst .,_,$(subst /,_,$()))
	$(SILENT_AS) $(AS) -x assembler-with-cpp $(INCLUDES) $(ASFLAGS)  -o $(@) $< 


%.elf: $(build_objects)	
	$(SILENT_LD) $(CC) $(LDFLAGS) -o $(@) $^ 

%.lss: %.elf
	$(SILENT_DISAS) $(OBJDUMP) -h -S $< > $@
	
