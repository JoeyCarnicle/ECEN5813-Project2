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
#include "UART.h"

//#define Polling

/* TODO: insert other definitions and declarations here. */
void char_count(char receive_input);
void insert_count();

int readIRQ_Flag = 0;
ring_t *ring = NULL;
ring_t *printring = NULL;
char data = 0;
char pdata = 0;
char count[256];
int incount = 0;
int outcount = 0;

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

    ring = init(100);
    printring = init(100);
    uart_init();


    while(1) {
#ifdef Polling
    	data = receivechar();
    	placechar(data);
#else

    	if ((printring->Ini != printring->Outi) && (UART0->S1 & UART_S1_TC_MASK)){
    				r_remove(printring, &pdata);
    				uart_transmit(pdata);
    	}
    	else{
    		for (int i = 0; i < 100000; i++);
    				GPIOD->PTOR = (1<<1);
    	}

#endif

    }
    return 0 ;
}


void char_count(char receive_input)
{
    for(int i=0; i<256; i++)
    {
       if (i == receive_input)
       count[i] = count[i] + 1;
    }
}

void insert_count()
{
    for(int i=0;i<256;i++)
    {
        if(count[i] != 0)
        {
        	int oneish = count[i]%10;
        	int tenish = (count[i]%100)/10;
        	int hundredish = count[i]/100;
            insert(printring,i);
            insert(printring,'-');
            if (count[i]>=100){
            insert(printring, hundredish+48);
            }
            if (count[i]>=10){
            	insert(printring, tenish+48);
            }
            insert(printring, oneish+48);
            insert(printring, '\n');
            insert(printring, '\r');
        }
    }
}

void UART0_IRQHandler()
{
	DisableIRQ(UART0_IRQn);

	if(UART0->S1 & UART_S1_RDRF_MASK){
		data = receivechar();
		insert(ring, data);
		UART0->C2 |= UART0_C2_TIE_MASK;
	}
	else if (UART0->S1 & UART_S1_TDRE_MASK){
		if (ring->Ini != ring->Outi){
		       	r_remove(ring, &data);
		       	//uart_transmit(data);
		       	char_count(data);
		       	}
		else{
			UART0->C2 &= ~UART0_C2_TIE_MASK;
			insert_count();
			}
	}
	EnableIRQ(UART0_IRQn);

}



