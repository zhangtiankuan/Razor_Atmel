/**********************************************************************************************************************
File: user_app1.c                                                                

Description:
Provides a Tera-Term driven system to display, read and write an LED command list.

Test1.
Test2 from Engenuics.
Test3.

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:
None.

Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

extern u8 G_au8DebugScanfBuffer[DEBUG_SCANF_BUFFER_SIZE]; /* From debug.c */
extern u8 G_u8DebugScanfCharCount;                        /* From debug.c */


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
  u8 u8Message_1[]="Press 1 to program LED command sequence";
  u8 u8Message_2[]="Press 2 to show current USER program";
  DebugPrintf(u8Message_1);
  DebugLineFeed();
  DebugPrintf(u8Message_2);
  DebugLineFeed();  
  //u8 au8UserApp1Start1[] = "LED program task started\n\r";
  
  /* Turn off the Debug task command processor and announce the task is ready */
  //DebugSetPassthrough();
  //DebugPrintf(au8UserApp1Start1);
    /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_FailedInit;
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
/* Wait for input */
static void UserApp1SM_Idle(void)
{
  extern u8 G_au8DebugScanfBuffer[];
  extern u8 G_u8DebugScanfCharCount;
  u8 u8Message_1[]="Press 1 to program LED command sequence";
  u8 u8Message_2[]="Press 2 to show current USER program";
  static u32 u32ontime = 0;
  static u32 u32offtime = 0;
  static u8 u8index1,u8index2,u8index3,u8index4,u8index5 = 0;
  static u8 u8color = 0;
  static u8 u8state = 0;
  static u8 u8digit1,u8digit2,u8position = 0;
  static u8 u8counter = 0;
  static u8 u8counter1 = 0;
  static u8 u8counter2 = 1;  
  static u8 u8Message1[100];
  static u16 u16Message2[1000];
  static u16 u16Message3[1000];
  static bool bflag1,bflag2,bflag3,bflag4,bflag5=FALSE;
  static bool bstate=TRUE; 
  static LedCommandType ledshow1;
  static LedCommandType ledshow2;
  static u8 u8ColorMessage1[]={'W','P','B','C','G','Y','O','R'};
  static u8 u8ColorMessage2[]={WHITE,PURPLE,BLUE,CYAN,GREEN,YELLOW,ORANGE,RED};
  
  if(G_u8DebugScanfCharCount!=0)
  {
    DebugScanf(u8Message1);
    u16Message2[u8index1]=u8Message1[0];
    u8index1++;

    if(u16Message2[0]=='1')//input commands
    {
      if(bstate)
      {
        DebugLineFeed();
        DebugLineFeed(); 
        DebugPrintf("Enter commands as LED-ONTIME-OFFTIME and press Enter");
        DebugLineFeed(); 
        DebugPrintf("Time is in milliseconds, max 100 commands");
        DebugLineFeed(); 
        DebugPrintf("LED colors:R, O, Y, G, C, B, P, W");
        DebugLineFeed();
        DebugPrintf("Example: R-100-200 (Red on at 100ms and off at 200ms)");
        DebugLineFeed();
        DebugPrintf("Press Enter on blank line to end");
        DebugLineFeed();
        DebugPrintNumber(u8counter2);
        DebugPrintf(":");
        bstate=FALSE;
      }
    }
    
    if(u16Message2[0]=='2')//current USER program
    {
        DebugLineFeed();
        DebugLineFeed();
        DebugPrintf("Current USER program");
        DebugLineFeed();
        DebugPrintf("LED  ON TIME  OFF TIME");
        DebugLineFeed();
        DebugPrintf("-----------------------");
        DebugLineFeed();
        u8state=2;
          
        for(u8index2=0;u8index2<u8counter;u8index2++)
        {
          LedDisplayPrintListLine(u8index2);
        }
        
        for(u8index2=0;u8index2<u8index5;u8index2++)
        {
          u16Message2[u8index2]='0';
        }
    }
    
    if(u8state==2)
    {
        if(u16Message2[1]==0x0d)//IF USER press 'enter',they will return to the main menu
        {
          u8index1=0;
          u8state=0;
          DebugPrintf("return to the main menu");
          DebugLineFeed();
          DebugPrintf(u8Message_1);
          DebugLineFeed();
          DebugPrintf(u8Message_2);
          DebugLineFeed();
        }
    }

    if(u16Message2[u8index1-1]==0x0d)//determine whether the command is fully entered
    {
          u8state=1;
          DebugLineFeed();
    }
 
    if(u8state==1)//determine if the command is correct
    {
        for(u8index2=0;u8index2<8;u8index2++)
        {
          if(u8ColorMessage1[u8index2]==u16Message2[1])
          {
            bflag1=TRUE;
            u8color=u8index2;//record the color of LED
          }
        }
        
        if(bflag1)
        {
          if(u16Message2[2]=='-')
          {
            bflag2=TRUE;
          }
        }
        
        if(bflag2)
        {
          for(u8index2=3;u8index2<u8index1;u8index2++)
          {
            if(u16Message2[u8index2]=='-')
            {
              u8position=u8index2;//record the second'-' position
              u8digit1=u8position-3;//record the first time's digit
            }
          }

          for(u8index2=3;u8index2<u8position;u8index2++)
          {
            if((u16Message2[u8index2]<=57)&&(u16Message2[u8index2]>=48))
            {
              u8counter1++;
            }
            if(u8counter1==u8digit1)
            {
              u8counter1=0;
              bflag3=TRUE;
            }
          }
        }
        
        if(bflag3)
        {
          u8digit2=u8index1-u8position-2;//record the second time's digit
          for(u8index2=u8position+1;u8index2<u8index1-1;u8index2++)
          {
            if((u16Message2[u8index2]<=57)&&(u16Message2[u8index2]>=48))
            {
              u8counter1++;
            }
            if(u8counter1==u8digit2)
            {
              u8counter1=0;
              bflag4=TRUE;
            }
          }
        }
        
        if(bflag4)
        {
          for(u8index2=3;u8index2<u8position;u8index2++)
          {
            u16Message3[u8index2]=u16Message2[u8index2]-48;
            for(u8index3=u8digit1-1;u8index3>0;u8index3--)
            {
              u16Message3[u8index2]=u16Message3[u8index2]*10;
            }
            u32ontime=u32ontime+u16Message3[u8index2];
            u8digit1--;
          }
          
          for(u8index2=u8position+1;u8index2<u8index1-1;u8index2++)
          {
            u16Message3[u8index2]=u16Message2[u8index2]-48;
            for(u8index3=u8digit2-1;u8index3>0;u8index3--)
            {
              u16Message3[u8index2]=u16Message3[u8index2]*10;
            }
            u32offtime=u32offtime+u16Message3[u8index2];
            u8digit2--;
          }
          
          if(u32offtime>u32ontime)
          {
            bflag5=TRUE;
          }
        }
        
        if(bflag5)//the command is correct
        {
          ledshow1.eLED=u8ColorMessage2[u8color];
          ledshow1.u32Time=u32ontime;
          ledshow1.bOn=TRUE;
          ledshow1.eCurrentRate=0;
          
          ledshow2.eLED=u8ColorMessage2[u8color];
          ledshow2.u32Time=u32offtime;
          ledshow2.bOn=FALSE;
          ledshow2.eCurrentRate=LED_PWM_100;
        
          u8counter2++;
          DebugPrintNumber(u8counter2);
          DebugPrintf(":");
          LedDisplayAddCommand(USER_LIST,&ledshow1);
          LedDisplayAddCommand(USER_LIST,&ledshow2);
          u8index5=u8index1;
          u8counter++;
        }
        
        else//the command is wrong
        {
          DebugPrintf("Invalid command: incorrect format. Please use L-ONTIME-OFFTIME");
          DebugLineFeed();
          DebugLineFeed();
          DebugPrintNumber(u8counter2);
          DebugPrintf(":");
          
        }
        u8index4=u8index1;
        u32ontime=0;
        u32offtime=0;
        u8index1=1;
        u8state=0;
        bflag1,bflag2,bflag3,bflag4,bflag5=FALSE;
        
        if(u16Message2[1]==0x0d)//If the user does not have an input command,press 'enter' to enter the main menu 
        { 
          for(u8index2=0;u8index2<u8index4;u8index2++)
          {
            u16Message2[u8index2]='0';
          }
          bstate=TRUE;
          u8index1=0;
          DebugLineFeed(); 
          DebugPrintf("return to the main menu");
          DebugLineFeed();
          DebugLineFeed();
          DebugPrintf(u8Message_1);
          DebugLineFeed();
          DebugPrintf(u8Message_2);
          DebugLineFeed();
          DebugLineFeed();
        }
    }
  }
  
} /* end UserApp1SM_Idle() */
                      
            
#if 0
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */
#endif


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserApp1SM_FailedInit(void)          
{
    
} /* end UserApp1SM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
