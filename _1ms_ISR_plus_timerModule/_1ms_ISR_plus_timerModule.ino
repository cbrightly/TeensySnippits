//**********************************************************************//
//  BEERWARE LICENSE
//
//  This code is free for any use provided that if you meet the author
//  in person, you buy them a beer.
//
//  This license block is BeerWare itself.
//
//  Written by:  Marshall Taylor
//  Created:  March 21, 2015
//
//**********************************************************************//

//HOW TO OPERATE
//  Make TimerClass objects for each thing that needs periodic service
//  pass the interval of the period in ticks
//  Set MAXINTERVAL to the max foreseen interval of any TimerClass
//  Set MAXTIMER to overflow number in the header.  MAXTIMER + MAXINTERVAL
//    cannot exceed variable size.

#define LEDPIN 13
#include "timerModule.h"
#include "stdint.h"

//Globals
IntervalTimer myTimer;

TimerClass msTimerA( 20 );
TimerClass msTimerB( 21 );

uint16_t msTicks = 0;
uint8_t msTicksMutex = 1; //start locked out

#define MAXINTERVAL 2000

void setup()
{
  //Serial.begin(9600);
  pinMode(LEDPIN, OUTPUT);

  // initialize IntervalTimer
  myTimer.begin(serviceMS, 1000);  // serviceMS to run every 0.001 seconds

  delay(2000);
  
  
}

int i = 0;
int intervalSeed = 20;
void loop()
{
  // main program
  
  if( msTicksMutex == 0 )  //Only touch the timers if clear to do so.
  {
    msTimerA.update(msTicks);
    msTimerB.update(msTicks);
    //Done?  Lock it back up
    msTicksMutex = 1;
  }  //The ISR should clear the mutex.
  
  
  if(msTimerA.flagStatus() == PENDING)
  {
    digitalWrite( LEDPIN, digitalRead(LEDPIN) ^ 1 );
  }
  if(msTimerB.flagStatus() == PENDING)
  {
    digitalWrite( LEDPIN, digitalRead(LEDPIN) ^ 1 );
  }
  i++;
  delay(1);
  if(i > 100)
  {
    i = 0;
    msTimerA.setInterval(intervalSeed);
    intervalSeed++;
    msTimerB.setInterval(intervalSeed);

  }
}

void serviceMS(void)
{
  uint32_t returnVar = 0;
  if(msTicks >= ( MAXTIMER + MAXINTERVAL ))
  {
    returnVar = msTicks - MAXTIMER;

  }
  else
  {
    returnVar = msTicks + 1;
  }
  msTicks = returnVar;
  msTicksMutex = 0;  //unlock
  
}


