/******************************************************************************
 *
 * Date: 13/10/2021
 *
 * File Name: Mini Project 4
 *
 * Description: Measuring distance using ultrasonic sensor.
 *
 * Author: Salma AbdelMonem
 *
 *******************************************************************************/

#include "icu.h"
#include "lcd.h"
#include "ultrasonic_sensor.h"
#include <avr\io.h>

int main(void)
{
	uint16 distance;

	/*Initialize the LCD*/
	LCD_init();

	/* Enable Global Interrupt I-Bit */
	SREG |= (1<<7);

	/* Display this string "Distance =   " only once on LCD at the first row */
	LCD_displayString("Distance=     cm");

	while(1)
	{
		/*Function to initialize the Ultrasonic Sensor driver*/
		Ultrasonic_init();

		/*Calculate distance using Ultrasonic_readDistance function*/
		distance = Ultrasonic_readDistance();

		LCD_moveCursor(0,0x0A);

		/* Display sensed distance */
		if(distance >= 100)
		{
			LCD_intgerToString( distance );
		}
		else
		{
			/* In case the digital value is two or one digits print space in the next digit place */
			LCD_intgerToString( distance );
			LCD_displayCharacter(' ');
		}
	}
}
