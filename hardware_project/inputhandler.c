#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

int getsw(){ //written in collaboration for lab
  int sw = (PORTD >> 8) & 0x0f;
  return sw;
}

int getbtns(){ //written in collaboration for lab
  int btn = (PORTD >> 6) & 0x3;
  return btn;
}

int getbtn2(){ //written in collaboration
  int btn = (PORTD >> 5) & 0x1;
  return btn;
}

int getbtn1(){//written in collaboration
  int btn = (PORTF >> 1) & 0x1;
  return btn;
}

// int sw4(){
//   int sw = (PORTD >> 11) & 0x1;
//   return sw;
// }



// int getbtns2(){
//   int btn = ((PORTD>>4) | (PORTF>>1)) & 0x3;
//   return btn;
// }
