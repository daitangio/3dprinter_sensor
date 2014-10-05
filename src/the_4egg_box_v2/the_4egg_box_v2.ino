
#include <pitches_it.h>
#include <Narcoleptic.h>

const int speakerOut=A5;


// Boot Music
void taDa(){
  //Serial.println("TADA!");
  tone(speakerOut,NOTE_SI5);
  delay(100);
  tone(speakerOut,NOTE_LA4);
  delay(100);
  tone(speakerOut,NOTE_DO3);
  delay(100);
  noTone(speakerOut);
  
  play(NOTE_DO3);
  play(NOTE_RE3);
  play(NOTE_MI3);
  play(NOTE_FA3);
  play(NOTE_RE3);
  //int scala2={ NOTE_DO3, 
  
  //gEM.queueEvent( EventManager::kEventUser0, 0);

}

void play(int n){
  tone(speakerOut,n);
  delay(100);
  noTone(speakerOut);
}


enum BlinkCmd {
  BlinkOn, BlinkOff
};

/*** FADER**/
int fadeLeds = 9;           // the pin that the LED is attached to
int reverseFadeLed=6;      // Reverse pin
int lg3=5;  //ledgroup 3 


void setup(){  
 pinMode(13, OUTPUT);
 pinMode(speakerOut, OUTPUT);
 pinMode(fadeLeds, OUTPUT);
 pinMode(reverseFadeLed,OUTPUT);
 taDa();
 Serial.begin(9600);
 Serial.println("The 4EggBox");
 Serial.println();
}

unsigned long lastToggfadeLedsBlinker;


void fadeOut(int pin){
  int brightness = 255;    // how bright the LED is
  int fadeAmount = -5;    // how many points to fade the LED by
  
  while(brightness >0 ){
    brightness = brightness + fadeAmount;
    analogWrite(pin, brightness);
    delay(90);
  }
}

void fadeIn(int pin){
  int brightness = 0;    // how bright the LED is
  int fadeAmount = 5;    // how many points to fade the LED by
  
  while(brightness <255 ){
    brightness = brightness + fadeAmount;
    analogWrite(pin, brightness);
    delay(90);
  }
}


// #define DEBUG yeppa

void loop() 
{

    
    /*** FADER PART */

    fadeIn(fadeLeds);
    fadeOut(fadeLeds);
    fadeIn(reverseFadeLed);
    fadeOut(reverseFadeLed);
    fadeIn(lg3);
    fadeOut(lg3);
}

