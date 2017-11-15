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


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */


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
   LedOff(WHITE);
   LedOff(PURPLE);
   LedOff(BLUE);  
   LedOff(RED);
   LCDCommand(LCD_CLEAR_CMD);
   LCDMessage(LINE1_START_ADDR,"Please Press:");
   LCDMessage(LINE2_START_ADDR,"B0   B1   B2");
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
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
  extern u8 G_au8DebugScanfBuffer[];
  extern u8 G_u8DebugScanfCharCount;
  static u8 u8mode = 0; 
  static u8 u8counter1 = 0; 
  static u8 u8counter2 = 4;
  static u8 u16Message1[100];
  static u8 u16Message2[100];
  static u8 u16Message3[100];
  static u8 u8index1 = 0;
  static u8 u8index2 = 0;
  static u8 u8index3 = 0;  
  static bool bflag1=TRUE;
  static bool bflag2=TRUE; 
  static bool bflag3=TRUE; 
  static u8 UserApp_CursorPosition;
  
  //***************first mode---name LCD
  if(WasButtonPressed(BUTTON0)&(bflag1==TRUE))
  {
    ButtonAcknowledge(BUTTON0);
    u8mode=1;
    LCDCommand(LCD_CLEAR_CMD);
    UserApp_CursorPosition = LINE1_START_ADDR+3;
    LCDMessage(LINE1_START_ADDR+3,"Zhang tiankuan");
    bflag1=FALSE;
    bflag2=FALSE;
    
  }
  
  if(u8mode==1)
  {
    if(WasButtonPressed(BUTTON0)&(bflag1==FALSE))
    {
      u8counter1++;
      u8counter2=4;
      ButtonAcknowledge(BUTTON0);
      if(UserApp_CursorPosition == LINE1_END_ADDR)
      {
        UserApp_CursorPosition = LINE2_START_ADDR; 
      } 
      if(UserApp_CursorPosition+13 == LINE2_END_ADDR)
      {
        UserApp_CursorPosition=LINE2_END_ADDR-13;
      }
      else
      {
        UserApp_CursorPosition++;
      }
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(UserApp_CursorPosition,"Zhang tiankuan");
      LCDMessage(UserApp_CursorPosition+20,"Zhang tiankuan");
    }
    
    if(WasButtonPressed(BUTTON1)&(bflag2==FALSE))
    {
      u8counter2--;
      u8counter1=0;
      ButtonAcknowledge(BUTTON1);
      if(UserApp_CursorPosition == LINE2_START_ADDR)
      {
        UserApp_CursorPosition = LINE1_END_ADDR;
      } 
      if(UserApp_CursorPosition==LINE1_START_ADDR)
      {
        UserApp_CursorPosition=LINE1_START_ADDR;
      }
      else
      {
        UserApp_CursorPosition--;
      }
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(UserApp_CursorPosition,"Zhang tiankuan");
      LCDMessage(UserApp_CursorPosition+20,"Zhang tiankuan");
    }
    
    if((UserApp_CursorPosition==LINE1_START_ADDR)||(UserApp_CursorPosition+13==LINE2_END_ADDR))
    {
       u8counter2=4;
       u8counter1=0;
       LedOn(RED);
       LedOff(WHITE);
       LedOff(PURPLE);
       LedOff(BLUE);
       PWMAudioSetFrequency(BUZZER2,500);
       PWMAudioOn(BUZZER2);
    }
    
    else
    {
      PWMAudioOff(BUZZER2);
    }
    
    if((u8counter1==1)||(u8counter2==1))
    {
      LedOn(WHITE);
      LedOff(PURPLE);
      LedOff(BLUE); 
      LedOff(RED);
      PWMAudioSetFrequency(BUZZER2,C3);
      PWMAudioOn(BUZZER2);
    }
    
    if((u8counter1==2)||(u8counter2==2))
    {
      LedOff(WHITE);
      LedOn(PURPLE);
      LedOff(BLUE);
      LedOff(RED);
      PWMAudioSetFrequency(BUZZER2,D3);
      PWMAudioOn(BUZZER2);
      
    }
    
    if((u8counter1==3)||(u8counter2==3))
    {
      LedOff(WHITE);
      LedOff(PURPLE);
      LedOn(BLUE);  
      LedOff(RED);
      PWMAudioSetFrequency(BUZZER2,E3);
      PWMAudioOn(BUZZER2);
    }
    
    if(u8counter1==4)
    {
      u8counter1=1;
    }
    
    if(u8counter2==0)
    {
      u8counter2=3;
    }
  }
  
  //**************second mode---debug LCD
  if(WasButtonPressed(BUTTON1)&(bflag2==TRUE))
  {
    ButtonAcknowledge(BUTTON1);
    LCDCommand(LCD_CLEAR_CMD);    
    UserApp_CursorPosition=LINE1_START_ADDR;
    u8mode=2;
    bflag2=FALSE;
    bflag1=FALSE;
  }
  
  if(u8mode==2)
  {
    if(G_u8DebugScanfCharCount!=0)
    {
      DebugScanf(u16Message1);
      u16Message2[u8index1]=u16Message1[0];
      u8index1++;
      
      if(UserApp_CursorPosition!=LINE2_END_ADDR+1)
      {
        LCDMessage(UserApp_CursorPosition,u16Message1);
        LCDMessage(UserApp_CursorPosition+20,u16Message1);
        UserApp_CursorPosition++;
      }
      
      else if((bflag3==TRUE)&(UserApp_CursorPosition==LINE2_END_ADDR+1))
      {
        u8index3=0;
        for(u8index2=u8index1-40;u8index2<=u8index1-1;u8index2++)
        {
          u16Message3[u8index3++]=u16Message2[u8index2];
        }
        
        LCDMessage(LINE1_START_ADDR,u16Message3);
        LCDMessage(LINE1_START_ADDR+20,u16Message3);
      }
      
      else if((bflag3==FALSE)&(UserApp_CursorPosition==LINE2_END_ADDR+1))
      {
        u8index3=0;
        for(u8index2=u8index1-20;u8index2<=u8index1-1;u8index2++)
        {
          u16Message3[u8index3++]=u16Message2[u8index2];
        }
        
        LCDMessage(LINE2_START_ADDR,u16Message3);
      }
      
      if(u16Message1[0]==0x0d)
      {
        UserApp_CursorPosition=LINE2_START_ADDR;
        bflag3=FALSE;
      }
      
      if(UserApp_CursorPosition == LINE1_END_ADDR)
      {
        UserApp_CursorPosition = LINE2_START_ADDR; 
      } 
      
    }
  }
  //***********third mode---return to main menu
  if(WasButtonPressed(BUTTON2))
  {
    ButtonAcknowledge(BUTTON2);
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,"Please Press:");
    LCDMessage(LINE2_START_ADDR,"B0   B1   B2");
    u8mode=0;
    bflag1=TRUE;
    bflag2=TRUE; 
    bflag3=TRUE;
    LedOff(WHITE);
    LedOff(PURPLE);
    LedOff(BLUE); 
    LedOff(RED); 
    PWMAudioOff(BUZZER2);
  }  
} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
