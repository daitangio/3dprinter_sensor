
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
int greenLed=6;      // Reverse pin
int blueLed=5;  //ledgroup 3 
const int DelayTime=44; //was 90

void setup(){  
 pinMode(13, OUTPUT);
 pinMode(speakerOut, OUTPUT);
 pinMode(fadeLeds, OUTPUT);
 pinMode(greenLed,OUTPUT);
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
    delay(DelayTime);
  }
}


void fadeOut3(int pin1,int pin2, int pin3){
  int brightness = 255;    // how bright the LED is
  int fadeAmount = -5;    // how many points to fade the LED by
  
  while(brightness >0 ){
    brightness = brightness + fadeAmount;
    analogWrite(pin1, brightness);
    analogWrite(pin2, brightness);
    analogWrite(pin3, brightness);
    delay(DelayTime);
  }
}



void fadeIn(int pin){
  int brightness = 0;    // how bright the LED is
  int fadeAmount = 5;    // how many points to fade the LED by
  
  while(brightness <255 ){
    brightness = brightness + fadeAmount;
    analogWrite(pin, brightness);
    delay(DelayTime);
  }
}


// #define DEBUG yeppa

void loop() 
{

    
    /*** FADER PART */

    fadeIn(fadeLeds);
    fadeOut(fadeLeds);
    fadeIn(greenLed);
    fadeOut(greenLed);
    fadeIn(blueLed);
    //fadeOut(blueLed);
    fadeIn(greenLed);
    fadeIn(fadeLeds);
    // TODO Replace with a global fade out:
    // fadeOut(blueLed);
    fadeOut3(blueLed,fadeLeds,greenLed);
    
    // turn off all
    analogWrite(greenLed,0);
    analogWrite(fadeLeds,0);
    analogWrite(blueLed,0);
    delay(DelayTime);
 
}

