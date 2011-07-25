


$(BUILD_DIR)%.o: $(BUILD_DIR)%.c $(subst .,_,$(subst /,_,$()))
	$(SILENT_CC) $(CC) $(INCLUDES) $(CFLAGS)  -c  -o $(@) $< 


%.elf: $(build_objects)	
	$(SILENT_LD) $(CC) $(LDFLAGS) -o $(@) $^ 

%.lss: %.elf
	$(SILENT_DISAS) $(OBJDUMP) -h -S $< > $@
	
