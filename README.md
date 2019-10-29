TinyTasker, by Ed Taychert -
minimal code size cooperative multitasking

 * TinyTasker is a manager for lightweight cooperation processes.
 TinyTasker automates the calling of synchronous processes at predetermined intervals
 The package also includes support for asynchronous periodic processes, class Periodic.
 
 * Processes managed by TinyTasker inherit from class Taskable.
 TinyTasker maintains ordered synchronisation between Taskable's. Sort of like a PLC...
 If that is not desired or required, many of the same benefits of coperative 
 multitasking can be had from inheriting from class Periodic instead.
 
 * The base class for both is class Runable. Runable has two virtual process called 
 "setup()" and "loop()" that have similar duties of the typical Arduino functions. 
 Derived classes implement or extend these functions as needed. By convention,
 setup() may be called repeatedly to set a class to a (new) time interval. Truly
 one-time initializers should be preformed at class instantiation or optionally in a begin() method.
  
 * TinyTasker includes an optional lightwieght global Semaphore package.
 Taskable's and Periodic's can use semaphores for interprocess communications.
 There is no need to lock semaphores as long as you don't use them in interupt service routine;
 
   * Semaphores are suited for synchronising with asynchronising with asynchronous processes.
   * Semaphores are suited for producer/consumer applications.
   * Semaphores are suited for remembering state between process invocations
   * Semaphores are used to indicate whether a process is ready to run.
 
For instance: a class that executes every 100 ms
```C++
class MyClass : protected Taskable { 
 int setup(long ms) {...};
 int loop(int arg) {...};
    ...
 } myClass; 
TinyTasker tinyTasker;
void setup() {
 myClass.setup(100);
 tinyTasker.add(myClass);
}
void loop() {
 tinyTasker.loop();
}
```
See "Blink13Task" example in the source header for a more complete example.

One advantage of class Taskable over simpler wait-for implementations is task synchronization. For example, if you create two routines to check on millis() to see if say, 1 second, has elapsed, they will slowly drift apart as other processing delays their ability to check each millisecond. TinyTasker presents one single time to all the tasks in the list regardless of how long each task takes. So, for example, Taskable's which run at 1 second intervals will all execute within the same TinyTasker slice. 
By default, TinyTasker executes every 4ms. For exact synchonization, you want 1) the entire set of tasks to complete within the 4ms slice and 2) all task periods to be a multiple of 4ms. The length of a time slice can be changed with the instantiation of the class. For example: `TinyTasker tinyTasker(5);` would start the tasker with an time interval of 5 milliseconds. There are hints within the code for making the length of the time slice dynamic.
