/**
  ******************************************************************************
  * @file    IAP_Main/Src/menu.c 
  * @author  MCD Application Team
  * @version 1.0.0
  * @date    8-May-2015
  * @brief   This file provides the software which contains the main menu routine.
  *          The main menu gives the options of:
  *             - downloading a new binary file, 
  *             - uploading internal flash memory,
  *             - executing the binary file already loaded 
  *             - configuring the write protection of the Flash sectors where the 
  *               user loads his binary file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/** @addtogroup STM32L4xx_IAP
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "common.h"
#include "flash_if.h"
#include "menu.h"
#include "ymodem.h"

/* Private typedef -----------------------------------------------------------*/
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
//extern TIM_HandleTypeDef htim6;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
pFunction JumpToApplication;
uint32_t JumpAddress;
uint32_t FlashProtection = 0;
uint8_t aFileName[FILE_NAME_LENGTH];

uint8_t JUMP=0;

/* Private function prototypes -----------------------------------------------*/
void SerialDownload(void);
void SerialUpload(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Download a file via serial port
  * @param  None
  * @retval None
  */
	
//***************************************************************
//ISR function
//***************************************************************
//ISR start

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	  if (htim->Instance == htim6.Instance)
//	    {
	        /* Toggle LED */
		  	//HAL_GPIO_TogglePin(L1_GPIO_Port,L1_Pin); //Toggle LED
//		  Flexi_Run();

//		  if(Flexi_Read(2)>600){
//			  JUMP=1;
//		  }
//	    }
//}

void User_UART_Error_Handler(UART_HandleTypeDef *huart){
	int tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_PE);
	int tmp2 = __HAL_UART_GET_FLAG(huart, UART_FLAG_NE);
	int tmp3 = __HAL_UART_GET_FLAG(huart, UART_FLAG_ORE);
	int tmp4 = __HAL_UART_GET_FLAG(huart, UART_FLAG_FE);
	if(tmp1) __HAL_UART_CLEAR_PEFLAG(huart);
	if(tmp2) __HAL_UART_CLEAR_NEFLAG(huart);
	if(tmp3) __HAL_UART_CLEAR_OREFLAG(huart);
	if(tmp4) __HAL_UART_CLEAR_FEFLAG(huart);
	/*
	if(huart->ErrorCode != HAL_UART_ERROR_NONE)
	{
		__HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_PEF | UART_CLEAR_FEF);
		//__HAL_UART_FLUSH_DRREGISTER(huart);
	}

	if (huart->ErrorCode == HAL_UART_ERROR_PE) // Frame error
	{
	   __HAL_UART_CLEAR_PEFLAG(huart);
	   //huart->State = HAL_UART_STATE_BUSY_RX;
	}
	if (huart->ErrorCode == HAL_UART_ERROR_FE) // Frame error
	{
	   __HAL_UART_CLEAR_FEFLAG(huart);
	}
	if (huart->ErrorCode == HAL_UART_ERROR_NE) // Noise error
	{
	   __HAL_UART_CLEAR_NEFLAG(huart);
	}
	if (huart->ErrorCode == HAL_UART_ERROR_ORE) // overrun
	{
	   __HAL_UART_CLEAR_OREFLAG(huart);
	}
	*/

}


void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart){
	//HAL_NVIC_SystemReset();		//h

		//#define  HAL_UART_ERROR_NONE             ((uint32_t)0x00000000U)    /*!< No error                */
		//#define  HAL_UART_ERROR_PE               ((uint32_t)0x00000001U)    /*!< Parity error            */
		//#define  HAL_UART_ERROR_NE               ((uint32_t)0x00000002U)    /*!< Noise error             */
		//#define  HAL_UART_ERROR_FE               ((uint32_t)0x00000004U)    /*!< Frame error             */
		//#define  HAL_UART_ERROR_ORE              ((uint32_t)0x00000008U)    /*!< Overrun error           */
		//#define  HAL_UART_ERROR_DMA              ((uint32_t)0x00000010U)    /*!< DMA transfer error      */
		User_UART_Error_Handler(huart);
}






void SerialDownload(void)
{
  uint8_t number[11] = {0};
  uint32_t size = 0;
  COM_StatusTypeDef result;

  Serial_PutString((uint8_t *)"Waiting for the file to be sent ... (press 'a' to abort)\n\r");
  __HAL_UART_CLEAR_FLAG(&huart2, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_PEF | UART_CLEAR_FEF);
  result = Ymodem_Receive( &size );
  if (result == COM_OK)
  {
    Serial_PutString((uint8_t *)"\n\n\r Programming Completed Successfully!\n\r----\r\n Name: ");
    Serial_PutString(aFileName);
    Int2Str(number, size);
    Serial_PutString((uint8_t *)"\n\r Size: ");
    Serial_PutString(number);
    Serial_PutString((uint8_t *)" Bytes\r\n");
    Serial_PutString((uint8_t *)"-------------------\n");
  }
  else if (result == COM_LIMIT)
  {
    Serial_PutString((uint8_t *)"\n\n\rThe image size is higher than the allowed space memory!\n\r");
  }
  else if (result == COM_DATA)
  {
    Serial_PutString((uint8_t *)"\n\n\rVerification failed!\n\r");
  }
  else if (result == COM_ABORT)
  {
    Serial_PutString((uint8_t *)"\r\n\nAborted by user.\n\r");
  }
  else
  {
    Serial_PutString((uint8_t *)"\n\rFailed to receive the file!\n\r");
  }
}



