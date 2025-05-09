/*
 * GccApplication8.c
 *
 * Created: 4/3/2025 3:43:37 PM
 * Author : jdupre
 */ 

#include "C:\Users\jdupre\Desktop\Libraries\F_cpu_lib.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include "C:\Users\jdupre\Desktop\Libraries\A2D_lib.h"
#include "C:\Users\jdupre\Desktop\Libraries\LCD_lib.h"
#include "C:\Users\jdupre\Desktop\Libraries\Motor_PWM_lib.h"
#include "C:\Users\jdupre\Desktop\Libraries\Serial_lib2.h"

int main(void)
{
	_delay_ms(1000);
	
	uint16_t serial_char_in;
	USART_vInit();
	
	HBridgeInit();
	
	int motorL = 1;
	int motorR = 0;
	
	double maxSpeed = 50;
	double turnSpeed = maxSpeed * 0.6;
	double sharpTurnSpeed = maxSpeed * 0.2;
	
	int leftOffset = 0;
	int rightOffset = 1;
	
	int reddetected = 0;

	_delay_ms(2000);
    while (reddetected == 0) 
    {
		
		//Motor offset calibration
		/*
		HBridgeCommand(motorL, maxSpeed - leftOffset, 1);
		HBridgeCommand(motorR, maxSpeed- rightOffset, 0);
		*/
		
		//Serial communication
		if (!serialCheckRxComplete())continue;
		serial_char_in = USART_vReceiveByte();
		
		//Motor control
		switch(serial_char_in)
		{
			case ('A'): //Far right
			{
				HBridgeCommand(motorL, maxSpeed - leftOffset, 1);
				HBridgeCommand(motorR, sharpTurnSpeed - rightOffset, 0);
				USART_vSendByte('A');
				break;
			}
			case ('B'): //Right
			{
				HBridgeCommand(motorL, maxSpeed - leftOffset, 1);
				HBridgeCommand(motorR, turnSpeed - rightOffset, 0);
				USART_vSendByte('B');
				break;
			}
			case ('C'): //Middle
			{
				HBridgeCommand(motorL, maxSpeed - leftOffset, 1);
				HBridgeCommand(motorR, turnSpeed - rightOffset, 0);
				USART_vSendByte('C');
				break;
			}
			case ('D'): //Left
			{
				HBridgeCommand(motorL, turnSpeed - leftOffset, 1);
				HBridgeCommand(motorR, maxSpeed - rightOffset, 0);
				USART_vSendByte('D');
				break;
			}
			case ('E'): //Far Left
			{
				HBridgeCommand(motorL, sharpTurnSpeed - leftOffset, 1);
				HBridgeCommand(motorR, maxSpeed - rightOffset, 0);
				USART_vSendByte('E');
				break;
			}
			case ('F'): //Red line detected
			{
				reddetected = 1;
				_delay_ms(5000);
				HBridgeCommand(motorL, 0, 1);
				HBridgeCommand(motorR, 0, 0);
				USART_vSendByte('F');
				break;
			}
			default: //System is off
			{
				HBridgeCommand(motorL, 0, 1);
				HBridgeCommand(motorR, 0, 0);
				USART_vSendByte('G');
				reddetected = 0;
				break;
			}
		}
		
		_delay_ms(1000);
    }
}

