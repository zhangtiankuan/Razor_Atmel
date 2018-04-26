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
static u8 au8message[]={0,0,0,0,0xA5,0,0,0};
static bool bfound_master = FALSE;
static bool bfound_slave = FALSE;
static bool bcounter1 = FALSE;
static bool bcounter2 = FALSE;
static bool bmaster_initialize = TRUE;
static bool bslave_initialize = TRUE;
static u32 u32counter1 = 0;
static u32 u32counter2 = 0;
static AntAssignChannelInfoType UserApp1_sMasterChannel0;
static AntAssignChannelInfoType UserApp1_sSlaveChannel1;
static AntAssignChannelInfoType UserApp1_sSlaveChannel2;


LedNumberType aeLedDisplayLevels[] = {RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, PURPLE, WHITE};


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
  bfound_master = FALSE;
  bfound_slave = FALSE;
  bcounter1 = FALSE;
  bcounter2 = FALSE;
  bmaster_initialize = TRUE;
  bslave_initialize = TRUE;
  PWMAudioSetFrequency(BUZZER1, 100);
  AllLedOff();
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR, "hide and seek");
  LCDMessage(LINE2_START_ADDR, "Press B0 to start");
  LedOff(LCD_RED);
  LedOff(LCD_GREEN);
  LedOff(LCD_BLUE);
  /* Master (Channel 0) */
  UserApp1_sMasterChannel0.AntChannel = ANT_CHANNEL_0;
  UserApp1_sMasterChannel0.AntChannelType = CHANNEL_TYPE_MASTER;
  UserApp1_sMasterChannel0.AntDeviceIdHi = ANT_DEVICEID_HI_USERAPP;
  UserApp1_sMasterChannel0.AntDeviceIdLo = ANT_DEVICEID_LO_USERAPP;
  UserApp1_sMasterChannel0.AntChannelPeriodLo = ANT_CHANNEL_PERIOD_LO_USERAPP;
  UserApp1_sMasterChannel0.AntChannelPeriodHi = ANT_CHANNEL_PERIOD_HI_USERAPP;
  
  UserApp1_sMasterChannel0.AntDeviceType = ANT_DEVICE_TYPE_USERAPP;
  UserApp1_sMasterChannel0.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
  UserApp1_sMasterChannel0.AntFrequency = ANT_FREQUENCY_DEFAULT;
  UserApp1_sMasterChannel0.AntTxPower = ANT_TX_POWER_DEFAULT;
  UserApp1_sMasterChannel0.AntNetwork = ANT_NETWORK_DEFAULT;
  /* Slave (Channel 1) */
  UserApp1_sSlaveChannel1.AntChannel = ANT_CHANNEL_1;
  UserApp1_sSlaveChannel1.AntChannelType = CHANNEL_TYPE_SLAVE;
  UserApp1_sSlaveChannel1.AntDeviceIdHi = ANT_DEVICEID_HI_USERAPP;
  UserApp1_sSlaveChannel1.AntDeviceIdLo = ANT_DEVICEID_LO_USERAPP;
  
  UserApp1_sSlaveChannel1.AntDeviceType = ANT_DEVICE_TYPE_USERAPP;
  UserApp1_sSlaveChannel1.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
  UserApp1_sSlaveChannel1.AntFrequency = ANT_FREQUENCY_DEFAULT;
  UserApp1_sSlaveChannel1.AntTxPower = ANT_TX_POWER_DEFAULT;
  UserApp1_sSlaveChannel1.AntNetwork = ANT_NETWORK_DEFAULT;
  /* Slave (Channel 2) */
  UserApp1_sSlaveChannel2.AntChannel = ANT_CHANNEL_2;
  UserApp1_sSlaveChannel2.AntChannelType = CHANNEL_TYPE_SLAVE;
  UserApp1_sSlaveChannel2.AntDeviceIdHi = ANT_DEVICEID_HI_USERAPP;
  UserApp1_sSlaveChannel2.AntDeviceIdLo = ANT_DEVICEID_LO_USERAPP;
  
  UserApp1_sSlaveChannel2.AntDeviceType = ANT_DEVICE_TYPE_USERAPP;
  UserApp1_sSlaveChannel2.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
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
  if( AntAssignChannel(&UserApp1_sMasterChannel0) )
  {
    /* Channel is configured, so change LED to yellow */
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    LedOn(YELLOW);
    UserApp1_StateMachine = UserApp1SM_AntConfigureMaster;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    LedBlink(RED, LED_4HZ);
    UserApp1_StateMachine = UserApp1SM_Error;
  }
  
} /* end UserApp1Initialize() */

