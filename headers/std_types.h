 /******************************************************************************
 *
 * Module: Common - Platform Types Abstraction
 *
 * File Name: std_types.h
 *
 * Description: types for AVR
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#ifndef STD_TYPES_H_
#define STD_TYPES_H_

/* Boolean Data Type */
typedef unsigned char boolean;

/* Boolean Values */
#ifndef FALSE
#define FALSE       (0u)
#endif
#ifndef TRUE
#define TRUE        (1u)
#endif

#define LOGIC_HIGH        (1u)
#define LOGIC_LOW         (0u)

#define NULL_PTR    ((void*)0)

typedef enum
	{NO_PASSWORD,
	PASSWORD_CREATED,
	PASSWORD_PROTECTED} ProtectionState;
typedef enum {
	ENTERING_PASSWORD_STATE,
	WRITEING_PASSWORD_STATE,
	REENTERING_PASSWORD_STATE,
	CONFIRMING_PASSWORD_STATE,
	SAVED_PASSWORD_STATE} PasswordStage;

typedef enum {ADC_PRESCAER_2,
			  ADC_PRESCAER_2_R,
			  ADC_PRESCAER_4,
			  ADC_PRESCAER_8,
			  ADC_PRESCAER_16,
			  ADC_PRESCAER_32,
			  ADC_PRESCAER_64,
			  ADC_PRESCAER_128}ADC_Prescaler;

typedef enum {ADC_REF_VOLTAGE_AREF,
			 ADC_REF_VOLTAGE_ACC,
		     ADC_REF_VOLTAGE_INTERNAL = 3}ADC_RefrenceVoltage;
typedef enum {DC_Motor_State_CW,DC_Motor_State_ACW,DC_Motor_State_STOP}DCMotor_State;

typedef unsigned char         uint8;          /*           0 .. 255              */
typedef signed char           sint8;          /*        -128 .. +127             */
typedef unsigned short        uint16;         /*           0 .. 65535            */
typedef signed short          sint16;         /*      -32768 .. +32767           */
typedef unsigned long         uint32;         /*           0 .. 4294967295       */
typedef signed long           sint32;         /* -2147483648 .. +2147483647      */
typedef unsigned long long    uint64;         /*       0 .. 18446744073709551615  */
typedef signed long long      sint64;         /* -9223372036854775808 .. 9223372036854775807 */
typedef float                 float32;
typedef double                float64;
typedef struct
{
	ADC_Prescaler preScaler;
	ADC_RefrenceVoltage refrenceVoltage;

}ADC_ConfigType;

#endif /* STD_TYPE_H_ */
