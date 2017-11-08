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
  static bool bflag=TRUE;
  static bool bstatus1=FALSE;
  static bool bstatus2=FALSE;
  static bool bstatus3=FALSE;
  static bool bstatus4=FALSE;
  static bool bscale1=FALSE;
  static bool bscale2=FALSE;
  static bool bscale3=FALSE;
  static bool bscale4=FALSE;
  static u8 u8NumCharsMessage[100];
  static u32 u32counter = 0;
  static u16 u16counter = 0;
  //************judge which button is pressed
  if(bflag)
  {
    if(IsButtonPressed(BUTTON0))//open first passageway;close other scale
    {
      bstatus1=TRUE;
      bstatus2=FALSE;
      bscale2=FALSE;
      bstatus3=FALSE;
      bscale3=FALSE;
      bstatus4=FALSE;
      bscale4=FALSE;
      bflag=FALSE;
    }
    if(IsButtonPressed(BUTTON1))//open second passageway;close other scale
    {
      bstatus2=TRUE;
      bstatus1=FALSE;
      bscale1=FALSE;
      bstatus3=FALSE;
      bscale3=FALSE;
      bstatus4=FALSE;
      bscale4=FALSE;
      bflag=FALSE;
    }
    if(IsButtonPressed(BUTTON2))//open third passageway;close other scale
    {
      bstatus3=TRUE;
      bstatus2=FALSE;
      bscale2=FALSE;
      bstatus1=FALSE;
      bscale1=FALSE;
      bstatus4=FALSE;
      bscale4=FALSE;
      bflag=FALSE;
    }
    if(IsButtonPressed(BUTTON3))//open fourth passageway;close other scale
    {
      bstatus4=TRUE;
      bstatus2=FALSE;
      bscale2=FALSE;
      bstatus3=FALSE;
      bscale3=FALSE;
      bstatus1=FALSE;
      bscale1=FALSE;
      bflag=FALSE;
    }
  }    
 //*********set frequency
  if(bstatus1)
  {
    bscale1=TRUE;
    DebugScanf(u8NumCharsMessage);
    if(u8NumCharsMessage[0]=='1')
    {
      PWMAudioSetFrequency(BUZZER2,C3);
      bstatus1=FALSE;
    }
    if(u8NumCharsMessage[0]=='2')
    {
      PWMAudioSetFrequency(BUZZER2,D3);
      bstatus1=FALSE;
    }
    if(u8NumCharsMessage[0]=='3')
    {
      PWMAudioSetFrequency(BUZZER2,E3);
      bstatus1=FALSE;
    }
    if(u8NumCharsMessage[0]=='4')
    {
      PWMAudioSetFrequency(BUZZER2,F3);
      bstatus1=FALSE;
    }
    if(u8NumCharsMessage[0]=='5')
    {
      PWMAudioSetFrequency(BUZZER2,G3);
      bstatus1=FALSE;
    }
    if(u8NumCharsMessage[0]=='6')
    {
      PWMAudioSetFrequency(BUZZER2,A3);
      bstatus1=FALSE;
    }
    if(u8NumCharsMessage[0]=='7')
    {
      PWMAudioSetFrequency(BUZZER2,B3);
      bstatus1=FALSE;
    }
  }

  if(bstatus2)
  {
    bscale2=TRUE;
    DebugScanf(u8NumCharsMessage);
    if(u8NumCharsMessage[0]=='q')
    {
      PWMAudioSetFrequency(BUZZER2,C4);
      bstatus2=FALSE;
    }
    if(u8NumCharsMessage[0]=='w')
    {
      PWMAudioSetFrequency(BUZZER2,D4);
      bstatus2=FALSE;
    }
    if(u8NumCharsMessage[0]=='e')
    {
      PWMAudioSetFrequency(BUZZER2,E4);
      bstatus2=FALSE;
    }
    if(u8NumCharsMessage[0]=='r')
    {
      PWMAudioSetFrequency(BUZZER2,F4);
      bstatus2=FALSE;
    }
    if(u8NumCharsMessage[0]=='t')
    {
      PWMAudioSetFrequency(BUZZER2,G4);
      bstatus2=FALSE;
    }
    if(u8NumCharsMessage[0]=='y')
    {
      PWMAudioSetFrequency(BUZZER2,A4);
      bstatus2=FALSE;
    }
    if(u8NumCharsMessage[0]=='u')
    {
      PWMAudioSetFrequency(BUZZER2,B4);
      bstatus2=FALSE;
    }
  }

  if(bstatus3)
  {
    bscale3=TRUE;
    DebugScanf(u8NumCharsMessage);
    if(u8NumCharsMessage[0]=='a')
    {
      PWMAudioSetFrequency(BUZZER2,C5);
      bstatus3=FALSE;
    }
    if(u8NumCharsMessage[0]=='s')
    {
      PWMAudioSetFrequency(BUZZER2,D5);
      bstatus3=FALSE;
    }
    if(u8NumCharsMessage[0]=='d')
    {
      PWMAudioSetFrequency(BUZZER2,E5);
      bstatus3=FALSE;
    }
    if(u8NumCharsMessage[0]=='f')
    {
      PWMAudioSetFrequency(BUZZER2,F5);
      bstatus3=FALSE;
    }
    if(u8NumCharsMessage[0]=='g')
    {
      PWMAudioSetFrequency(BUZZER2,G5);
      bstatus3=FALSE;
    }
    if(u8NumCharsMessage[0]=='h')
    {
      PWMAudioSetFrequency(BUZZER2,A5);
      bstatus3=FALSE;
    }
    if(u8NumCharsMessage[0]=='j')
    {
      PWMAudioSetFrequency(BUZZER2,B5);
      bstatus3=FALSE;
    }
  } 

  if(bstatus4)
  {
    bscale4=TRUE;
    DebugScanf(u8NumCharsMessage);
    if(u8NumCharsMessage[0]=='z')
    {
      PWMAudioSetFrequency(BUZZER2,C6);
      bstatus4=FALSE;
    }
    if(u8NumCharsMessage[0]=='x')
    {
      PWMAudioSetFrequency(BUZZER2,D6);
      bstatus4=FALSE;
    }
    if(u8NumCharsMessage[0]=='c')
    {
      PWMAudioSetFrequency(BUZZER2,E6);
      bstatus4=FALSE;
    }
    if(u8NumCharsMessage[0]=='v')
    {
      PWMAudioSetFrequency(BUZZER2,F6);
      bstatus4=FALSE;
    }
    if(u8NumCharsMessage[0]=='b')
    {
      PWMAudioSetFrequency(BUZZER2,G6);
      bstatus4=FALSE;
    }
    if(u8NumCharsMessage[0]=='n')
    {
      PWMAudioSetFrequency(BUZZER2,A6);
      bstatus4=FALSE;
    }
    if(u8NumCharsMessage[0]=='m')
    {
      PWMAudioSetFrequency(BUZZER2,B6);
      bstatus4=FALSE;
    }
  }  
  
 //**************open the buzzer
  
  if(bscale1)
  {
    if((u8NumCharsMessage[0]=='1')||(u8NumCharsMessage[0]=='2')
      ||(u8NumCharsMessage[0]=='3')||(u8NumCharsMessage[0]=='4')
      ||(u8NumCharsMessage[0]=='5')||(u8NumCharsMessage[0]=='6')
      ||(u8NumCharsMessage[0]=='7'))
    {
      PWMAudioOn(BUZZER2);
      for(u32counter=0;u32counter<=50;u32counter++)
      {
       u16counter++;
      }//delay a period of time
      if(u16counter==1275)
      {
       PWMAudioOff(BUZZER2);
       bflag=TRUE;
       bstatus1=TRUE;
       u8NumCharsMessage[0]=0;
       u16counter=0;
      }
    }
  }

  if(bscale2)
  {
    if((u8NumCharsMessage[0]=='q')||(u8NumCharsMessage[0]=='w')
      ||(u8NumCharsMessage[0]=='e')||(u8NumCharsMessage[0]=='r')
      ||(u8NumCharsMessage[0]=='t')||(u8NumCharsMessage[0]=='y')
      ||(u8NumCharsMessage[0]=='u'))
    {
      PWMAudioOn(BUZZER2);
      for(u32counter=0;u32counter<=50;u32counter++)
      {
       u16counter++;
      }//delay a period of time
      if(u16counter==1275)
      {
       PWMAudioOff(BUZZER2);
       bstatus2=TRUE;
       bflag=TRUE;
       u8NumCharsMessage[0]=0;
       u16counter=0;
      }
    }
  }

  if(bscale3)
  {
    if((u8NumCharsMessage[0]=='a')||(u8NumCharsMessage[0]=='s')
      ||(u8NumCharsMessage[0]=='d')||(u8NumCharsMessage[0]=='f')
      ||(u8NumCharsMessage[0]=='g')||(u8NumCharsMessage[0]=='h')
      ||(u8NumCharsMessage[0]=='j'))
    {
      PWMAudioOn(BUZZER2);
      for(u32counter=0;u32counter<=50;u32counter++)
      {
       u16counter++;
      }//delay a period of time
      if(u16counter==1275)
      {
       PWMAudioOff(BUZZER2);
       bstatus3=TRUE;
       bflag=TRUE;
       u8NumCharsMessage[0]=0;
       u16counter=0;
      }
    }
  }  

  if(bscale4)
  {
    if((u8NumCharsMessage[0]=='z')||(u8NumCharsMessage[0]=='x')
      ||(u8NumCharsMessage[0]=='c')||(u8NumCharsMessage[0]=='v')
      ||(u8NumCharsMessage[0]=='b')||(u8NumCharsMessage[0]=='n')
      ||(u8NumCharsMessage[0]=='m'))
    {
      PWMAudioOn(BUZZER2);
      for(u32counter=0;u32counter<=50;u32counter++)
      {
       u16counter++;
      }//delay a period of time
      if(u16counter==1275)
      {
       PWMAudioOff(BUZZER2);
       bstatus4=TRUE;
       bflag=TRUE;
       u8NumCharsMessage[0]=0;
       u16counter=0;
      }
    }
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
