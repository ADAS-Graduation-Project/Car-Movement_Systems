/* FILENAME: NCC_program 
*  Author:  Ali El Bana
*  Version:  V1.0
*  DATE:   Mon 04/17/2023
*/


/************************************************************************/
/*                        Include headers                        	    */
/************************************************************************/

#include "../../LIB/LSTD_TYPES.h"
#include "../../LIB/LSTD_COMPILER.h"
#include "../../LIB/LSTD_VALUES.h"
#include "../../LIB/LSTD_BITMATH.h"

#include "../../HAL/Bluetooth/Bluetooth_interface.h"
#include "../../HAL/DCMOTOR/DCM_interface.h"
#include "../../HAL/Car_Movement/Car_Movement_interface.h"

#include "../Mob_APP/Mob_APP_interface.h"

#include "NCC_interface.h"
#include "NCC_private.h"
#include "NCC_config.h"

/**************************************************************************************/
/**************************************************************************************/

void ANCC_vModeON( void )
{

	/* A variable that holds the value of the car speed */
	u32_t L_u32SpeedValue 		= INITIAL_ZERO ;

	/* A variable that holds the char of the pressed button from user */
	c8_t  L_c8RecievedButton 	= INITIAL_ZERO ;

	/* Print "NCC Mode ON" on the mob application terminal */
	HBluetooth_vSendString( "\nNCC Mode ON\n" ) ;

	do
	{

		/* Car moves forward */
		HCarMove_vForward( ) ;

		/* Get the car speed and assign its value to this variable */
		L_u32SpeedValue = HCarMove_u32GetCarSpeed(  ) ;

		/* Send the speed value to mob application to present it on the speed indicator */
		AMobApp_vSendSpeedValue( L_u32SpeedValue ) ;

		/* This is a blocking (polling) function that receives a char and return it to a variable */
		L_c8RecievedButton = HBluetooth_u8ReceiveByte( ) ;

		/* Car speed decision according to the received char */
		switch( L_c8RecievedButton )
		{

			/* If the received char '+' car speed increases by 10 percent */
			case INC_SPEED_CHAR:

				/* Get the last car speed and assign its value to this variable */
				L_u32SpeedValue = HCarMove_u32GetCarSpeed(  ) ;

				/* If the car speed is 100% already and you want to increase it, it will remain at 100% */
				if( L_u32SpeedValue >= SPEED_100_PERCENT )
				{
					/* Keep the car speed at 100% */
					L_u32SpeedValue = SPEED_100_PERCENT ;

					/* Send the speed value to mob application to present it on the speed indicator */
					AMobApp_vSendSpeedValue( L_u32SpeedValue ) ;
				}

				/* If it's not at the 100%, increase it by 10% */
				else
				{
					/* Increase the car speed by 10% */
					L_u32SpeedValue += SPEED_10_PERCENT ;

					/* Make the car move by the new speed */
					HCarMove_vSpeedRatio( L_u32SpeedValue ) ;

					/* Send the speed value to mob application to present it on the speed indicator */
					AMobApp_vSendSpeedValue( L_u32SpeedValue ) ;
				}

			break ;

			/* If the received char '-' car speed decreases by 10 percent */
			case DEC_SPEED_CHAR:

				/* Get the last car speed and assign its value to this variable */
				L_u32SpeedValue = HCarMove_u32GetCarSpeed(  ) ;

				/* If the car speed is 0% already and you want to decrease it, it will remain at 0% */
				if( L_u32SpeedValue <= SPEED_0_PERCENT )
				{
					/* Keep the car speed at 0% */
					L_u32SpeedValue = SPEED_0_PERCENT ;

					/* Send the speed value to mob application to present it on the speed indicator */
					AMobApp_vSendSpeedValue( L_u32SpeedValue ) ;
				}

				/* If it's not at the 0%, decrease it by 10% */
				else
				{
					/* Decrease the car speed by 10% */
					L_u32SpeedValue -= SPEED_10_PERCENT ;

					/* Make the car move by the new speed */
					HCarMove_vSpeedRatio( L_u32SpeedValue ) ;

					/* Send the speed value to mob application to present it on the speed indicator */
					AMobApp_vSendSpeedValue( L_u32SpeedValue ) ;
				}

			break ;

			/* If the received char 'e' exit from the traditional mode */
			case EXIT_MODE_CHAR:

					/* Stop the 4 DC motors */
					HCarMove_vStop( ) ;

					/* Send the speed value to mob application to present it on the speed indicator */
					HBluetooth_vSendString( SPEED0_STR ) ;

					/* Print "NCC Mode Off" on the mob application terminal */
					HBluetooth_vSendString( "\nNCC Mode Off\n" ) ;

				break ;

			default: /* Do Nothing */ break ;
		}

	/* In case the user sends 'e' break the loop and return to the Mob_APP_program */
	}while( L_c8RecievedButton != EXIT_MODE_CHAR ) ;

}

/**************************************************************************************/
/**************************************************************************************/




























