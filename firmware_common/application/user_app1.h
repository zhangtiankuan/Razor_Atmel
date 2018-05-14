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
#define ANT_CHANNEL_TYPE_USERAPP        CHANNEL_TYPE_SLAVE    /* ANT SLAVE */
#define ANT_DEVICEID_LO_USERAPP         (u8)0x22               /* Low byte of two-byte Device # */
#define ANT_DEVICEID_HI_USERAPP         (u8)0x13                /* High byte of two-byte Device # */
#define ANT_DEVICE_TYPE_USERAPP         (u16)120                 /* 1 - 255 */
#define ANT_TRANSMISSION_TYPE_USERAPP   (u8)0x0                /* 1-127 (MSB is pairing bit) */
#define ANT_CHANNEL_PERIOD_LO_USERAPP   (u8)0x86              /* Low byte of two-byte channel period 0x0001 - 0x7fff */
#define ANT_CHANNEL_PERIOD_HI_USERAPP   (u8)0x1F             /* High byte of two-byte channel period */
#define ANT_FREQUENCY_USERAPP           (u8)57                /* 2400MHz + this number 0 - 99 */
#define ANT_TX_POWER_USERAPP            RADIO_TX_POWER_4DBM   /* RADIO_TX_POWER_0DBM, RADIO_TX_POWER_MINUS5DBM, RADIO_TX_POWER_MINUS10DBM, RADIO_TX_POWER_MINUS20DBM */


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


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void UserApp1SM_Idle(void);    
static void UserApp1SM_OpenChannel(void);
static void UserApp1SM_CloseChannel(void);
static void UserApp1SM_RadioActive(void);
static void UserApp1SM_WaitChannelAssign(void);
static void UserApp1SM_Error(void);         
static void AllLedOff(void);

#endif /* __USER_APP1_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
