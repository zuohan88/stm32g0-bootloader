# stm32g0-bootloader-IAP
The bootloader code is developed in stm32cubeIDE\n\r
You may upload the application file using teraterm or the python code (will be uploaded soon)

# Issue 
The bootloader and application only run well if code generated in Debugging mode. 
Unable to jump into application if code generated in Release mode. Unknown issue.
The cubeMX will generate wrong constant of CRC.

# Rom Address
Rom Address for Application Code start from 8000
const volatile uint8_t *userConfig=(const volatile uint8_t *)0x08008000;
you can adjust this value to increase the memory size of Bootloader.
You also need to modify STM32G070KBTX_FLASH.ld file in Application code as below

/* Memories definition */
MEMORY
{
  RAM    (xrw)    : ORIGIN = 0x20000000,   LENGTH = 36K
  FLASH    (rx)    : ORIGIN = 0x8008000,   LENGTH = 128K-8000
}




# Resources
STM32 in-application programming (IAP) using the USART
https://www.st.com/resource/en/application_note/an4657-stm32-inapplication-programming-iap-using-the-usart-stmicroelectronics.pdf
