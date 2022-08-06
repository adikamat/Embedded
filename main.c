
// FunctionPointer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "iolpc2148.h"
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
//#include "semphr.h"


#define RED_LED_PIN             16     // RED LED to PIN P0.16
#define GREEN_LED_PIN           28     // GREEN LED PIN  P1.28
#define PINK_LED_PIN            1     // PINK LED PIN P0.1
#define  BUTTON_PIN             18    // BTTON PIN P1.18
#define BUTTON_MASK             (1<<BUTTON_PIN )

void vTask_BlinkRedLed(void  *pvParameter );
void vTask_BlinkGreenLed(void  *pvParameter );
void vTask_Button(void  *pvParameter );

void delayMs(unsigned int x);



int main()
{
  BaseType_t xReturned;
   
  /* Create Blink RedLed Task */
   xTaskCreate( vTask_BlinkRedLed,"TaskRedLed",100,NULL,1,NULL);
   /* Create Blink GreenLed Task */
   xTaskCreate( vTask_BlinkGreenLed,"TaskGreenLed",100,NULL,1,NULL);
    /* Create Button Task */
   xTaskCreate( vTask_Button,"TaskButton",100,NULL,2,NULL);
  
    /* Start Schedular*/
    vTaskStartScheduler();
  /* if all is well in main then we will never reach here   */
  for(; ;);
}

/***************************************************************
 Task Button
***************************************************************/
void vTask_Button(void  *pvParameter )
{
  IO0DIR |= (1 << PINK_LED_PIN); // port0 pin 1 direction out
  IO1DIR &= ~(1 << BUTTON_PIN);   // port1 pin 18 direction IN

  while(1)
  { /* Check whether button is pressed or Not.
       if press make Pink LED ON else OFF*/
     if( IO1PIN & (BUTTON_MASK))
         IO0SET  |= (1 << PINK_LED_PIN);  /* Pink Led ON */
     else
         IO0CLR |= (1 << PINK_LED_PIN); // Pink Led OFF
     
  }
} 

/***************************************************************
 Task GreeLed Blink
***************************************************************/
void vTask_BlinkGreenLed(void  *pvParameter )
{
  IO1DIR |= (1 << GREEN_LED_PIN); // port1 pin 28 direction out
  while(1)
  {
      IO1SET  |= (1 << GREEN_LED_PIN);  // Turn ON LED
      delayMs(500);               // 1 second delay
      IO1CLR |= (1 << GREEN_LED_PIN); // Turn OFF LED
      delayMs(500);               // 1 second delay
  }
}  

/***************************************************************
 Task RedLed Blink
**************************************************************/
void vTask_BlinkRedLed(void  *pvParameter )
{
  IO0DIR |= (1 << RED_LED_PIN); // port0 pin 16 direction out
  while(1)
  {
    IO0SET  |= (1 << RED_LED_PIN);  // Turn ON LED
      delayMs(100);               // 1 second delay
      IO0CLR |= (1 << RED_LED_PIN); // Turn OFF LED
      delayMs(100);               // 1 second delay
  }
}  
/***************************************************************
Blocking delay function
**************************************************************/
void delayMs(unsigned int x)
{
    unsigned int j;

    for(;x>0;x--)
        for(j=0; j<0xFFF; j++);
 }
/***************************************************************
  PLL Init function
***************************************************************/
/*void PLLInit(void)
{
  PLLCON=0x01;             // PPLE=1 & PPLC=0 so it will be enabled  but not connected after FEED sequence
  PLLCFG=0x24;          // set the multipler to 5 (i.e actually 4) i.e 12x5 = 60 Mhz (M - 1 = 4), Set P=2 since we want FCCO in range So , Assign PSEL =01 in PLL0CFG as per the table.
  PLLFEED=0XAA;          //feed
  PLLFEED=0X55;
  while((PLLSTAT&(1<<10))==0);      // check whether PLL has locked on to the  desired freq by reading the lock bit in the PPL0STAT register
  PLLCON=0x03;          //enable & connect pll
  PLLFEED=0XAA;
  PLLFEED=0X55;
  VPBDIV = 0x01;          // PCLK is same as CCLK i.e 60Mhz
}*/
