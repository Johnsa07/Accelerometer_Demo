/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *    History :
 *    1. Date        : 19, July 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the IAR-STM32F107VC-SK board. It shows basic
 * use of parallel I/O, timer, interrupt controller, interface to
 * a Nokia Graphic color LCD module, step motor and 3D accelerometer sensor.
 *  It try to lock position motor's rotor pointer regardless of the board
 * position.
 *
 *  Jumpers:
 *   PWR_SEL - depending of power source
 *   JTAG_SEL  - open (on board j-link)
 *   B0_1/B0_0 - B0_0
 *   B1_1/B1_0 - B1_0
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "includes.h"

#define DLY_100US  450

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

Int32U CriticalSecCntr;
char countX, countY;
Int16S X, Y, Z, XVel=0, YVel=0, ZVel=0 , XPos=0, YPos=0, ZPos=0;
long velX[2], velY[2], accX[2], accY[2], posX[2], posY[2];
Int32S DegShow = 0, DegShow_h = 1, Delta, DegShowX=0, A,B,C;
Flo32 Deg, DegX, XGrav, YGrav;
Flo32 MotorDeg = 0.0;
Boolean NewData = FALSE;
Boolean Dir;
Int32U Steps;

volatile Boolean SysTickFl= TRUE;

/*************************************************************************
 * Function Name: TickHandler
 * Parameters: void
 * Return: void
 *
 * Description:
 *		
 *************************************************************************/
void TickHandler(void)
{
  SysTickFl = TRUE;
}

/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: Int32U Dly
 *
 * Return: none
 *
 * Description: Delay ~ (arg * 100us)
 *
 *************************************************************************/
void DelayResolution100us(Int32U Dly)
{
  for(; Dly; Dly--)
  {
    for(volatile Int32U j = DLY_100US; j; j--)
    {
    }
  }
}

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: main
 *
 *************************************************************************/
void position(void);
void move_end_check(void);
void main(void)
{

  ENTR_CRT_SECTION();
  /* Setup STM32 system (clock, PLL and Flash configuration) */
  SystemInit();

  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  // SysTick end of count event each 0.5s with input clock equal to 9MHz (HCLK/8, default)
  SysTick_Config(9500000);
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);

  // Step motor init
  StepMotorInit();
  // I2C1 init
  I2C1_Init();

  EXT_CRT_SECTION();

  // GLCD init
  GLCD_PowerUpInit((pInt8U)IAR_Logo.pPicStream); //(
  GLCD_Backlight(BACKLIGHT_ON);
  GLCD_SetFont(&Terminal_9_12_6,0x000F00,0x00FF0);
  GLCD_SetWindow(10,104,131,131);

  // Init Accl sensor
  if(FALSE == Accl_Init())
  {
    // Initialization fault
    GLCD_TextSetPos(0,0);
    GLCD_print("\fLIS3LV020 Init.\r\nfault\r\n");
    while(1);
  }
  while(1)
  {
    if(Accl_Get(&X,&Y,&Z))
    {
      
      YPos=YPos+YVel;
      YVel=YVel+Y;
      
      ZPos=ZPos+ZVel;
      ZVel=ZVel+Z;
      // normalization and calculate angle between the board and the horizontal plain
      if(Y > 1023)
      {
        Y = 1023;
      }
      else if (Y < -1023)
      {
        Y = -1023;
      }
      Deg = (Flo32)Y/1023.0;
      Deg = asin(Deg);
      if (Deg >= 0)
      {
        if (Z < 0)
        {
          Deg = 3.14 - Deg;
        }
      }
      else
      {
        if (Z < 0)
        {
          Deg = -3.14 - Deg;
        }
      }
      DegShow = (Int32S)((Deg*180.0)/(3.14));
      YGrav=sin(Deg)*961;
      YVel=YVel-(Int16S)YGrav;
      
      if(X > 1023)
      {
        X = 1023;
      }
      else if (X < -1023)
      {
        X = -1023;
      }
      DegX = (Flo32)X/1023.0;
      DegX = asin(DegX);
      if (DegX >= 0)
      {
        if (Z < 0)
        {
          DegX = 3.14 - DegX;
        }
      }
      else
      {
        if (Z < 0)
        {
          DegX = -3.14 - DegX;
        }
      }
      DegShowX=(Int32S)((DegX*180.0)/(3.14));
      XGrav=sin(DegX)*1025;
      XPos=XPos+XVel;
      B=XPos;
      if(0<X && X<50)
      {
      }
      else
      {
        XVel=XVel+X-28;
      }
      
      A=(Int32S)XGrav;
      //B=(Int32S)DegX;
      C=XVel;
      position();
      A=accX[1];
      B=velX[1];
      C=posX[1];
      if(TRUE)//DegShow != DegShow_h
      {
        DegShow_h = DegShow;
        NewData = TRUE;
        if(SysTickFl) //
        {
          SysTickFl = FALSE;
          //GLCD_TextSetPos(0,0);
          //GLCD_print("\fPosition :");
          //GLCD_TextSetPos(0,0);
          //GLCD_print("\f%d,%d,%d,%d Deg\r\n",XPos,YPos,ZPos, DegShow);
          //GLCD_TextSetPos(2,0);
          //GLCD_print("\fVelocity :");
          GLCD_TextSetPos(0,0);
          GLCD_print("\f%d, %d, %d, %d Deg\r\n",A, B, C, X);
        }
      }
      else if (StepMotorDone())
      {
        // reduce power consumption
        StepMotorStop();
      }
    }

    if (   StepMotorDone()
        && NewData)
    {
      // Apply compensation
      NewData = FALSE;
      Delta = (Int32S)MotorDeg;
      Delta = DegShow - Delta;
      if(Dir = Delta < 0)
      {
        Delta = -Delta;
      }
      if(Delta > 180)
      {
        Dir = !Dir;
        do
        {
          Delta = 360 - Delta;
          if (Delta < 0)
          {
            Delta = -Delta;
          }
        }
        while(Delta >= 360);
      }

      Steps = StepMotorDeg2Steps ((Flo32)Delta);
      if(Steps)
      {
        StepMotorSet(1000,!Dir);
        StepMotorRun(Steps);
        if(Dir)
        {
          MotorDeg -= StepMotorSteps2Deg(Steps);
        }
        else
        {
          MotorDeg += StepMotorSteps2Deg(Steps);
        }
        if(MotorDeg > 180.0)
        {
          MotorDeg = MotorDeg - 360;
        }
        else if(MotorDeg < -180.0)
        {
          MotorDeg = MotorDeg + 360;
        }
      }
    }
  }
}

