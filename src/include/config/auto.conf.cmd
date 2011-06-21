deps_config := \
	version.kcnf \
	deploy/avrdude-remote/kcnf \
	deploy/avrdude/kcnf \
	src/arch/avr/deploy.kcnf \
	deploy/kcnf \
	src/apps/eurobot/kcnf \
	src/apps/kcnf \
	src/drivers/kcnf \
	src/arch/8051/kcnf \
	src/arch/arm/kcnf \
	src/arch/avr/common/adc/chantags.kcnf \
	src/arch/avr/common/adc/chanlist.kcnf \
	src/arch/avr/common/adc/kcnf \
	src/arch/avr/kcnf \
	src/arch/kcnf \
	./kcnf

src/include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
