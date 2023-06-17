/* FILENAME: Mob_APP_program 
*  Author:  Ali El Bana
*  Version:  V1.0
*  DATE:   Sun 03/26/2023
*/

/************************************************************************/
/*                        Include headers                        	    */
/************************************************************************/

#include "stdlib.h"
#include "string.h"

#include "../../LIB/LSTD_TYPES.h"
#include "../../LIB/LSTD_COMPILER.h"
#include "../../LIB/LSTD_VALUES.h"
#include "../../LIB/LSTD_BITMATH.h"

#include "../../MCAL/GPIO/GPIO_interface.h"

#include "../../HAL/Bluetooth/Bluetooth_interface.h"
#include "../../HAL/DCMOTOR/DCM_interface.h"
#include "../../HAL/Car_Movement/Car_Movement_interface.h"
#include "../../HAL/UltraSonic/UltraSonic_interface.h"
#include "../../HAL/BUZZER/BUZZER_interface.h"

#include "../Traditional_Mode/Traditional_Mode_interface.h"
#include "../NCC/NCC_interface.h"
#include "../ACC/ACC_interface.h"
#include "../FCW/FCW_interface.h"

#include "Mob_APP_interface.h"
#include "Mob_APP_private.h"
#include "Mob_APP_config.h"

/************************************************************************/
/*                     Functions implementations                      	*/
/************************************************************************/

void AMobApp_vInit( void )
{

	/* Initialization of Bluetooth Module */
	HBluetooth_vInit( ) ;

	/* Print "Successfully connected" on the mob application terminal */
	HBluetooth_vSendString( "\nSuccessfully connected\n" ) ;

}

/**************************************************************************************/
/**************************************************************************************/
void AMobApp_vSendSpeedValue( u32_t A_u32SpeedValue )
{

	/* A string holds two chars: '*' and 's' */
	char L_strSpeedIndc[9] = "*S" ;

	/* A string holds the speed value */
	char L_strSpeedStr[6] ;

	/* Convert the speed value from integer to string and store it in speedStr */
	itoa( A_u32SpeedValue, L_strSpeedStr, DECIMAL ) ;

	/* Concatenate "*S" with the speed value string to result a string like that "*S100" */
	strcat( L_strSpeedIndc, L_strSpeedStr ) ;

	/* Concatenate speed value string with "*" to result a string like that "*S100*" */
	strcat( L_strSpeedIndc, "*" ) ;

	/* Then send this string through bluetooth to the speed indicator */
	HBluetooth_vSendString( L_strSpeedIndc ) ;

}

/**************************************************************************************/
/**************************************************************************************/

void AMobApp_vCntrlCar( void )
{

	/* A variable holds the char of the received state */
	c8_t L_c8RecievedState = TRAD_MODE_CHAR ;

	/* Print "Car is ready to be controlled" on the mob application terminal */
	HBluetooth_vSendString( "\nCar is ready to be controlled\n" ) ;

	while( TRUE )
	{

		/* Disable the UART reception IRQ */
		HBluetooth_vDisableAsynchReceive( ) ;

		/* Display the car modes options on the mob application terminal */
		HBluetooth_vSendString( "\nSelect one of these modes:\n" ) ;
		HBluetooth_vSendString( "\n1-Traditional Mode\n" ) ;
		HBluetooth_vSendString( "\n2-NCC Mode\n" ) ;
		HBluetooth_vSendString( "\n3-ACC Mode\n" ) ;
		HBluetooth_vSendString( "\n4-FCW Mode\n" ) ;
		HBluetooth_vSendString( "\n5-Exit Mode\n" ) ;

		/* This is a blocking (polling) function that receives a char and return it to a variable */
		L_c8RecievedState = HBluetooth_u8ReceiveByte( ) ;

		/* Car mode decision according to the received char */
		switch( L_c8RecievedState )
		{

			/* If the received char 'T' turn on the traditional mode */
			case TRAD_MODE_CHAR	: ATraditional_vModeON( ) ; break ;

			/* If the received char 'N' turn on the normal cruise control mode */
			case NCC_MODE_CHAR	: ANCC_vModeON( ) 		  ; break ;

			/* If the received char 'A' turn on the adaptive cruise control mode */
			case ACC_MODE_CHAR	: AACC_vModeON( )         ; break ;

			/* If the received char 'F' turn on the forward collision mode */
			case FCW_MODE_CHAR	: AFCW_vModeON( )         ;	break ;

			/* If any another char sent the default would be the traditional mode */
			default				: ATraditional_vModeON( ) ;	break ;

		}

	}

}

/**************************************************************************************/
/**************************************************************************************/








































