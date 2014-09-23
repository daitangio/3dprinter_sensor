// Template for idle thread plus three schedulable threads

#include <NilRTOS.h>

// Use tiny unbuffered NilRTOS NilSerial library.
#include <NilSerial.h>

// Macro to redefine Serial as NilSerial to save RAM.
// Remove definition to use standard Arduino Serial.
#define Serial NilSerial

//------------------------------------------------------------------------------
// Declare a stack with 64 bytes beyond context switch and interrupt needs.
NIL_WORKING_AREA(waWebServerThread, 64);

// Declare thread function for thread 1.
NIL_THREAD(WebServerThread, arg) {

  while (TRUE) {
  
    // Sleep so lower priority threads can execute.
    nilThdSleep(100);
    
    // Dummy use of CPU - use nilThdSleep in normal app.
    //Serial.print("Dummy Web Server Processing...");
    //nilThdDelay(100);
    //Serial.print("...done");
  }
}
//------------------------------------------------------------------------------
// Declare a stack with 64 bytes beyond context switch and interrupt needs.
NIL_WORKING_AREA(waBlinkHeartBeatThread, 64);

// Declare thread function for HeartBeat.
// If the blinks is every second, you can be sure it is working well
NIL_THREAD(BlinkHeartBeatThread, arg) {
  // Setup my led 
  pinMode(13, OUTPUT);
  while (TRUE) {
    
    //Serial.print("-");    
    
    digitalWrite(13, HIGH);     
    nilThdSleep(500);
    //Serial.print("/");
    digitalWrite(13, LOW); 
    nilThdSleep(500);
    //Serial.print("\\");

  }
}
//------------------------------------------------------------------------------
// Declare a stack with 64 bytes beyond context switch and interrupt needs.
NIL_WORKING_AREA(waLoggingThread, 64);

// Declare thread function for thread 3.
NIL_THREAD(LoggingThread, arg) {

  while (TRUE) {

    // Sleep so lower priority threads can execute.
    nilThdSleep(900);
    //nilPrintStackSizes(&Serial);
    nilPrintUnusedStack(&Serial);

  }
}
//------------------------------------------------------------------------------
/*
 * Threads static table, one entry per thread.  A thread's priority is
 * determined by its position in the table with highest priority first.
 *
 * These threads start with a null argument.  A thread's name is also
 * null to save RAM since the name is currently not used.
 */
NIL_THREADS_TABLE_BEGIN()
NIL_THREADS_TABLE_ENTRY(NULL, WebServerThread, NULL, waWebServerThread, sizeof(waWebServerThread))

// Used for serial communication reporting and general logging
NIL_THREADS_TABLE_ENTRY(NULL, LoggingThread, NULL, waLoggingThread, sizeof(waLoggingThread))

// Always last because it is the lower priority guy
NIL_THREADS_TABLE_ENTRY(NULL, BlinkHeartBeatThread, NULL, waBlinkHeartBeatThread, sizeof(waBlinkHeartBeatThread))
NIL_THREADS_TABLE_END()
//------------------------------------------------------------------------------
void setup() {



  Serial.begin(9600);
  // start kernel
  nilSysBegin();
}
//------------------------------------------------------------------------------
// Loop is the idle thread.  The idle thread must not invoke any
// kernel primitive able to change its state to not runnable.
void loop() {
  //nilPrintStackSizes(&Serial);
  //nilPrintUnusedStack(&Serial);
  //Serial.println("IDLE LOOP HERE");

  // Delay for one second.
  // Must not sleep in loop so use nilThdDelayMilliseconds().
  // Arduino delay() can also be used in loop().
  //nilThdDelayMilliseconds(1000);
}
