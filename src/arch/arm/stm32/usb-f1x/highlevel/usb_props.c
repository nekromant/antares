/* Part of Antares OpenSource project */

#include <string.h>
#include "usb_lib.h"
#include "usb_istr.h"
#include "usb_conf.h"
#include "usb_desc.h"

DEVICE Device_Table =
  {
    CONFIG_USB_EP_NUM,
    1
  };

const uint8_t UsbDeviceDescriptor[] =
  {
    0x12,   /* bLength */
    USB_DEVICE_DESCRIPTOR_TYPE,     /* bDescriptorType */
    CONFIG_STM32_USB_BCDUSB_LO,
    CONFIG_STM32_USB_BCDUSB_HI,   /* bcdUSB = 2.00 */
    CONFIG_STM32_USB_DEVCLASS,   /* bDeviceClass: CDC */
    CONFIG_STM32_USB_DEVSUBCLASS,   /* bDeviceSubClass */
    CONFIG_STM32_USB_DEVPROTO,   /* bDeviceProtocol */
    CONFIG_STM32_USB_MAX_PACKET_SIZE,   /* bMaxPacketSize0 */
    CONFIG_STM32_USB_VID_LO,
    CONFIG_STM32_USB_VID_HI,   /* idVendor = 0x0483 */
    CONFIG_STM32_USB_PID_LO,
    CONFIG_STM32_USB_PID_HI,   /* idProduct = 0x7540 */
    CONFIG_STM32_USB_BCDDEV_LO,
    CONFIG_STM32_USB_BCDDEV_HI,   /* bcdDevice = 2.00 */
    CONFIG_STM32_USB_MIDX,              /* Index of string descriptor describing manufacturer */
    CONFIG_STM32_USB_PIDX,              /* Index of string descriptor describing product */
    CONFIG_STM32_USB_SIDX,              /* Index of string descriptor describing the device's serial number */
    CONFIG_STM32_USB_CONFNUM    /* bNumConfigurations */
  };


DEVICE_PROP Device_Property =
  {
    usb_device_init,
    usb_device_reset,
    usb_device_status_in,
    usb_device_status_out,
    usb_device_data_setup,
    usb_device_nodata_setup,
    usb_device_get_interface_setting,
    usb_device_getdevicedescriptor,
    usb_device_getconfigdescriptor,
    usb_device_getstringdescriptor,
    0,
    CONFIG_STM32_USB_MAX_PACKET_SIZE /*MAX PACKET SIZE*/
  };
  

uint8_t *usb_device_getdevicedescriptor(uint16_t length)
{
	return Standard_GetDescriptorData(length, (PONE_DESCRIPTOR) &UsbDeviceDescriptor);
}

RESULT usb_device_get_interface_setting(uint8_t interface, uint8_t alternatesetting)
{
  if (alternatesetting >= CONFIG_STM32_USB_ALTNUM)
  {
    return USB_UNSUPPORT;
  }
  else if (interface >= CONFIG_STM32_USB_IFACENUM)
  {
    return USB_UNSUPPORT;
  }
  return USB_SUCCESS;
}

/* USB String Descriptors */
static const uint8_t lang_id_dsc[4] =
{
	4,
	USB_STRING_DESCRIPTOR_TYPE,
	0x09,
	0x04 /* LangID = 0x0409: U.S. English */
};

static const uint8_t* usb_str_dsc[] =
  {
	  lang_id_dsc,
/* Don't think that I'm insane. 
 * The following is generated via a piece of bash 
 */
#include "usb_strings.h"
  };

/* TODO: This is not the best way, since it wastes a lot of RAM*/
static uint8_t tmp[256];

static ONE_DESCRIPTOR tmp_d = {
	.Descriptor = tmp
};

static void usb_compile_descriptor(ONE_DESCRIPTOR* target, const uint8_t* str)
{
	int i;
	target->Descriptor[0] = (uint8_t) strlen((char*)str) + 2;
	target->Descriptor_Size = (uint16_t) strlen((char*)str) + 2;
	target->Descriptor[1] = USB_STRING_DESCRIPTOR_TYPE;
	for (i=2;i<strlen((char*)str)*2+2;i=i+2) {
		target->Descriptor[i]=str[i];
		target->Descriptor[i+1]=0x0;
	}
}

uint8_t* usb_device_getstringdescriptor(uint16_t len)
{
	uint8_t wValue0 = pInformation->USBwValue0;
	if (wValue0 > CONFIG_STM32_USB_STR_DSC_COUNT) {
#if CONFIG_STM32_USB_STR_DSC_DYNAMIC
		uint8_t* str = usb_device_get_dynamic_string(wValue0);
		if (str) {
			usb_compile_descriptor(&tmp_d,str);
			return Standard_GetDescriptorData(len, &tmp_d);
		}
#endif
		return NULL;
	} else {
		if (wValue0 == 0)
			return (uint8_t*)usb_str_dsc;
		else {
			/* Here, we have to compile ascii to unicode */
			usb_compile_descriptor(&tmp_d,usb_str_dsc[wValue0]);
			return Standard_GetDescriptorData(len, &tmp_d);			
		}	
		//;
	}
}

USER_STANDARD_REQUESTS User_Standard_Requests =
  {
    usb_device_getconfiguration_callback,
    usb_device_setconfiguration_callback,
    usb_device_getinterface_callback,
    usb_device_setinterface_callback,
    usb_device_getstatus_callback,
    usb_device_clearfeature_callback,
    usb_device_setendpointfeature_callback,
    usb_device_setdevicefeature_callback,
    usb_device_setdeviceaddress_callback
  };
