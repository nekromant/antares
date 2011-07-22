
libmaple_cflags=
libmaple_asflags=
libmaple_ldflags=
#Get's the actual source

include make/Makefile.lib
$(info $(COMPILER_TOOLS))
$(info $(CC))
#We use libmaple's Makefile as our target
$(alien_dir).downloaded:
	$(Q) git clone https://github.com/leaflabs/libmaple.git $(alien_dir)source
	$(Q) mkdir -p $(TOP_DIR)/include/alien
	$(Q) ln -sf $(alien_dir)source $(TOP_DIR)include/alien/libmaple
	$(Q) $(if $(ALIEN_LIBMAPLE_GITREV), cd $(alien_dir)/source && git checkout $(ALIEN_LIBMAPLE_GITREV))
	$(Q) touch $@
  
download: $(alien_dir).downloaded
	@echo "Download complete"

update:	$(alien_dir).downloaded
	$(Q) cd $(alien_dir)/source && git pull

clean:	$(alien_dir).downloaded
	$(Q) cd $(alien_dir)source && make clean

mrproper: $(alien_dir).downloaded
	$(Q) rm -Rfv $(alien_dir)source
	$(Q) rm -Rfv $(alien_dir).downloaded
  
build:	$(alien_dir).downloaded
	$(info $(COMPILER_TOOLS))
	$(Q) cd $(alien_dir)source && $(MAKE) $(COMPILER_TOOLS) $(PARAMS) board=antares library 

.PHONY: $(alien_dir)source
