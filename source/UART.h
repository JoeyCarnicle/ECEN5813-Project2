/*
 * UART.h
 *
 *  Created on: Apr 11, 2019
 *      Author: carni
 */

#ifndef UART_H_
#define UART_H_

void uart_init();
void placechar(char data);
int uart_transmit_ready(void);
void uart_transmit(char input);
char receivechar(void);
int uart_receive_ready();
char uart_receive();

#endif /* UART_H_ */
