# Mandatory files for LUFA USB stack
objects-y += ConfigDescriptors.o Events.o USBTask.o

# USB Device sources
objects-$(CONFIG_LIB_LUFA_DEVICE) += DeviceStandardReq.o
# USB Host sources
objects-$(CONFIG_LIB_LUFA_HOST) += HostStandardReq.o


# Arch-depending subdirs
subdirs-$(CONFIG_ARCH_AVR) += AVR8
