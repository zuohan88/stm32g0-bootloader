/*
 * FlexiTimer.h
 *
 *  Created on: Aug 8, 2017
 *      Author: zuoha
 */

#ifndef SOURCES_FLEXITIMER_H_
#define SOURCES_FLEXITIMER_H_


void Flexi_Init();
void Flexi_Run();
unsigned long int Flexi_Read(int i);
int Flexi_Set(int i,unsigned long int j);
int Flexi_Reset(int i);


#define timer_uart1 	1
#define timer_uart2		2
#define timer_uart3		3
#define timer_uart4		4
#define timer_uart5		5
#define timer_uart6		6


#define timer_LED		10

#endif /* SOURCES_FLEXITIMER_H_ */
