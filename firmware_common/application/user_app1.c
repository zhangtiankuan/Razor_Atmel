/**********************************************************************************************************************
File: user_app1.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.
**********************************************************************************************************************/
#include "configuration.h"
/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */

/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern u32 G_u32AntApiCurrentDataTimeStamp;                       /* From ant_api.c */
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;    /* From ant_api.c */
extern u8 G_au8AntApiCurrentMessageBytes[ANT_APPLICATION_MESSAGE_BYTES];  /* From ant_api.c */
extern AntExtendedDataType G_sAntApiCurrentMessageExtData;        /* From ant_api.c */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */
extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */
/***********************************************************************************************************************

Global variable definitions with scope limited to this local application.

Variable names shall start with "UserApp1_" and be declared as static.

***********************************************************************************************************************/

static u32 UserApp1_u32DataMsgCount = 0;             /* Counts the number of ANT_DATA packets received */
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */
/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR,"heart rate detector");
  LCDMessage(LINE2_START_ADDR,"Press B0 to start");
  AntAssignChannelInfoType sAntSetupData;
  
 /* Configure ANT for this application */
  sAntSetupData.AntChannel          = ANT_CHANNEL_0;
  sAntSetupData.AntChannelType      = ANT_CHANNEL_TYPE_USERAPP;
  sAntSetupData.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
  sAntSetupData.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;
  sAntSetupData.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP;
  sAntSetupData.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP;
  sAntSetupData.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
  sAntSetupData.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
  sAntSetupData.AntFrequency        = ANT_FREQUENCY_USERAPP;
  sAntSetupData.AntTxPower          = ANT_TX_POWER_USERAPP;
  sAntSetupData.AntNetwork          = ANT_NETWORK_DEFAULT;  
  
  sAntSetupData.AntNetworkKey[0] = 0xB9;
  sAntSetupData.AntNetworkKey[1] = 0xA5;
  sAntSetupData.AntNetworkKey[2] = 0x21;
  sAntSetupData.AntNetworkKey[3] = 0xFB;
  sAntSetupData.AntNetworkKey[4] = 0xBD;
  sAntSetupData.AntNetworkKey[5] = 0x72;
  sAntSetupData.AntNetworkKey[6] = 0xC3;
  sAntSetupData.AntNetworkKey[7] = 0x45;
  if( AntAssignChannel(&sAntSetupData) )
  {
    /* Channel assignment is queued so start timer */
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_WaitChannelAssign;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    LedBlink(RED, LED_4HZ);
    UserApp1_StateMachine = UserApp1SM_Error;
  }
} /* end UserApp1Initialize() */

static void UserApp1SM_WaitChannelAssign(void)
{
  /* Check if the channel assignment is complete */
  if(AntRadioStatusChannel(ANT_CHANNEL_0) == ANT_CONFIGURED)
  {
    LedOn(YELLOW);
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  /* Monitor for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout,3000))
  {
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, "Channel config failed");
    LedBlink(RED, LED_4HZ);
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_Error;
  }
} /* end UserApp1SM_WaitChannelAssign() */ 

/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    LedOff(YELLOW); 
    LedOn(GREEN);
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    AntOpenChannelNumber(ANT_CHANNEL_0);
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,"B0:CLOSE");
    LCDMessage(LINE2_START_ADDR,"B1  B2  B3");
    UserApp1_StateMachine = UserApp1SM_OpenChannel;
  }
} /* end UserApp1SM_Idle() */
    
static void UserApp1SM_OpenChannel(void)
{
  if(AntRadioStatusChannel(ANT_CHANNEL_0) == ANT_OPEN)
  {
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_RadioActive;
  }
 
  if( IsTimeUp(&UserApp1_u32Timeout, 3000) )
  {
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, "Channel open failed");
    UserApp1_StateMachine = UserApp1SM_Error;    
  }
}/*end UserApp1SM_OpenChannel()*/


static void UserApp1SM_RadioActive(void)
{
  static u8 u8mode;
  static u8 u8heartrate;
  static u8 u8heart_rate[] = "0";
  static u8 u8heart_rate_LCD[] = "000";
  static u8 u8message1[]={0x46,0xff,0xff,0xff,0xff,0x80,0x05,0x01};
  static u8 u8message2[]={0x46,0xff,0xff,0xff,0xff,0x80,0x07,0x01};
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    AntCloseChannelNumber(ANT_CHANNEL_0);
    AllLedOff();
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_CloseChannel;
  } 
  if(WasButtonPressed(BUTTON1))
  {
    ButtonAcknowledge(BUTTON1);
    u8mode=1;
  }
  if(WasButtonPressed(BUTTON2))
  {
    ButtonAcknowledge(BUTTON2);
    u8mode=2;
    AntQueueAcknowledgedMessage(ANT_CHANNEL_0,u8message1);
  }
  if(WasButtonPressed(BUTTON3))
  {
    ButtonAcknowledge(BUTTON3);
    u8mode=3;
    AntQueueAcknowledgedMessage(ANT_CHANNEL_0,u8message2);
  }
  if( AntReadAppMessageBuffer())
  {
    /* New data message: check what it is */
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
        if(u8mode==1)
        {
          u8heart_rate[1] = G_au8AntApiCurrentMessageBytes[7]%0x10;
          u8heart_rate[0] = G_au8AntApiCurrentMessageBytes[7]/0x10;
          u8heartrate = u8heart_rate[0]*16+u8heart_rate[1];
          u8heart_rate_LCD[2] = u8heartrate%10+48;
          u8heart_rate_LCD[1] = (u8heartrate/10)%10+48;
          u8heart_rate_LCD[0] = u8heartrate/100+48;
          LedBlink(GREEN,LED_4HZ);  
          LCDCommand(LCD_CLEAR_CMD);
          LCDMessage(LINE1_START_ADDR,"Now heartrate:");
          LCDMessage(LINE1_START_ADDR+17,u8heart_rate_LCD);

        }
        
        if(u8mode==2)
        {
          
        }
        
     }

    
  }
}/*end UserApp1SM_RadioActive()*/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */

static void UserApp1SM_CloseChannel(void)
{
  if(AntRadioStatusChannel(ANT_CHANNEL_0) == ANT_CLOSED)
  {
    LedOn(YELLOW);
    UserApp1_StateMachine = UserApp1Initialize;
  }
  /* Check for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout, 3000) )
  {
    LedBlink(RED, LED_4HZ);
    UserApp1_StateMachine = UserApp1SM_Error;
  }
}/*end UserApp1SM_CloseChannel()*/

static void AllLedOff(void)
{
  LedOff(WHITE);
  LedOff(PURPLE);
  LedOff(BLUE);
  LedOff(CYAN);
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOff(ORANGE);
  LedOff(RED);
}/*end AllLedOff() */
/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
