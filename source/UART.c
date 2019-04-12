/*
 * UART.c
 *
 *  Created on: Apr 11, 2019
 *      Author: carni
 */

#include "MKL25Z4.h"
#include "UART.h"

//#define Polling

void uart_init()
{
	SIM->SCGC4 |= SIM_SCGC4_UART0(1);
	SIM->SCGC5 |= SIM_SCGC5_PORTA(1);
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(01);

	PORTA->PCR[1] |= PORT_PCR_MUX(2); //RX
	PORTA->PCR[2] |= PORT_PCR_MUX(2);	//TX

	/* LED init */
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	GPIOD->PSOR |= (1<<1);
	GPIOD->PDDR |= (1<<1);
	PORTD->PCR[1] |= PORT_PCR_MUX(1);

	UART0->BDH = 0x00;
	UART0->C2 = 0x00;
	UART0->BDL = 3; //460800
#ifdef Polling
	UART0->C2 |= UART0_C2_TE(1);
	UART0->C2 |= UART0_C2_RE(1);
	UART0->S1 = 0;
#else
	NVIC_EnableIRQ(UART0_IRQn);
	UART0->C2 |= UART0_C2_TE(1);
	UART0->C2 |= UART0_C2_RE(1);
	UART0->C2 |= UART0_C2_RIE(1);
	UART0->C2 |= UART0_C2_TIE(1);
	UART0->S1 = 0;
#endif

}

void placechar(char data)
{
	int ret = -1;
	ret = uart_transmit_ready();
	if (ret == 0){
		uart_transmit(data);
	}
}

char receivechar()
{
	int ret = -1;
	ret = uart_receive_ready();
	if (ret == 0){
		return (uart_receive());
	}
	return ret;
}

int uart_transmit_ready(void)
{
	int ret = -1;
	while(!(UART0->S1 & UART_S1_TC_MASK) && !(UART0->S1 & UART_S1_TDRE_MASK)){}
	ret = 0;
	return ret;
}

void uart_transmit(char input)
{
	UART0->D = input;
}

int uart_receive_ready()
{
	int ret = -1;
	while (!(UART0->S1 & UART_S1_RDRF_MASK)){}
	ret = 0;
	return ret;
}

char uart_receive()
{
	return UART0->D;
}

