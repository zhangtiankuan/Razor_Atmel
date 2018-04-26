/**********************************************************************************************************************
File: user_app1.h                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
1. Follow the instructions at the top of user_app1.c
2. Use ctrl-h to find and replace all instances of "user_app1" with "yournewtaskname"
3. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
4. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
5. Add #include yournewtaskname.h" to configuration.h
6. Add/update any special configurations required in configuration.h (e.g. peripheral assignment and setup values)
7. Delete this text (between the dashed lines)
----------------------------------------------------------------------------------------------------------------------

Description:
Header file for user_app1.c

**********************************************************************************************************************/

#ifndef __USER_APP1_H
#define __USER_APP1_H
#include <intrinsics.h>
#define ANT_CHANNEL_TYPE_USERAPP        CHANNEL_TYPE_MASTER   // Device is a MASTER
#define ANT_CHANNEL_USERAPP             ANT_CHANNEL_0         // Channel 0 - 7
#define ANT_DEVICEID_LO_USERAPP         (u8)0x22              // Low byte of two-byte Device #
#define ANT_DEVICEID_HI_USERAPP         (u8)0x13              // High byte of two-byte Device #
#define ANT_DEVICE_TYPE_USERAPP         (u8)1                 // 1 - 255
#define ANT_TRANSMISSION_TYPE_USERAPP   (u8)1                 // 1-127 (MSB is pairing bit)
#define ANT_CHANNEL_PERIOD_LO_USERAPP   (u8)0x00              // Low byte of two-byte channel period 0x0001 - 0x7fff
#define ANT_CHANNEL_PERIOD_HI_USERAPP   (u8)0x20              // High byte of two-byte channel period 
#define ANT_FREQUENCY_USERAPP           (u8)55                // 2400MHz + this number 0 - 99
#define ANT_TX_POWER_USERAPP            RADIO_TX_POWER_4DBM   // RADIO_TX_POWER_0DBM, RADIO_TX_POWER_MINUS5DBM, RADIO_TX_POWER_MINUS10DBM,


#define ANT_CONFIGURE_TIMEOUT_MS  3000

#define Found_you            (u8)2
#define Be_found             (u8)2

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/


/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/


/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/
void UserApp1Initialize(void);
void UserApp1RunActiveState(void);

static void UserApp1SM_AntConfigureMaster(void);
static void UserApp1SM_AntConfigureSlave1(void);
static void UserApp1SM_AntConfigureSlave2(void);

static void UserApp1SM_OpenChannel(void);
static void UserApp1SM_RadioActive_master_initialize(void);
static void UserApp1SM_RadioActive_slave_initialize(void);

static void UserApp1SM_RadioActive_master(void);
static void UserApp1SM_RadioActive_slave(void);

/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void UserApp1SM_Idle(void);    

static void UserApp1SM_Error(void);         
static void AllLedOff(void);
static void AllLedBlink(void);
static void RssiDBM(s8 s8Data);

#endif /* __USER_APP1_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/

