// Demo code, a TinyTasker version of "blink"

// build settings for my mirco:
// Nano, old bootloader

#define BLINK13TASK
#include "tinytasker.h"

TinyTasker tasker;
Blink13Task blinker ;

void setup() {
  // put your setup code here, to run once:

  tasker.add((Taskable*) & blinker );
}

void loop() {
  // put your main code here, to run repeatedly:
  
  tasker.loop();

}
