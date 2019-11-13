#pragma once
/*
EncoderTask.h Copyright Ed Taychert, 2019
   This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "tinytasker.h"

// Support for three encoder tasks, Encoders[3], on port B. 
// unused pins may be used for other purposes
// See tinytasker.h for more information on using tasks.
// Example, have something like this in your hardware setup:
#if 0
// port B bit number assignments to encoders, you can change the order to whatever
static int EncoderPinA[3] = {0, 2, 4};
static int EncoderPinB[3] = {1, 3, 5};
EncoderTask bigknob(11,12); // create encoder "bigknob" on pins 10,11 which are B2 and B3
EncoderTask *Encoders[3] = {0, &bigknob, 0} ; // helper for interupt handler
myTasker.add((Taskable*)& bigknob)
// indicdual clicks can be used but preferred usage smooths out the rotation
// by condensing clicks into a simple -1,0,1 value because detents on the
// inexpensive encoders generate four changes per click.
  int direction = bigknob.change ;
  if ( direction > 1 ) direction = 1 ;
  else if ( direction < -1) direction = -1 ;
  else diection = 0 ;
  bigknob.change = 0;
#endif

extern EncoderTask *Encoders[3];
void encoderChange(void) ; // one ISR for all encoders

class EncoderTask : protected Taskable {
    int me;
    int lastChange;
  public:
    int lastA, lastB;
    int pinA, pinB;
    int change ;
    EncoderTask( int a, int b ) {
      pinA = a;
      pinB = b;
      pinMode(pinA,INPUT_PULLUP);
      pinMode(pinB,INPUT_PULLUP);
      lastA = digitalRead(pinA);
      lastB = digitalRead(pinB);         
      lastChange = change = 0;
      attachPCINT(digitalPinToPCINT(pinA), encoderChange, CHANGE);
      attachPCINT(digitalPinToPCINT(pinB), encoderChange, CHANGE);  
      setup(10);
    };
    // returns a bit for each encoder changed.
    int loop (int arg = 0 ) { 
      int changes = 0 ;
      if ( lastChange == change ) {
        changes = 0 ;
    } else { 
        lastChange = change;
        changes = 1;
      }
      return changes ;
    } 
    void put (int a)
    {
      change = lastChange = a;
    }
    setup(int ms=4){ // default, update every 4 milliseconds
      Taskable::setup(ms);
    }
} ;

// this is the pin change interrupt service routine
void encoderChange(void)
{    
  int a,b,i,j;
  int bits ;
  bits = PINB ;
  for ( i = 0 ; i < 3; i++) {
    if ( Encoders[i] == 0 ) // these encoder pins are not assigned;
      continue ; 
      
    j = EncoderPinA[i];
    a = ((bits & 1<<j) ?1:0) ;
    j = EncoderPinB[i];
    b = ((bits & 1<<j) ?1:0) ;
    
    if (Encoders[i]->lastA == a && Encoders[i]->lastB == b ) // no change
      continue ;
    if(Encoders[i]->lastA != a && Encoders[i]->lastB != b) { // missed a count!
      Encoders[i]->lastA = a;
      Encoders[i]->lastB = b; // old working code: a;
      continue ;
    }
    if ( Encoders[i]->lastB != a )
      Encoders[i]->change -- ;
    else 
     Encoders[i]->change ++ ;
     
    Encoders[i]->lastA = a;
    Encoders[i]->lastB = b;

  }
}
