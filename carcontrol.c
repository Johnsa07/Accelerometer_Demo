#include "includes.h"

#define CAR_PORT   GPIOE
#define CAR_FORWARD_MASK   GPIO_Pin_2
#define CAR_BACKWARDS_MASK    GPIO_Pin_3
#define CAR_RIGHT_MASK    GPIO_Pin_4
#define CAR_LEFT_MASK    GPIO_Pin_5

void Car_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin =  JS_RIGHT_MASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(JS_RIGHT_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  JS_LEFT_MASK;
  GPIO_Init(JS_LEFT_PORT, &GPIO_InitStructure);
  
    //PE2 init
  GPIO_InitStructure.GPIO_Pin =  CAR_FORWARD_MASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(CAR_PORT, &GPIO_InitStructure);

//PE3 init
GPIO_InitStructure.GPIO_Pin = CAR_BACKWARDS_MASK;
GPIO_Init(CAR_PORT, &GPIO_InitStructure);
//PE4 init
GPIO_InitStructure.GPIO_Pin = CAR_RIGHT_MASK;
GPIO_Init(CAR_PORT, &GPIO_InitStructure);
//PE5 init
GPIO_InitStructure.GPIO_Pin = CAR_LEFT_MASK;
GPIO_Init(CAR_PORT, &GPIO_InitStructure);
}

/*************************************************************************
 * Function Name: Forward
 * Parameters: none
 *
 * Return: none
 *
 * Description: Reset the bit for going backwards and set the one going 
 *
 *************************************************************************/
void Forward(void)
{
        GPIO_ResetBits(CAR_PORT, CAR_BACKWARDS_MASK); //Reset bit for not going backwards
        GPIO_SetBits(CAR_PORT, CAR_FORWARD_MASK); //Set bit for going forward
}

/*************************************************************************
 * Function Name: Backwards
 * Parameters: none
 *
 * Return: none
 *
 * Description: Reset the bit for going forwards and set the one going backwards
 *
 *************************************************************************/
void Backwards(void)
{
        GPIO_ResetBits(CAR_PORT, CAR_FORWARD_MASK); //Reset bit for not going forward
        GPIO_SetBits(CAR_PORT, CAR_BACKWARDS_MASK); //Set bit for going backwards
}

/*************************************************************************
 * Function Name: Stop
 * Parameters: none
 *
 * Return: none
 *
 * Description: Resets bits for forwards and backwards
 *
 *************************************************************************/
void Stop(void)
{
        GPIO_ResetBits(CAR_PORT, CAR_FORWARD_MASK); //Reset bit for not going forward
        GPIO_ResetBits(CAR_PORT, CAR_BACKWARDS_MASK); //Set bit for not going backwards
}

/*************************************************************************
 * Function Name: Right
 * Parameters: none
 *
 * Return: none
 *
 * Description: Resets bits for forwards and backwards
 *
 *************************************************************************/
void Right(void)
{
        GPIO_ResetBits(CAR_PORT, CAR_LEFT_MASK); //Reset bit for not going forward
        GPIO_SetBits(CAR_PORT, CAR_RIGHT_MASK); //Set bit for not going backwards
}

/*************************************************************************
 * Function Name: Left
 * Parameters: none
 *
 * Return: none
 *
 * Description: Resets bits for forwards and backwards
 *
 *************************************************************************/
void Left(void)
{
        GPIO_ResetBits(CAR_PORT, CAR_RIGHT_MASK); //Reset bit for not going forward
        GPIO_SetBits(CAR_PORT, CAR_LEFT_MASK); //Set bit for not going backwards
}

/*************************************************************************
 * Function Name: StopTurn
 * Parameters: none
 *
 * Return: none
 *
 * Description: Resets bits for forwards and backwards
 *
 *************************************************************************/
void StopTurn(void)
{
        GPIO_ResetBits(CAR_PORT, CAR_LEFT_MASK); //Reset bit for not going forward
        GPIO_ResetBits(CAR_PORT, CAR_RIGHT_MASK); //Set bit for not going backwards
}

void GoCar(car_state command)
{
  if (command == 0){
    StopTurn();
    Forward();
  }
  else if (command == 1){
    Right();
    Forward();
  }
    else if (command == 2){
    Left();
    Forward();
  }
    else if (command == 4){
    StopTurn();
    Backwards();
  }
      else if (command == 5){
    Right();
    Backwards();
  }
      else if (command == 6){
    Left();
    Backwards();
  }  else {
    StopTurn();
    Stop();
  }
    

}

void GoCar(int Test, int TestTurn)
{    //Control speed
  
    if (Test==1) 
    {
      Forward();
    }
    else if(Test==2)
    {
      Backwards();
    }
    else if(Test==0)
    {
      Stop();
    }
    
    //Control direction
        if (TestTurn==1) 
    {
      Right();
    }
    else if(TestTurn==2)
    {
      Left();
    }
    else if(TestTurn==0)
    {
      StopTurn();
    }
}
