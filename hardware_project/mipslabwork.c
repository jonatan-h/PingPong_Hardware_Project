/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

// int mytime = 0x5957;
//
// int timeoutcount = 0;
//
// char textstring[] = "text, more text, and even more text!";

/* Lab-specific initialization goes here */
void initialization( void )
{ //
  IECSET(0) = 0x80;//enable interrupts for switch 1
  IPCSET(1) = 0x04000000;// set priority of switch 1 to 1

  IECSET(0) = 0x800;//enable interrupts for switch 2
  IPCSET(2) = 0x04000000;// set priority of switch 2 to 1

  IECSET(0) = 0x8000;//enable interrupts for switch 3
  IPCSET(3) = 0x04000000;// set priority of switch 3 to 1

  IECSET(0) = 0x080000; //enable interrupts for switch 4
  IPCSET(4) = 0x04000000; // set priority of switch 4 to 1

  PR2 = (80000000/256)/10000; //setting the period in the period register
  IEC(0) = 0x100;
  IPCSET(2) = 0x4;
  IFS(0) = 0;
  T2CON = 0x0070; //prescaling to 1:256 //written in collaboration

  volatile int *trise = (volatile int*) 0xbf886100;
  *trise &= 0xffffff00;

  PORTD &= 0xfc0;
  PORTF &= 0x2;
  enable_interrupt();
}
