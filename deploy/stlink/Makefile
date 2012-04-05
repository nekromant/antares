deploy_help+="\nstlink - deploy via st-util (st-flash looks broken)"

root=$(call check_root,$(CONFIG_DEPLOY_STLINK_ROOT))

stlink: 
	-$(root) st-util&
	echo "" > $(TMPDIR)/gdbscript
	echo "target extended-remote localhost:4242" >> $(TMPDIR)/gdbscript
	echo "load $(IMAGENAME).elf" >> $(TMPDIR)/gdbscript
	$(TOOL_PREFIX)gdb < $(TMPDIR)/gdbscript
	