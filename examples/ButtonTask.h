#pragma once
/*
ButtonTask.h Copyright Ed Taychert, 2019
GNU Lesser General Public License version 3
*/

#include "tinytasker.h"

#if 0
// archetype for running a button:
    // Create a button object in Arduino "setup()" or a global section
    Button myButton(myPin);
    // setup the task interval in Ardiono "setup()", 20 ms is good value
    myButton.setup(20);
    // add your button to the tasker
    tinyTasker.add((Taskable*) &myButton);

    // use code like this in your add to check for button press'es and long press'es 
    // ... 
    if (myButton.state == Button::pressed){
      bit = 1-bit ;  // an example, do something here
      // note a pressed-state must be cleared with an "up" before the next press
      ShiftButton.state = ShiftButton.up;
    } else if (ShiftButton.state == ShiftButton.longPress) {
      bit = 0 ; // do something
      ShiftButton.state = ShiftButton.up;
    } 
#endif

class ButtonTask : protected Taskable {
    unsigned int read, lastRead;
    int lastState;
  private:
    long lastChange;
    int myPin ;
  public:
    enum { up=0, down, pressed, longPress, timeout };
    int state ;
    int timeButton;
    ButtonTask (int pin) {
      myPin = pin;
      pinMode( myPin, INPUT_PULLUP);
      lastState = state = up;
      lastChange = millis() ;
      timeButton = 0 ;
      setup(20);
    } 
    int loop (int arg=0) {
      char pinstate;
      int ms ;
      unsigned long now ;
      now = millis() ;
      ms = (int)(now-lastChange) ; // overflow asks for a deadlock restart?
      // no action until previous action is cleared. Potential deadlock must be handled.
      if (state == longPress) {
        timeButton = ms;
        return longPress ;
      } 
      if (state == pressed){ 
        timeButton = ms;
        return pressed ;
      }  
      pinstate = (digitalRead(myPin)) ? up : down ;
      if (lastState == longPress && pinstate == down ) {
        return up ; // do nothing until the button actually gets up
      } else if ( lastState == longPress && pinstate == up ) { // restart
        lastState = up; 
        return up;
      }
      if (state == up && pinstate == down) {  // a press has been started
        lastChange = now;
        lastState = up;
        state = down ;
        timeButton = 0 ;
        return down ;
      }
      if (state == down && pinstate == up) {  // a press
        lastChange = now;
        if ( timeButton > 1000 ) // released a long press (1.000s is a long press)
          state = lastState = up; 
        else 
          state = lastState = pressed;
        return state ;
      }     
      if (state == down && pinstate == down) { // long press?
        if ( ms > 1000 ) { // same state for at least a second
          // long press, been down for a second
          lastChange = now ;  
          timeButton = 0 ;
          state = lastState = longPress ;
          return longPress ;
        }
        // no change
        state = lastState = down;
        return down ;
      }
      // shouldn't get here
      return up ; // idle 
    } ;
} ;
