#pragma once
/*
 * Copyright 2019, Edward L. Taychert
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
#ifndef MAXTASKS
#define MAXTASKS 10
#endif

// just for documentation purposes ...
#ifndef USETINYSEMAPHORES
#undef USETINYSEMAPHORES
#endif
#ifndef BLINK13TASK
#undef BLINK13TASK
#endif


class Runable {
  protected:
    byte state ;  // run-state not implented yet
    byte taskID ;
    unsigned long interval;
    unsigned long wait ;
  public :
    enum {  WAITING = 0, RUNNING};
    virtual int setup (long) = 0 ;
    virtual int loop (int) = 0 ;
    void setTaskID(int n){taskID = n;};
    int getTaskID() { return taskID;};
    int getState () { return state; };
    int setState (int s) { return (state=s);};
    int ms() {
      unsigned long now ;
      now = millis() ;
      if ( wait <= 0 ) { // setup a new delay
        wait = now + interval ;
        return 0 ;
      } 
      if ( wait < now ) { // delay has expired
        wait = now + interval ;  // set up for next use
        return 1 ;
      }
      return 0 ;  // keep waiting ...
    };
} ;

class Periodic : protected Runable { 
  public:
    int setup(long interval) {
      this->interval = interval; 
      state = RUNNING;
      wait = 0 ;
      return 0 ;
    };
    int loop(int arg = -1) {
      if (state == RUNNING ) 
        return ms();
      else return WAITING ;
    };
    Periodic() {
      wait = 0L;
    };
}; 

// TBD: Derived classes are expensive, about 22 bytes each at runtime. 
class Taskable : protected Runable {
  protected:
    friend class TinyTasker ;
  public:
    Taskable(long ms=4) {
      setup(ms);
    }
    int setup(long ms) {
      interval = ms; 
      state = RUNNING;
      wait = 0 ;
      return 0 ;
    }
    virtual int loop(int arg ) = 0 ;
};

class TinyTasker : protected Taskable { 
  Taskable* tasks[MAXTASKS];
  int numRunable ;
  public:
    TinyTasker(long ms =4) {
      setup(ms);
      numRunable = 0 ;
    }
    int add(Taskable *t) {
      t->setTaskID(-1);
      for (int i=0 ; i<MAXTASKS; i++ ) {
        if (tasks[i] == 0 ) {
          tasks[i] = t;
          t->setTaskID(i);
          return i;
        }
      }
      return 0;
    }
    int remove (int ti ) {
      if ( ti < MAXTASKS ) 
        if ( tasks[ti] != 0 ) {
          tasks[ti] = 0 ;
          return 1 ;
        }
      return 0;
    }
    int remove (Taskable *t) {
      for (int i = 0 ; i < MAXTASKS ; i++) {
        if ( tasks[i] == t) {
          tasks[i] = 0;
          return 1;
        }
      }
      return 0;
    }
    int loop(int arg=0 ) {
      if ( ms() ) {
        long now = millis();
        for (int i = 0 ; i < MAXTASKS ; i ++) {
          if (tasks[i] != 0 ) {
            if (tasks[i]->wait > now ) 
              continue;
            if ( tasks[i]->wait <= now ) 
              tasks[i]->loop(0);
            // rescedule next (including starting first)
            tasks[i]->wait = now + tasks[i]->interval ;
          }
        }
        // Future: Compare loop execution time and adduust as necessary 
        // TBD compare "now" to currect millis() to get loop time.
        return 1;
      }
    }
};

#ifdef BLINK13TASK
class Blink13Task : public Taskable {
  int pin;
  public:
    Blink13Task() {
      pinMode(13, OUTPUT);
      pin = 13;
      Taskable:: setup(200);
    }
    int loop (int arg=0) {
      if (interval < 500)
        digitalWrite(pin, 0);
      else
        digitalWrite(pin, 1);
      setup(1000 - interval);
      return 0;
    }
} blink13Task ;
#endif

#ifdef USETINYSEMAPHORES
#ifndef MAXSEMAS
#define MAXSEMAS 10
#endif

volatile int Semaphore[MAXSEMIS];
#define semaInc(a) (a)++
#define semaDec(a) (a)--
#define semaDrain(a) (a) = 0 
#endif
