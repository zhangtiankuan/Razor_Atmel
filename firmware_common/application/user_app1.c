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


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


extern u32 G_u32AntApiCurrentMessageTimeStamp;                            // From ant_api.c
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;            // From ant_api.c
extern u8 G_au8AntApiCurrentMessageBytes[ANT_APPLICATION_MESSAGE_BYTES];  // From ant_api.c
extern AntExtendedDataType G_sAntApiCurrentMessageExtData;                // From ant_api.c
/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */
static u8 au8message[]={0,0,0,0,0,0,0,0};

static AntAssignChannelInfoType UserApp1_sMasterChannel0;
static AntAssignChannelInfoType UserApp1_sSlaveChannel1;
static AntAssignChannelInfoType UserApp1_sSlaveChannel2;
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
  LedOff(RED);
  LedOff(ORANGE);
  LedOff(YELLOW);
  LedOff(GREEN);
  LedOff(CYAN);
  LedOff(BLUE);
  LedOff(PURPLE);
  LedOff(WHITE);
  LCDMessage(LINE1_START_ADDR, "hide and seek");
  /* Master (Channel 0) */
  UserApp1_sMasterChannel0.AntChannel = ANT_CHANNEL_0;
  UserApp1_sMasterChannel0.AntChannelType = CHANNEL_TYPE_MASTER;
  UserApp1_sMasterChannel0.AntDeviceIdHi = 0x13;
  UserApp1_sMasterChannel0.AntDeviceIdLo = 0x22;
  UserApp1_sMasterChannel0.AntDeviceType = 1;
  UserApp1_sMasterChannel0.AntTransmissionType = 1;
  UserApp1_sMasterChannel0.AntFrequency = ANT_FREQUENCY_DEFAULT;
  UserApp1_sMasterChannel0.AntTxPower = ANT_TX_POWER_DEFAULT;
  UserApp1_sMasterChannel0.AntNetwork = ANT_NETWORK_DEFAULT;
  /* Slave (Channel 1) */
  UserApp1_sSlaveChannel1.AntChannel = ANT_CHANNEL_1;
  UserApp1_sSlaveChannel1.AntChannelType = CHANNEL_TYPE_SLAVE;
  UserApp1_sSlaveChannel1.AntDeviceIdHi = 0x13;
  UserApp1_sSlaveChannel1.AntDeviceIdLo = 0x22;
  UserApp1_sSlaveChannel1.AntDeviceType = 1;
  UserApp1_sSlaveChannel1.AntTransmissionType = 1;
  UserApp1_sSlaveChannel1.AntFrequency = ANT_FREQUENCY_DEFAULT;
  UserApp1_sSlaveChannel1.AntTxPower = ANT_TX_POWER_DEFAULT;
  UserApp1_sSlaveChannel1.AntNetwork = ANT_NETWORK_DEFAULT;
  /* Slave (Channel 2) */
  UserApp1_sSlaveChannel2.AntChannel = ANT_CHANNEL_2;
  UserApp1_sSlaveChannel2.AntChannelType = CHANNEL_TYPE_SLAVE;
  UserApp1_sSlaveChannel2.AntDeviceIdHi = 0x13;
  UserApp1_sSlaveChannel2.AntDeviceIdLo = 0x22;
  UserApp1_sSlaveChannel2.AntDeviceType = 1;
  UserApp1_sSlaveChannel2.AntTransmissionType = 1;
  UserApp1_sSlaveChannel2.AntFrequency = ANT_FREQUENCY_DEFAULT;
  UserApp1_sSlaveChannel2.AntTxPower = ANT_TX_POWER_DEFAULT;
  UserApp1_sSlaveChannel2.AntNetwork = ANT_NETWORK_DEFAULT;

  for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
  {
    UserApp1_sMasterChannel0.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
    UserApp1_sSlaveChannel1.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
    UserApp1_sSlaveChannel2.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;    
  }
  
  /* Queue configuration of Master channel */
  AntAssignChannel(&UserApp1_sMasterChannel0);
  UserApp1_u32Timeout = G_u32SystemTime1ms;
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_AntConfigureMaster;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

static void UserApp1SM_AntConfigureMaster(void)
{
  /* Wait for the ANT master channel0 to be configured */
  if(AntRadioStatusChannel(ANT_CHANNEL_0) == ANT_CONFIGURED)
  {
    /* Queue configuration of Slave channel */
    AntAssignChannel(&UserApp1_sSlaveChannel1);
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_AntConfigureSlave1;
  }
  /* Check for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout, ANT_CONFIGURE_TIMEOUT_MS) )
  {
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, "Master config failed");
    UserApp1_StateMachine = UserApp1SM_Error;    
  }
} /* end UserApp1SM_AntConfigureMaster() */  

static void UserApp1SM_AntConfigureSlave1(void)
{
  /* Wait for the ANT slave channel1 to be configured */
  if(AntRadioStatusChannel(ANT_CHANNEL_1) == ANT_CONFIGURED)
  {
    AntAssignChannel(&UserApp1_sSlaveChannel2);
    UserApp1_StateMachine = UserApp1SM_AntConfigureSlave2;
  }
  /* Check for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout, ANT_CONFIGURE_TIMEOUT_MS) )
  {
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, "Slave1 config failed");
    UserApp1_StateMachine = UserApp1SM_Error;    
  }
} /* end UserApp1SM_AntConfigureSlave1() */

static void UserApp1SM_AntConfigureSlave2(void)
{
  /* Wait for the ANT slave channel1 to be configured */
  if(AntRadioStatusChannel(ANT_CHANNEL_2) == ANT_CONFIGURED)
  {
    AntQueueBroadcastMessage(ANT_CHANNEL_0, au8message);
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  /* Check for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout, ANT_CONFIGURE_TIMEOUT_MS) )
  {
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, "Slave12 config failed");
    UserApp1_StateMachine = UserApp1SM_Error;    
  }
} /* end UserApp1SM_AntConfigureSlave1() */
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
    AntOpenChannelNumber(ANT_CHANNEL_0); 
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_OpeningChannels;
  }
} /* end UserApp1SM_Idle() */

static void UserApp1SM_OpeningChannels(void)
{
  if(AntRadioStatusChannel(ANT_CHANNEL_0) == ANT_OPEN)
  {
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, "hide");
    UserApp1_StateMachine = UserApp1SM_RadioActive;    
  }
  /* Check for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout, ANT_CONFIGURE_TIMEOUT_MS) )
  {
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, "Channel open failed");
    UserApp1_StateMachine = UserApp1SM_Error;    
  }
}/* end UserApp1SM_OpeningChannels() */

static void UserApp1SM_RadioActive(void)
{
  
  LedNumberType aeLedDisplayLevels[] = {RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, PURPLE, WHITE};
  s8 as8dBmLevels[] = {DBM_LEVEL1, DBM_LEVEL2, DBM_LEVEL3, DBM_LEVEL4, 
                       DBM_LEVEL5, DBM_LEVEL6, DBM_LEVEL7, DBM_LEVEL8};
  
  if( AntReadAppMessageBuffer())
  {
    if(G_eAntApiCurrentMessageClass == ANT_TICK)
    {
      s8 s8Rssi = G_sAntApiCurrentMessageExtData.s8RSSI;
       for(u8 i = 0;i<8;i++)
       {
         if(s8Rssi>as8dBmLevels[i])
         {
           LedOn(aeLedDisplayLevels[i]);
         }
         else
         {
           LedOff(aeLedDisplayLevels[i]);
         }
       }
    }
  }
}/*end UserApp1SM_RadioActive()*/
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
