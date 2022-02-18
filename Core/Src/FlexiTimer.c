/*
 * FlexiTimer.c
 *
 *  Created on: Aug 8, 2017
 *      Author: zuoha
 */
#include "FlexiTimer.h"
#include "main.h"
//timer variable
//*********************************************************
unsigned long int D[13];
//*********************************************************
//TIM_HandleTypeDef htim6;

void Flexi_Init(){
//	HAL_TIM_Base_Start_IT(&htim6);


}


void Flexi_Run(){
	D[0]++;    //RFID 0
	D[1]++;    //RFID 1
	D[2]++;      //SYNC
	D[3]++;      //LED
	D[4]++;      //Relay0
	D[5]++;      //Relay
	D[6]++;      //msync
	D[7]++;      //debugging
	D[8]++;      //FREE MEMORY
	D[9]++;      //system check
	D[10]++;     //LEDdisplay
	D[11]++;     //master reset
	D[12]++;     //serial debugging

	for(int i=0;i<11;i++){
		if(D[i]>=0xfffe){D[i]=0xfffe;}
	}

}

unsigned long int Flexi_Read(int i){
	return D[i];

}


int Flexi_Set(int i,unsigned long int j){

	D[i]=j;
	return 1;

}

int Flexi_Reset(int i){

	D[i]=0;
	return 1;

}
