/* FILENAME: main.c
*  Author:  Ali El Bana
*  Version:  V1.0
*  DATE:   sat 6/11/2022
*/

/************************************************************************/
/*                        Include headers                        	    */
/************************************************************************/

#include "COTS/LIB/LSTD_TYPES.h"
#include "COTS/LIB/LSTD_COMPILER.h"
#include "COTS/LIB/LSTD_VALUES.h"
#include "COTS/LIB/LSTD_BITMATH.h"

#include "COTS/MCAL/RCC/MRCC_interface.h"
#include "COTS/MCAL/SysTick/SysTick_interface.h"
#include "COTS/MCAL/GPIO/GPIO_interface.h"
#include "COTS/MCAL/TIM1/TIM1_interface.h"

#include "COTS/HAL/DCMOTOR/DCM_interface.h"
#include "COTS/HAL/Car_Movement/Car_Movement_interface.h"
#include "COTS/HAL/UltraSonic/UltraSonic_interface.h"

#include "COTS/APP/Mob_APP/Mob_APP_interface.h"

/************************************************************************/
/*                     Functions implementations                      	*/
/************************************************************************/

int main( void )
{

	/* Define the ultrasonic pins */
	VAR(HULTSNC_ConfigType)
	TRIG =
	    {
	        .u8Port = GPIO_PORTB,
	        .u8Pin  = GPIOx_PIN8
	    };

	/* Initiate RCC for PORTA, PORTB and TIM1 */
	MRCC_vInit( ) ;
	MRCC_vEnablePeriphralCLK( RCC_AHB1, AHB1ENR_GPIOAEN  ) ;
	MRCC_vEnablePeriphralCLK( RCC_AHB1, AHB1ENR_GPIOBEN  ) ;
	MRCC_vEnablePeriphralCLK( RCC_APB2, APB2ENR_TIM1EN   ) ;

	/* Lock GPIO pins specified for SW debug */
	MGPIOx_vLockedPins( ) ;

	/* Initiate STK timer */
	MSysTick_vInit( ) ;

	/* Initialize car motors */
	HCarMove_vInit( ) ;

	/* Set initial speed of the car (half of the speed) */
	HCarMove_vSpeedRatio( SPEED_50_PERCENT ) ;

	/* Initialize ultrasonic  */
	HULTSNC_vInit( &TRIG ) ;

	/* Initiate TIM1 and enable the counter */
	MTIM1_vEnableCounter( ) ;

	/* Initialize the mobile application to send/receive commands */
	AMobApp_vInit( ) ;

	while( TRUE )
	{
		/* Ask the user about the mode and run it */
		AMobApp_vCntrlCar( ) ;

	}

}

/**************************************************************************************/
/**************************************************************************************/

