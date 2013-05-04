deploy_help+="\nstlink - deploy via st-util (st-flash looks broken)"

root=$(call check_root,$(CONFIG_DEPLOY_STLINK_ROOT))

stlink: 
	-$(root) st-util > $(TMPDIR)/stutil.log&
	echo "" > $(TMPDIR)/gdbscript
	echo "target extended-remote localhost:4242" >> $(TMPDIR)/gdbscript
	echo "load $(IMAGENAME).elf" >> $(TMPDIR)/gdbscript
	echo "file $(IMAGENAME).elf" >> $(TMPDIR)/gdbscript
ifneq ($(CONFIG_DEPLOY_STLINK_DEBUG),y)
	$(TOOL_PREFIX)gdb < $(TMPDIR)/gdbscript
else
	$(TOOL_PREFIX)gdb -x $(TMPDIR)/gdbscript
endif
	killall st-util
