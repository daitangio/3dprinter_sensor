// Template for idle thread plus three schedulable threads

#include <NilRTOS.h>

// Use tiny unbuffered NilRTOS NilSerial library.
#include <NilSerial.h>

// Macro to redefine Serial as NilSerial to save RAM.
// Remove definition to use standard Arduino Serial.
#define Serial NilSerial



#include <sensor_subsystem.h>
// -------------------------

// The log queue 
#include <NilFIFO.h>
NilFIFO<String, 5> LogQueue;
boolean debug=true;

// onst unsigned char *str)
void log(String msg){

  String* p = LogQueue.waitFree(TIME_IMMEDIATE);  
  if(p==0) {
    Serial.println("Error Cannot log:"+msg);
  }else{
    *p=msg;
    LogQueue.signalData();

  }
}

//------------------------------------------------------------------------------
// Declare a stack with 128 bytes beyond context switch and interrupt needs.
NIL_WORKING_AREA(waWebServerThread, 128);


// WEB SERVER

#include "SPI.h"
#include "Ethernet.h"

// Debug incoming connection on serial port.
#define WEBDUINO_SERIAL_DEBUGGING 1
#include <WebServer.h>
// no-cost stream operator as described at 
// http://sundial.org/arduino/?page_id=119
template<class T>
inline Print &operator <<(Print &obj, T arg)
{ obj.print(arg); return obj; }


// CHANGE THIS TO YOUR OWN UNIQUE VALUE
static uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// CHANGE THIS TO MATCH YOUR HOST NETWORK
static uint8_t ip[] = { 192, 168, 1, 210 };
#define PREFIX ""

WebServer webserver(PREFIX, 80);

//////////////////////////////////

// Declare thread function for thread 1.
NIL_THREAD(WebServerThread, arg) {  

  
  
  while(TRUE) {
    nilThdSleep(600); 
    log("W+");
  }

/*
  // set pins 0-8 for digital input
  for (int i = 0; i <= 9; ++i)
    pinMode(i, INPUT);
  pinMode(9, OUTPUT);

  Ethernet.begin(mac, ip);
  webserver.begin();

  // webserver.setDefaultCommand(&defaultCmd);

  log("WebServer Configured");
  while (TRUE) {    
    // process incoming connections one at a time forever
    webserver.processConnection();
    //log("Serving...");
    // Sleep so lower priority threads can execute.
    nilThdSleep(2000);    
    // Dummy use of CPU - use nilThdSleep in normal app.
    //Serial.print("Dummy Web Server Processing...");
    //nilThdDelay(100);
    //Serial.print("...done");
    log("WebServer Loop finished");
  }
*/
}
//------------------------------------------------------------------------------
// Declare a stack with 4 bytes beyond context switch and interrupt needs.
// HeartBeat needs very little
NIL_WORKING_AREA(waBlinkHeartBeatThread, 4);


MSG(hok)="HeartBeat \\/";

// Declare thread function for HeartBeat.
// If the blinks fast, you can be sure it is working well
NIL_THREAD(BlinkHeartBeatThread, arg) {
  // Setup my led 
  pinMode(13, OUTPUT);
  while (TRUE) {    
    
    digitalWrite(13, HIGH);     
    nilThdSleep(100);
    digitalWrite(13, LOW); 
    nilThdSleep(100);    
    log(hok);
  }
}
//------------------------------------------------------------------------------
// Declare a stack with 64 bytes beyond context switch and interrupt needs.
NIL_WORKING_AREA(waLoggingThread, 64);

// Declare thread function for Logging thread

MSG(SYS_VERSION)=" Eva00 Universal Web Prober [SysTest]";
boolean CompactPrint=false;
NIL_THREAD(LoggingThread, arg) {
  int sleepCompensator=200; // Sleep value

  Serial.println(SYS_VERSION);
  nilPrintUnusedStack(&Serial);   
  while (TRUE) {

    /*Serial.print("LOG SLEEPING:");
    Serial.print(sleepCompensator);
    Serial.print(" ");
    nilPrintUnusedStack(&Serial); */
    //nilThdSleep(sleepCompensator);
    
    String *p= LogQueue.waitData(TIME_IMMEDIATE);
    if(!p) {
      /// No data.... hum not working very well     
      Serial.print("[LOG Nothing] ");    
      nilPrintUnusedStack(&Serial); 
      // Sleep so lower priority threads can execute.
      nilThdSleep(sleepCompensator);
    }else{
      String msg=*p;
      if(msg.length()!=0){
        Serial.print("[LOG] ");
        Serial.print(msg);
        if(LogQueue.freeCount() <=1){
          Serial.print(" Log Free count:");
          Serial.print(LogQueue.freeCount());
          //Serial.print(" LogSize:");
          //Serial.println(msg.length());         
        }
        Serial.print("\n");
      }     
      LogQueue.signalFree();
    }    
  } // while
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

// Used for serial communication reporting and general logging: move its priority up or down 
// it is quite complex to accompish minimal noise
NIL_THREADS_TABLE_ENTRY(NULL, LoggingThread, NULL, waLoggingThread, sizeof(waLoggingThread))

// Always last because it is the lower priority guy
NIL_THREADS_TABLE_ENTRY(NULL, BlinkHeartBeatThread, NULL, waBlinkHeartBeatThread, sizeof(waBlinkHeartBeatThread))
NIL_THREADS_TABLE_END()
//------------------------------------------------------------------------------
void setup() {



  Serial.begin(9600);
  // start kernel
  Serial.println("Setup()::: ARDUINO RESET");
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
