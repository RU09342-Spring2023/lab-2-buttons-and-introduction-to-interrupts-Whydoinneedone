/*
 * OccupancyDetector.c
 *
 *  Created on: Jan 30, 2023
 *      Author: russty
 */
#include <msp430.h>

#define ARMED_STATE 0
#define WARNING_STATE 1
#define ALERT_STATE 2

char state = 0;
char i = 0;
// Put some initialization here
int main(void)
{
        WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    // Configure GPIO




         P6OUT &= ~BIT6;             // Clear P1.0 output latch for a defined power-on state
         P6DIR |= BIT6;              // Configure P6.6 to an Output

         P1OUT &= ~BIT0;             // Clear P1.0 output latch for a defined power-on state
         P1DIR |= BIT0;              // Configure P6.6 to an Output

         P2DIR &= ~BIT3;             // Configure P2.3 to an Input
         P4DIR &= ~BIT1;             // Configure P2.3 to an Input


         P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
         P2REN |= BIT3;                          // P2.3 pull-up register enable
         P2IES |= BIT3;                         // P2.3 Low --> High edge
         P2IE |= BIT3;                           // P2.3 interrupt enabled
         P2IFG &= ~BIT3;

         P4OUT |= BIT1;                          // Configure P2.3 as pulled-up
         P4REN |= BIT1;                          // P2.3 pull-up register enable
         P4IES |= BIT1;                         // P2.3 Low --> High edge
         P4IE |= BIT1;                           // P2.3 interrupt enabled
         P4IFG &= ~BIT1;
         PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                                  // to activate previously configured port settings


/*
         __bis_SR_register( GIE);                 // Enter LPM3 w/interrupt
*/
         _enable_interrupts();

while(1)
{
    if (i == 15)            // Used for the warning state, 15 cycles allow the led to blink for 15 sec before returning to safe state
    {   state=0;            // used to return to the safe state
        i=0;                // resets the count for further use
    }

  switch (state) {

  case 0:      // AllClear
        {
            P1OUT &= ~BIT0;                     // Make sure the red led is off
            P6OUT ^= BIT6;                      // Toggle P6.6
            __delay_cycles(100000);             // Delay for 100000*(1/MCLK)=0.1s
            P6OUT ^= BIT6;                      // Toggle P6.6
            __delay_cycles(3000000);            // Delay for 3000000*(1/MCLK)=3s
            break;
        }
  case 1:
    {

        P1OUT &= ~BIT0;                     // Make sure the red led is off
        P1OUT ^= BIT0;                      // Toggle P1.0
        __delay_cycles(500000);             // Delay for 500000*(1/MCLK)=0.5s
        P1OUT ^= BIT0;                      // Toggle P1.0
        __delay_cycles(500000);             // Delay for 500000*(1/MCLK)=0.5s
        i++;

            break;
        }



    case 2:
    {
        P1OUT &= ~BIT0;                     // Make sure the red led is off
        P1OUT ^= BIT0;                      // Turns on the red led
        break;

    }
    case 3:
       {
           state = 0;                   // Resets to case 0
           break;

       }

  }
}
}

/*
#pragma vector=PORT2_VECTOR                 // Used to simulate the detector
__interrupt void Port_2(void)
{
  if (state == 0)
      state= 1  ;
  else if (state == 1 )
      state= 2  ;


   P2IFG &= ~BIT3;
  //  state ++;                               // Used to increase the alert state by one level
 //   i=0;                                    // Used to reset the timer for warning level
 //   P2IFG &= ~BIT3;                         // Clear P2.3 IFG
//   }
*/


#pragma vector=PORT4_VECTOR                 // Used to simulate the sleep button
__interrupt void Port_4(void)
{
    state ++;                               // Used to increase the alert state by one level
    i=0;                                    // Used to reset the timer for warning level
   P4IFG &= ~BIT1;                          // Clear P4.1 IFG

}
