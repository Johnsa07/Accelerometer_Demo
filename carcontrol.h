#ifndef __CARCONTROL_H
#define __CARCONTROL_H

//#include "stm32f10x_gpio.h"
#include "includes.h"

#define JS_RIGHT_MASK   GPIO_Pin_2
#define JS_RIGHT_PORT   GPIOC
#define JS_LEFT_MASK    GPIO_Pin_7
#define JS_LEFT_PORT    GPIOE

void Forward(void);
void Backwards(void);
void Stop(void);
void StopTurn(void);
void Left(void);
void Right(void);
void GoCar(int, int);
void GoCar(car_state);
void Car_Init(void);

#endif