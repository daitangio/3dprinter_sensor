#include <EventManager.h>
#include <dht11.h>
dht11 DHT;

#define DHT11_PIN 4       // define a pin for DHT sensor

enum BlinkCmd {
  BlinkOn, BlinkOff
};


// No interrupts, so can use the non-interrupt safe mode
EventManager gEM( EventManager::kNotInterruptSafe );

void setup(){  
 pinMode(13, OUTPUT);
 Serial.begin(9600);
 Serial.println("3D Printer OSNF - Open Sensors Network Framework 0.1 ");
 Serial.print("DHT LIBRARY VERSION:");
 Serial.println(DHT11LIB_VERSION);
 Serial.print("SENSOR NAME:");
 Serial.println("OSA1");
 Serial.println();
 gEM.addListener( EventManager::kEventUser0, temperatureListener );
 gEM.addListener( EventManager::kEventUser1, blinker );

}

unsigned long lastToggledBlinker;
BlinkCmd lastBlinkCmd=BlinkOn;

void blinker(int event, int cmd){
  BlinkCmd command= (BlinkCmd)cmd;
  switch (command) {
    case BlinkOn:
      digitalWrite(13, HIGH); 
      break;
    case BlinkOff:
      digitalWrite(13, LOW); 
      break;
    default:
       Serial.println("blinker error unkown command:"+cmd);
  }
  lastToggledBlinker=millis();
}


void temperatureListener(int event, int pin){
  int chktemp =  DHT.read(pin); // DHT.read(DHT11_PIN);
  switch (chktemp){
    case DHTLIB_OK:  
                //Serial.print("OK,\t"); 
                {
                  int temp = DHT.temperature; 
                  int humidity = DHT.humidity;
                  Serial.print("TEMP:");
                  Serial.print(temp);
                  Serial.print(",");
                  Serial.print("HUM:");
                  Serial.print(humidity);                 
                }
                // Retrofit an event                
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                Serial.print("Checksum error,\t"); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                Serial.print("Time out error,\t"); 
                break;
    default: 
                Serial.print("Unknown error,\t"); 
                break;
  }
}





void loop() 
{
    // Handle any events that are in the queue
    gEM.processEvent();
    // Push a blinker and a sensor check
    if ( ( millis() - lastToggledBlinker ) > 2000 ) 
    {      
      gEM.queueEvent( EventManager::kEventUser1, lastBlinkCmd);
      if(lastBlinkCmd==BlinkOn) { 
        lastBlinkCmd=BlinkOff;
      }else{
        lastBlinkCmd=BlinkOn;
      }
      gEM.queueEvent( EventManager::kEventUser0, DHT11_PIN);
    }
    // Push another event after 2000 sec
    
    delay(2000);
       

}

