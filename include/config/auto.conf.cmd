deps_config := \
	version.kcnf \
	arch/8051/kcnf \
	arch/arm/kcnf \
	arch/avr/m640/kcnf \
	arch/avr/kcnf \
	arch.kcnf \
	./kcnf

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
