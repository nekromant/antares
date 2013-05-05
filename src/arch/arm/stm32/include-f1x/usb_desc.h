/**
  ******************************************************************************
  * @file    usb_desc.h
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    29-June-2012
  * @brief   Descriptor Header for Virtual COM Port Device
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DESC_H
#define __USB_DESC_H

#include <generated/usb_conf.h>
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05


void usb_device_init(void);
void usb_device_reset();
void usb_device_status_in();
void usb_device_status_out();
RESULT usb_device_data_setup(uint8_t req_no);
RESULT usb_device_nodata_setup(uint8_t req_no);
RESULT usb_device_get_interface_setting(uint8_t interface, uint8_t altsetting);
uint8_t* usb_device_getdevicedescriptor(uint16_t len);
uint8_t* usb_device_getconfigdescriptor(uint16_t len);
uint8_t* usb_device_getstringdescriptor(uint16_t len);
uint8_t* usb_device_get_dynamic_string(uint8_t idx);


void    usb_device_getconfiguration_callback();
void    usb_device_setconfiguration_callback();
void    usb_device_getinterface_callback();
void    usb_device_setinterface_callback();
void    usb_device_getstatus_callback();
void    usb_device_clearfeature_callback();
void    usb_device_setendpointfeature_callback();
void    usb_device_setdevicefeature_callback();
void    usb_device_setdeviceaddress_callback();

#endif /* __USB_DESC_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
