#include <EventManager.h>
#include <pitches_it.h>
#include <Narcoleptic.h>

const int speakerOut=A5;
// No interrupts, so can use the non-interrupt safe mode
EventManager gEM( EventManager::kNotInterruptSafe );


// Boot Music
void taDa(int event, int p){
  //Serial.println("TADA!");
  tone(speakerOut,NOTE_SI5);
  delay(100);
  tone(speakerOut,NOTE_LA4);
  delay(100);
  tone(speakerOut,NOTE_DO3);
  delay(100);
  noTone(speakerOut);
  //gEM.queueEvent( EventManager::kEventUser0, 0);

}


enum BlinkCmd {
  BlinkOn, BlinkOff
};

/*** FADER**/
int fadeLeds = 9;           // the pin that the LED is attached to
int reverseFadeLed=6;      // Reverse pin
int brightness = 120;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by






void setup(){  
 pinMode(13, OUTPUT);
 pinMode(speakerOut, OUTPUT);
 pinMode(fadeLeds, OUTPUT);
 pinMode(reverseFadeLed,OUTPUT);
 Serial.begin(9600);
 Serial.println("The 4EggBox");
 Serial.println();
 gEM.addListener( EventManager::kEventUser0, taDa );
 gEM.addListener( EventManager::kEventUser1, blinker );
 
 // fire the Tada
 gEM.queueEvent( EventManager::kEventUser0, 0);
}

unsigned long lastToggfadeLedsBlinker;
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
  lastToggfadeLedsBlinker=millis();
}



// #define DEBUG yeppa

void loop() 
{
    //gEM.queueEvent( EventManager::kEventUser0, 0);
    
    // Handle any events that are in the queue
    gEM.processEvent();
    #ifdef DEBUG
    // Push a blinker and a sensor check
    if ( ( millis() - lastToggfadeLedsBlinker ) > 500 ) 
    {      
      gEM.queueEvent( EventManager::kEventUser1, lastBlinkCmd);
      if(lastBlinkCmd==BlinkOn) { 
        lastBlinkCmd=BlinkOff;
      }else{
        lastBlinkCmd=BlinkOn;
      }
      
    }
    #endif
    
    /*** FADER PART */
   analogWrite(fadeLeds, brightness);    
   analogWrite(reverseFadeLed, 255- brightness);

    // change the brightness for next time through the loop:
    brightness = brightness + fadeAmount;
    #ifdef DEBUG
    Serial.print(fadeAmount);
    Serial.print(" ");
    Serial.println(brightness);
    #endif
    // reverse the direction of the fading at the ends of the fade: 
    if (brightness <= 50 || brightness >= 255) {
      fadeAmount = -fadeAmount ; 
      // Fire the TADA then black
      if(brightness ==255) gEM.queueEvent( EventManager::kEventUser0, 0); 
    }     
    // wait for 30 or 90 milliseconds to see the dimming effect    
    delay(90); // was 90
    // Narcoleptic did not work well here 
    //Narcoleptic.delay(40);
    

}

