# stm32g0-bootloader-IAP
The bootloader code is developed in stm32cubeIDE.<br />
You may upload the application file using teraterm or the python code (will be uploaded soon)<br />

# Issue 
The bootloader and application only run well if code generated in Debugging mode. <br />
Unable to jump into application if code generated in Release mode. Unknown issue.<br />
The cubeMX will generate wrong constant of CRC.<br />

# Rom Address
Rom Address for Application Code start from 8000<br />
const volatile uint8_t *userConfig=(const volatile uint8_t *)0x08008000;<br />
you can adjust this value to increase the memory size of Bootloader.<br />
You also need to modify STM32G070KBTX_FLASH.ld file in Application code as below<br />

/* Memories definition */
MEMORY<br />
{<br />
  RAM    (xrw)    : ORIGIN = 0x20000000,   LENGTH = 36K<br />
  FLASH    (rx)    : ORIGIN = 0x8008000,   LENGTH = 128K-8000<br />
}<br />




# Resources
STM32 in-application programming (IAP) using the USART<br />
https://www.st.com/resource/en/application_note/an4657-stm32-inapplication-programming-iap-using-the-usart-stmicroelectronics.pdf
