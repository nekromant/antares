/* Part of Antares OpenSource project */


#include "usb_lib.h"
#include "usb_istr.h"
#include "usb_conf.h"

DEVICE Device_Table =
  {
    CONFIG_USB_EP_NUM,
    1
  };

DEVICE_PROP Device_Property =
  {
    Device_Init_Callback,
    Device_Reset_Callback,
    Device_Status_In_Callback,
    Device_Status_Out_Callback,
    Device_Data_Setup_Callback,
    Device_NoData_Setup_Callback,
    Device_Get_Interface_Setting_Callback,
    Device_GetDeviceDescriptor_Callback,
    Device_GetConfigDescriptor_Callback,
    Device_GetStringDescriptor_Callback,
    0,
    CONFIG_STM32_USB_MAX_PACKET_SIZE /*MAX PACKET SIZE*/
  };
  