/**
  * @brief  Display the Main Menu on HyperTerminal
  * @param  None
  * @retval None
  */
void Main_Menu(void)
{
  uint8_t key = 0;



  while (1)
  {

    /* Clean the input path */
	//__HAL_UART_RESET_HANDLE_STATE(&huart2);
    //__HAL_UART_FLUSH_DRREGISTER(&huart2);
    //HAL_UART_Init(&huart2);			//data come in when not polling then uart will hang
    //__HAL_UART_RESET_HANDLE_STATE(&huart2);
    //__HAL_UART_ENABLE(&huart2);
	//HAL_UART_AbortReceive(&huart2);
	//__HAL_UART_CLEAR_FLAG(&huart2, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_PEF | UART_CLEAR_FEF);
	    /* Disable PEIE, EIE, RXNEIE and RXFTIE interrupts */
	    //CLEAR_BIT((&huart2)->Instance->CR1, (USART_CR1_PEIE | USART_CR1_RXNEIE_RXFNEIE));
	    //CLEAR_BIT((&huart2)->Instance->CR3, USART_CR3_EIE | USART_CR3_RXFTIE);
    /* Receive key */
	HAL_StatusTypeDef status;
	status = HAL_TIMEOUT;
	while(status != HAL_OK){
		status = HAL_UART_Receive(&huart2, &key, 1, 500);	//RX_TIMEOUT
		int tmp1 = __HAL_UART_GET_FLAG(&huart2, UART_FLAG_PE);
		int tmp2 = __HAL_UART_GET_FLAG(&huart2, UART_FLAG_NE);
		int tmp3 = __HAL_UART_GET_FLAG(&huart2, UART_FLAG_ORE);
		int tmp4 = __HAL_UART_GET_FLAG(&huart2, UART_FLAG_FE);
		if(tmp1 || tmp2 || tmp3 || tmp4) key ='@';
		User_UART_Error_Handler(&huart2);
		if(JUMP==1)
		{
			status = HAL_OK;
			key = '3';
		}
		if(status == HAL_ERROR)
		{
			key = '*';
		}
	}

	char temp[2];
    switch (key)
    {
		case 'X' :
		  /* Download user application in the Flash */
		  //HAL_TIM_Base_Stop_IT(&htim6);
		  HAL_GPIO_WritePin(FORCEOFF_GPIO_Port, FORCEOFF_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(FORCEON_GPIO_Port, FORCEON_Pin, GPIO_PIN_SET);
		  while(!HAL_GPIO_ReadPin(READY_GPIO_Port,READY_Pin));
		  HAL_Delay(1);
		  SerialDownload();
		  break;
		case 'Y' :
			HAL_GPIO_WritePin(FORCEOFF_GPIO_Port, FORCEOFF_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(FORCEON_GPIO_Port, FORCEON_Pin, GPIO_PIN_SET);
			while(!HAL_GPIO_ReadPin(READY_GPIO_Port,READY_Pin));
			HAL_Delay(1);
			HAL_UART_Transmit(&huart1,"Y",1,100);			//send to next sensor
			HAL_GPIO_WritePin(FORCEOFF_GPIO_Port, FORCEOFF_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(FORCEON_GPIO_Port, FORCEON_Pin, GPIO_PIN_RESET);

			//HAL_TIM_Base_Stop_IT(&htim6);
			Serial_PutString((uint8_t *)"\r\nStart program execution for all\r\n");
			/* execute the new program */
			JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
			/* Jump to user application */
			JumpToApplication = (pFunction) JumpAddress;
			/* Initialize user application's Stack Pointer */
			__set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
			JumpToApplication();
		  /* Upload user application from the Flash */
		  //SerialUpload();
		  break;
		case 'Z' :
		  //HAL_TIM_Base_Stop_IT(&htim6);
		  Serial_PutString((uint8_t *)"\r\nStart program execution\r\n");
		  /* execute the new program */
		  JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
		  /* Jump to user application */
		  JumpToApplication = (pFunction) JumpAddress;
		  /* Initialize user application's Stack Pointer */
		  __set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
		  JumpToApplication();
		  break;
		case '4' :

		  break;
		default:

			temp[0]=(uint8_t)key;
			temp[1]=0;
			Serial_PutString((uint8_t *)"Invalid Input !<");
			Serial_PutString((uint8_t *)temp);
			Serial_PutString((uint8_t *)">\r\n");

		break;
    }
  }
}




/**
  * @}
  */
