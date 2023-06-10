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

	VAR(HULTSNC_ConfigType)
	TRIG =
	    {
	        .u8Port = GPIO_PORTB,
	        .u8Pin  = GPIOx_PIN8
	    };

	MRCC_vInit( ) ;

	MRCC_vEnablePeriphralCLK( RCC_AHB1, AHB1ENR_GPIOAEN  ) ;
	MRCC_vEnablePeriphralCLK( RCC_AHB1, AHB1ENR_GPIOBEN  ) ;
	MRCC_vEnablePeriphralCLK( RCC_APB2, APB2ENR_TIM1EN   ) ;

	MGPIOx_vLockedPins( ) ;

	MSysTick_vInit( ) ;

	HCarMove_vInit( ) ;

	HCarMove_vSpeedRatio( SPEED_50_PERCENT ) ;

	HULTSNC_vInit( &TRIG ) ;

	MTIM1_vEnableCounter( ) ;

	AMobApp_vInit( ) ;

	while( TRUE )
	{

		AMobApp_vCntrlCar( ) ;

	}

}

/**************************************************************************************/
/**************************************************************************************/