static void UserApp1SM_AntConfigureMaster(void)
{
  /* Wait for the ANT master channel0 to be configured */
  if(AntRadioStatusChannel(ANT_CHANNEL_0) == ANT_CONFIGURED)
  {
    /* Queue configuration of Slave channel */
    if( AntAssignChannel(&UserApp1_sSlaveChannel1) )
    {
      /* Channel is configured, so change LED to yellow */
      UserApp1_u32Timeout = G_u32SystemTime1ms;
      LedOff(RED);
      LedOn(YELLOW);
      UserApp1_StateMachine = UserApp1SM_AntConfigureSlave1;
    }
    else
    {
      /* The task isn't properly initialized, so shut it down and don't run */
      LedBlink(RED, LED_4HZ);
      UserApp1_u32Timeout = G_u32SystemTime1ms;
      UserApp1_StateMachine = UserApp1SM_Error;
    }

  }
  /* Check for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout, ANT_CONFIGURE_TIMEOUT_MS) )
  {
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, "Master config failed");
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_Error;    
  }
} /* end UserApp1SM_AntConfigureMaster() */  

static void UserApp1SM_AntConfigureSlave1(void)
{
  /* Wait for the ANT slave channel1 to be configured */
  if(AntRadioStatusChannel(ANT_CHANNEL_1) == ANT_CONFIGURED)
  {
    /* Queue configuration of Slave channel */
    if( AntAssignChannel(&UserApp1_sSlaveChannel2) )
    {
      /* Channel is configured, so change LED to yellow */
      UserApp1_u32Timeout = G_u32SystemTime1ms;
      LedOff(RED);
      LedOn(YELLOW);
      UserApp1_StateMachine = UserApp1SM_AntConfigureSlave2;
    }
    else
    {
      /* The task isn't properly initialized, so shut it down and don't run */
      LedBlink(RED, LED_4HZ);
      UserApp1_StateMachine = UserApp1SM_Error;
    }

  }
  /* Check for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout, ANT_CONFIGURE_TIMEOUT_MS) )
  {
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, "Slave1 config failed");
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_Error;    
  }
} /* end UserApp1SM_AntConfigureSlave1() */

static void UserApp1SM_AntConfigureSlave2(void)
{
  /* Wait for the ANT slave channel1 to be configured */
  if(AntRadioStatusChannel(ANT_CHANNEL_2) == ANT_CONFIGURED)
  {
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  /* Check for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout, ANT_CONFIGURE_TIMEOUT_MS) )
  {
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, "Slave12 config failed");
    UserApp1_u32Timeout = G_u32SystemTime1ms;
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
    AllLedOff();
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, "hide");
    ButtonAcknowledge(BUTTON0);
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_OpenChannel;
  }
} /* end UserApp1SM_Idle() */

static void UserApp1SM_OpenChannel(void)
{
  AntOpenChannelNumber(ANT_CHANNEL_0);
  AntOpenChannelNumber(ANT_CHANNEL_1);
  AntOpenChannelNumber(ANT_CHANNEL_2);
  if((AntRadioStatusChannel(ANT_CHANNEL_0) == ANT_OPEN)&&
     (AntRadioStatusChannel(ANT_CHANNEL_1) == ANT_OPEN)&&
     (AntRadioStatusChannel(ANT_CHANNEL_2) == ANT_OPEN))
    
  {
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_RadioActive_master_initialize;    
  }
  /* Check for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout, ANT_CONFIGURE_TIMEOUT_MS) )
  {
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, "Channel open failed");
    UserApp1_StateMachine = UserApp1SM_Error;    
  }
}/* end UserApp1SM_OpenChannel() */

static void UserApp1SM_RadioActive_master_initialize(void)
{
  if(bmaster_initialize)
  {
    AllLedOff();
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, "hide");
    LedOff(LCD_BLUE);
    LedOn(LCD_GREEN);
    bmaster_initialize = FALSE;
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_RadioActive_master;
  }
}/*end UserApp1SM_RadioActive_master_initialize*/

static void UserApp1SM_RadioActive_slave_initialize(void)
{
  if(bslave_initialize)
  {
    AllLedOff();
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,"seek");
    LedOn(LCD_BLUE);
    LedOff(LCD_GREEN);
    bslave_initialize = FALSE;
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_RadioActive_slave;
  }
}/*end UserApp1SM_RadioActive_slave_initialize()*/



static void UserApp1SM_RadioActive_master(void)
{
  if( AntReadAppMessageBuffer())
  {
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
        if(G_au8AntApiCurrentMessageBytes[0]==Be_found)
        {
          bfound_master = TRUE;
          bcounter1 = TRUE;
        }
    }
    
    if(bfound_master)
    {
      bfound_master = FALSE;
      bmaster_initialize = FALSE;
      
      AllLedOff();
      LedBlink(WHITE,LED_1HZ);
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(LINE1_START_ADDR,"You found me!");
      
    }
    if(bcounter1)
    {
      u32counter1++;
    }
    if(u32counter1==10)//delay some time
    {
      bcounter1 = FALSE;
      u32counter1 = 0;
      bslave_initialize = TRUE;
      UserApp1_u32Timeout = G_u32SystemTime1ms;
      UserApp1_StateMachine = UserApp1SM_RadioActive_slave_initialize;
    }
  }
}/*end UserApp1SM_RadioActive_master()*/



static void UserApp1SM_RadioActive_slave(void)
{
  if( AntReadAppMessageBuffer())
  {
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      s8 s8Rssi=G_sAntApiCurrentMessageExtData.s8RSSI;
      RssiDBM(s8Rssi);
      for(u8 i=0;i<8;i++)
      {
        if(s8Rssi>=(-45-i*5))
        {
            LedOn(aeLedDisplayLevels[i]);
        }
      }
      
      for(u8 i=0;i<=7;i++)
      {
        if(s8Rssi<=(-45-i*5))
        {
            LedOff(aeLedDisplayLevels[i]);
        }
      }
      
      if(s8Rssi>=-51)
      {

        bfound_slave = TRUE;
        bcounter2 = TRUE;
      }
    }
    
    if(bfound_slave)
    {
      bfound_slave = FALSE;

      AllLedOff();
      AllLedBlink();
      au8message[0] = Found_you;
      PWMAudioOn(BUZZER1);
      AntQueueAcknowledgedMessage(ANT_CHANNEL_1,au8message);
      AntQueueAcknowledgedMessage(ANT_CHANNEL_2,au8message);
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(LINE1_START_ADDR,"Found you!" );  
    }
    if(bcounter2)
    {
      u32counter2++;
    }
    if(u32counter2==20)//delay some time
    {
      bcounter2 = FALSE;
      u32counter2 = 0;
      bmaster_initialize = TRUE;
      UserApp1_u32Timeout = G_u32SystemTime1ms;
      PWMAudioOff(BUZZER1);
      UserApp1_StateMachine = UserApp1SM_RadioActive_master_initialize;
    }

    if(WasButtonPressed(BUTTON3))
    {
      ButtonAcknowledge(BUTTON3);
      UserApp1_u32Timeout = G_u32SystemTime1ms;
      UserApp1_StateMachine = UserApp1Initialize;
    }
    
  }
}/*end UserApp1SM_RadioActive_slave*/



/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{

} /* end UserApp1SM_Error() */

/***close all led****/
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
}/*end AllLedOff */

static void AllLedBlink(void)
{
    LedBlink(WHITE,LED_4HZ);
    LedBlink(PURPLE,LED_4HZ);
    LedBlink(BLUE,LED_4HZ);
    LedBlink(CYAN,LED_4HZ);
    LedBlink(GREEN,LED_4HZ);
    LedBlink(YELLOW,LED_4HZ);
    LedBlink(ORANGE,LED_4HZ);
    LedBlink(RED,LED_4HZ);
}/*end AllLedBlink()*/

static void RssiDBM(s8 s8Data)
{
  u8 u8MESSAGE[]={'-',1,1,'d','B','m'};
  u8 temp=abs(s8Data);
  u8MESSAGE[1]=temp/10+48;
  u8MESSAGE[2]=temp%10+48;
  LCDMessage(LINE2_START_ADDR,u8MESSAGE);
}/*end RssiDBM*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