void position(void)
{
  unsigned int count1;
  count1 = 0;
  accX[1]=0;
  accY[1]=0;
  do
  {
    Accl_Get(&X,&Y,&Z);
    accX[1]=accX[1]+X;
    accY[1]=accY[1]+Y;
    count1++;
  }while (count1!=64); // 64 sums of the acceleration sample
    
    accX[1]=accX[1]>>6; //Div by 64
    accY[1]=accY[1]>>6;
    
    accX[1]=accX[1]-22; //Remove the offset due to gravity
    accY[1]=accY[1]-6;
    
    if ((accX[1] <=6)&&(accX[1] >= -6)) //Discrimination window applied
      {accX[1] = 0;} // to the X axis acceleration
      //variable

    if ((accY[1] <=3)&&(accY[1] >= -3))
      {accY[1] = 0;} 
    
    //First integration:
    velX[1]=velX[0] + accX[0] + ((accX[1]-accX[0])>>1);
    velY[1]=velY[0] + accY[0] + ((accY[1]-accY[0])>>1);
    
    //Second integration:
    posX[1]=posX[0] + velX[0] + ((velX[1]-velX[0])>>1);
    posY[1]=posY[0] + velY[0] + ((velY[1] - velY[0])>>1);
    
    //Current values sent to previous values
    accX[0]=accX[1];
    accY[0]=accY[1];
    velX[0]=velX[1];
    velY[0]=velY[1];
    
    //posX[1]=posX[1]>>18; //Sensibility adjustment
    posY[1]=posY[1]>>18;
    
    //data_tranfer();
    
    //posX[1]=posX[1]<<18; //Return original value
    posY[1]=posY[1]<<18;    
    
    move_end_check();
    
    posX[0] = posX[1];
    posY[0] = posY[1];

}

void move_end_check(void)
{
  if (accX[1]==0) //Count number of acceleration samples equal to zero
  {countX++;}
  else {countX=0;}
  
  if (countX >= 25) //if this number exceeds 25, we assume that veocity i zero
  {
    velX[1]=0;
    velX[0]=0;
  }
  
  if (accY[1]==0) //Same as above...
  {countY++;}
  else {countY=0;}
  
  if (countY >= 25)
  {
    velY[1]=0;
    velY[0]=0;
  }
}