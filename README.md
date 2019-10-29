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
 setup() may be called repeated to set a class to a (new) initial state. Truly
 one-time initializers should be preformed at class instantiation or optionaslly in begin().
  
 * TinyTasker included an option lightwieght global Semaphore package.
 Taskable's and Periodic's can use semaphores for interprocess communications.
 There is no need to lock semaphores as long as you don't use them in interupt service routine;
 
   * Semaphores are suited for synchronising with asynchronising with asynchronous processes.
   * Semaphores are suited for producer/consumer applications.
   * Semaphores are suited for remembering state between process invocations
   * Semaphores are used to indicate whether a process is ready to run.
 
