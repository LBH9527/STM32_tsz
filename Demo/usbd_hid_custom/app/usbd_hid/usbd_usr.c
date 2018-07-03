/**
  ******************************************************************************
  * @file    usbd_usr.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    09-November-2015
  * @brief   This file includes the user application layer
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
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

/* Includes ------------------------------------------------------------------*/
#include "usbd_usr.h"
#define usb_printf	printf

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
* @{
*/

/** @defgroup USBD_USR 
* @brief    This file includes the user application layer
* @{
*/ 

/** @defgroup USBD_USR_Private_TypesDefinitions
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBD_USR_Private_Defines
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBD_USR_Private_Macros
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBD_USR_Private_Variables
* @{
*/ 

USBD_Usr_cb_TypeDef USR_cb =
{
  USBD_USR_Init,
  USBD_USR_DeviceReset,
  USBD_USR_DeviceConfigured,
  USBD_USR_DeviceSuspended,
  USBD_USR_DeviceResumed,
  
  USBD_USR_DeviceConnected,
  USBD_USR_DeviceDisconnected,  
};

uint32_t ADC_ConvertedValueX = 0;
uint32_t ADC_ConvertedValueX_1 = 0;

/**
* @}
*/

/** @defgroup USBD_USR_Private_Constants
* @{
*/ 

/**
* @}
*/



/** @defgroup USBD_USR_Private_FunctionPrototypes
* @{
*/ 
static void ADC_Configuration(void);
/**
* @}
*/ 


/** @defgroup USBD_USR_Private_Functions
* @{
*/ 

#define USER_INFORMATION1      (uint8_t*)"[Key]:switch On/Off Button[ReportID 5] "
#define USER_INFORMATION2      (uint8_t*)"[Leds]:switch on/off Leds [ReportID 1]"
#define USER_INFORMATION3      (uint8_t*)"[Potentiometer]:[ReportID 7]"

/**
* @brief  USBD_USR_Init 
*         Displays the message on LCD for host lib initialization
* @param  None
* @retval None
*/
void USBD_USR_Init(void)
{  
  
}

/**
* @brief  USBD_USR_DeviceReset 
*         Displays the message on LCD on device Reset Event
* @param  speed : device speed
* @retval None
*/
void USBD_USR_DeviceReset(uint8_t speed )
{
    /* 在USB未连接状态，这个函数每隔20ms进来一次 */
 switch (speed)
 {
   case USB_OTG_SPEED_HIGH: 
     usb_printf ("     USB Device Library v1.2.0 [HS]" );
     break;

  case USB_OTG_SPEED_FULL: 
     usb_printf ("     USB Device Library v1.2.0 [FS]" );
     break;
 default:
     usb_printf ("     USB Device Library v1.2.0 [??]" );
     break;  
 }
}


/**
* @brief  USBD_USR_DeviceConfigured
*         Displays the message on LCD on device configuration Event
* @param  None
* @retval Status
*/
void USBD_USR_DeviceConfigured (void)
{
  usb_printf("> HID Interface started.\n");
}


/**
* @brief  USBD_USR_DeviceConnected
*         Displays the message on LCD on device connection Event
* @param  None
* @retval Status
*/
void USBD_USR_DeviceConnected (void)
{
  usb_printf("> USB Device Connected.\n");
}


/**
* @brief  USBD_USR_DeviceDisonnected
*         Displays the message on LCD on device disconnection Event
* @param  None
* @retval Status
*/
void USBD_USR_DeviceDisconnected (void)
{
  usb_printf("> USB Device Disconnected.\n");
}

/**
* @brief  USBD_USR_DeviceSuspended 
*         Displays the message on LCD on device suspend Event
* @param  None
* @retval None
*/
void USBD_USR_DeviceSuspended(void)
{
  usb_printf("> USB Device in Suspend Mode.\n");
  /* Users can do their application actions here for the USB-Reset */
}


/**
* @brief  USBD_USR_DeviceResumed 
*         Displays the message on LCD on device resume Event
* @param  None
* @retval None
*/
void USBD_USR_DeviceResumed(void)
{
    usb_printf("> USB Device in Idle Mode.\n");
  /* Users can do their application actions here for the USB-Reset */
}

/**
* @}
*/ 

/**
* @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
