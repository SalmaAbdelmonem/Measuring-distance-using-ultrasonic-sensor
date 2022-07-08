/******************************************************************************
 *
 * Module: Ultrasonic Sensor
 *
 * File Name: ultrasonic_sensor.c
 *
 * Description: Source file for the AVR Ultrasonic Sensor driver
 *
 * Author: Salma AbdelMonem
 *
 *******************************************************************************/
#include "icu.h"
#include "gpio.h"
#include "common_macros.h"
#include "ultrasonic_sensor.h"
#include <util\delay.h>
#include <avr\io.h>

uint8 g_edgeCount = 0;
uint16 g_timeHigh = 0;
uint16 distance = 0;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description : Function to initialize the Ultrasonic Sensor driver
 * 	1. Initialize the ICU driver as required.
 * 	2. Setup the ICU call back function.
 * 	3. Setup the direction for the trigger pin.
 */
void Ultrasonic_init(void)
{
	/* Create configuration structure for ICU driver */
	Icu_ConfigType Icu_Config = {F_CPU_8,RISING};

	Icu_init(&Icu_Config);
	Icu_setCallBack(Ultrasonic_edgeProcessing);

	GPIO_setupPinDirection(PORTB_ID, PIN5_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(PORTD_ID, PIN6_ID, PIN_INPUT);
}

/*
 * Description : Function to trigger the Ultrasonic Sensor driver
 * 	1. Send the Trigger pulse to the ultrasonic.
 */

void Ultrasonic_Trigger(void)
{
	SET_BIT(PORTB,PIN5_ID);
	_delay_us(10);
	CLEAR_BIT(PORTB,PIN5_ID);
}

/*
 * Description : Function to read distance from the Ultrasonic Sensor driver
 * 	1. Send the trigger pulse by using Ultrasonic_Trigger function.
 * 	2. Start the measurements by the ICU from this moment.
 */
uint16 Ultrasonic_readDistance(void)
{

	Ultrasonic_Trigger();

	if(g_edgeCount == 2)
	{
		Icu_DeInit();
		g_edgeCount = 0;
		distance = ((float32)g_timeHigh / 58.8);
	}
	return distance;
}

/*
 * Description : Function to get high time of edge
 * 	1. This is the call back function called by the ICU driver.
 * 	2. This is used to calculate the high time (pulse time) generated by the ultrasonic sensor.
 */
void Ultrasonic_edgeProcessing(void)
{
	g_edgeCount++;
	if(g_edgeCount == 1)
	{
		/*
		 * Clear the timer counter register to start measurements from the
		 * first detected rising edge
		 */
		Icu_clearTimerValue();
		/* Detect falling edge */
		Icu_setEdgeDetectionType(FALLING);
	}
	else if(g_edgeCount == 2)
	{
		/* Store the High time value */
		g_timeHigh = Icu_getInputCaptureValue();
		/* Detect rising edge */
		Icu_setEdgeDetectionType(RISING);
	}
}