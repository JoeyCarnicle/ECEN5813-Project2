/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or withdata modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software withdata specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY data OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    ECEN5813_Project2.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "system_MKL25Z4.h"
#include "fsl_debug_console.h"
#include "fsl_uart.h"
#include "fsl_common.h"
#include "ring.h"

//#define Polling 0

/* TODO: insert other definitions and declarations here. */
void uart_init();
void uart_transmit(char input);
char uart_receive();
int BaudCalc();

int readIRQ_Flag = 0;
ring_t *ring = NULL;
char data = 0;

/*
 * @brief   Application entry point.
 */
int main(void) {

  	/* Init board hardware. */
	BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    uart_init();

    ring = init(100);

    while(1) {
#ifdef Polling
    	data = uart_receive();
    	uart_transmit(data);
#else
       	/*if (readIRQ_Flag == 1){
    	insert(ring, data);
    	readIRQ_Flag = 0;
    	}
       	if (ring->Ini != ring->Outi){
       	r_remove(ring, &data);
       	uart_transmit(data);
       	}*/
#endif

    }
    return 0 ;
}

void uart_init()
{
	SIM->SCGC4 |= SIM_SCGC4_UART0(1);
	SIM->SCGC5 |= SIM_SCGC5_PORTA(1);
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(01);

	PORTA->PCR[1] |= PORT_PCR_MUX(010); //RX
	PORTA->PCR[2] |= PORT_PCR_MUX(010);	//TX

	UART0->BDH = 0x00;
	UART0->C2 = 0x00;
	UART0->BDL = BaudCalc();
#ifdef Polling
	UART0->C2 |= UART0_C2_TE(1);
	UART0->C2 |= UART0_C2_RE(1);
#else
	UART0->C2 |= UART0_C2_TE(1);
	UART0->C2 |= UART0_C2_RE(1);
	UART0->C2 |= UART0_C2_RIE(1);
	UART0->C2 |= UART0_C2_TIE(1);
	NVIC_EnableIRQ(UART0_IRQn);
#endif
	UART0->S1 = 0;

}

void uart_transmit(char input)
{
	while(!(UART0->S1 & UART_S1_TC_MASK) && !(UART0->S1 & UART_S1_TDRE_MASK)){}
	UART0->D = input;
}

char uart_receive()
{
	while (!(UART0->S1 & UART_S1_RDRF_MASK)){}
	return UART0->D;
}

int BaudCalc()
{
	int bd;
	bd = DEFAULT_SYSTEM_CLOCK/(16*115200);
	return bd;
}

void UART0_IRQHandler()
{
	DisableIRQ(UART0_IRQn);

	if(UART0->S1 & UART_S1_RDRF_MASK){
		data = UART0->D;
		insert(ring, data);
	}
	else if (UART0->S1 & UART_S1_TDRE_MASK){
		if (ring->Ini != ring->Outi){
		       	r_remove(ring, &data);
		       	uart_transmit(data);
		       	}
	}
	EnableIRQ(UART0_IRQn);

}




