/* FILENAME: FCW_program 
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

#include "../../MCAL/GPIO/GPIO_interface.h"

#include "../../HAL/Bluetooth/Bluetooth_interface.h"
#include "../../HAL/BUZZER/BUZZER_interface.h"
#include "../../HAL/DCMOTOR/DCM_interface.h"
#include "../../HAL/Car_Movement/Car_Movement_interface.h"
#include "../../HAL/UltraSonic/UltraSonic_interface.h"

#include "../Mob_APP/Mob_APP_interface.h"

#include "FCW_interface.h"
#include "FCW_private.h"
#include "FCW_config.h"

/**************************************************************************************/
/**************************************************************************************/

/* A variable that holds the char of the pressed button from user */
c8_t volatile G_c8RecievedButton = INITIAL_ZERO ;

/* A flag is used to determine if an ISR called or not */
u8_t volatile GV_u8InISR		 = FALSE		;

/* Define the ultrasonic pins */
static VAR(HULTSNC_ConfigType)
TRIG =
{
    .u8Port = GPIO_PORTB ,
    .u8Pin  = GPIOx_PIN8
} ;

/* Define the buzzer pins */
static VAR(BUZZER_BuzzerConfiguration)
Buzzer =
{
    .u8Port = GPIO_PORTB ,
    .u8Pin  = GPIOx_PIN5
} ;

/**************************************************************************************/
/**************************************************************************************/

/* This is a call back UART RX function */
static void Button_vCurrentData( void )
{
	/* This is a non-blocking (interrupt) function that receives a char and return it to a variable */
	G_c8RecievedButton 	= HBluetooth_u8GetDataRegister( ) 	;

	/* Assign true to the flag to determine that we entered an ISR */
	GV_u8InISR		 	= TRUE								;
}

/**************************************************************************************/
/**************************************************************************************/

void AFCW_vModeON( void )
{

	/* A variable that holds the value of the car speed */
	u32_t L_u32SpeedValue 	= INITIAL_ZERO ;

	/* A variable that holds the value of the ultrasonic measured distance */
	f32_t L_f32Distance 	= INITIAL_ZERO 	;

	/* Initialize the received button value by zero */
	G_c8RecievedButton 		= INITIAL_ZERO 	;

	/* Initialize the ISR flag with false */
	GV_u8InISR		 		= FALSE			;

	/* Print "FCW Mode ON" on the mob application terminal */
	HBluetooth_vSendString( "\nFCW Mode ON\n" ) ;

	/* Initialize the buzzer */
	HBUZZER_vInit( &Buzzer ) ;

	/* Enable the UART reception IRQ */
	HBluetooth_vEnableAsynchReceive( ) ;

	/* This function passes the address of the called back function */
	HBluetooth_u8AsynchReceiveByte( &Button_vCurrentData ) ;

	/* Keep triggering the ultrasonic and receiving the pressed buttons from the user until he pressed the exit button ('e') */
	do
	{

		/* Trigger the ultrasonic */
		HULTSNC_vTrigger( &TRIG ) ;

		/* Get the distance from the ultrasonic and assign it to the variable */
		L_f32Distance = HULTSNC_f32GetDistance(  ) ;

		/* If the distance less than 80cm --> turn on the buzzer */
		if( L_f32Distance < FCW_SAFE_DIST )
		{
			/* Turn on the buzzer */
			HBUZZER_vSoundOn( &Buzzer ) ;

			/* Send "Warning there's an object 80cm away" to the mob application */
			HBluetooth_vSendString( "\nWarning there's an object 80cm away\n" ) ;
		}

		/* If the distance bigger than or equal 80cm --> turn off the buzzer */
		else if( L_f32Distance >= FCW_SAFE_DIST + HYSTRS_VALUE )
		{
			/* Turn off the buzzer */
			HBUZZER_vSoundOff( &Buzzer ) ;
		}
		else
		{
			/* Do Nothing */
		}

		/* If the user sends a char an ISR will be fired then the flag would be true */
		if( GV_u8InISR == TRUE )
		{
			/* Reset the flag to false again */
			GV_u8InISR = FALSE ;

			/* Car movement/speed decision according to the received char */
			switch( G_c8RecievedButton )
			{

				/* If the received char 'f' car moves forward */
				case FORW_CHAR:

					/* Make the 4 DC motors move CW */
					HCarMove_vForward( ) ;

					/* Get the car speed and assign its value to this variable */
					L_u32SpeedValue = HCarMove_u32GetCarSpeed(  ) ;

					/* Send the speed value to mob application to present it on the speed indicator */
					AMobApp_vSendSpeedValue( L_u32SpeedValue ) ;

				break ;

				/* If the received char 'b' car moves backward */
				case BACKW_CHAR:

					/* Make the 4 DC motors move CCW */
					HCarMove_vBackward( ) ;

					/* Get the car speed and assign its value to this variable */
					L_u32SpeedValue = HCarMove_u32GetCarSpeed(  ) ;

					/* Send the speed value to mob application to present it on the speed indicator */
					AMobApp_vSendSpeedValue( L_u32SpeedValue ) ;

				break ;

				/* If the received char 'r' car moves right */
				case RIGHT_CHAR:

					/* Stop the right side 2 DC motors and make the left side 2 DC motors move CW */
					HCarMove_vRight( ) ;

					/* Get the car speed and assign its value to this variable */
					L_u32SpeedValue = HCarMove_u32GetCarSpeed(  ) ;

					/* Send the speed value to mob application to present it on the speed indicator */
					AMobApp_vSendSpeedValue( L_u32SpeedValue ) ;

				break ;

				/* If the received char 'l' car moves right */
				case LEFT_CHAR:

					/* Stop the left side 2 DC motors and make the right side 2 DC motors move CW */
					HCarMove_vLeft( ) ;

					/* Get the car speed and assign its value to this variable */
					L_u32SpeedValue = HCarMove_u32GetCarSpeed(  ) ;

					/* Send the speed value to mob application to present it on the speed indicator */
					AMobApp_vSendSpeedValue( L_u32SpeedValue ) ;

				break ;

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

				/* If the received char 's' car stops */
				case STOP_CHAR:

					/* Stop the 4 DC motors */
					HCarMove_vStop( ) ;

					/* Send the speed value to mob application to present it on the speed indicator */
					HBluetooth_vSendString( SPEED0_STR ) ;

				break ;

				/* If the received char 'e' exit from the traditional mode */
				case EXIT_MODE_CHAR:

					/* Turn off the buzzer */
					HBUZZER_vSoundOff( &Buzzer ) ;

					/* Stop the 4 DC motors */
					HCarMove_vStop( ) ;

					/* Send the speed value to mob application to present it on the speed indicator */
					HBluetooth_vSendString( SPEED0_STR ) ;

					/* Clear the address of the passed call back function */
					HBluetooth_u8AsynchReceiveByte( NULL ) ;

				break ;

				default: /* Do Nothing */ break ;
			}

		}
		else
		{
			/* Do nothing */
		}

	/* In case the user sends 'e' break the loop and return to the Mob_APP_program */
	}while( G_c8RecievedButton != EXIT_MODE_CHAR ) ;

}

/**************************************************************************************/
/**************************************************************************************/































