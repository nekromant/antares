include src/arch/arm/arm_march.mk
include src/arch/arm/arm_mcpus.mk

$(if $(CONFIG_ARCH_ARM_STM32),$(eval include src/arch/arm/stm32/make.mk))
$(if $(CONFIG_ARCH_ARM_STELLARIS),$(eval include src/arch/arm/stellaris/make.mk))


