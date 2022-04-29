


#define USE_ARDUINO_INTERRUPTS true    // lage interrupts op de arduino voor meer accurate data
#include <PulseSensorPlayground.h>     // include libraries
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
const int PulseWire = 0; 
int Threshold = 550; 

PulseSensorPlayground pulseSensor;

#define SCREEN_WIDTH 128 // size van de display
#define SCREEN_HEIGHT 64

// declare het scherm
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


//functie voor om ledjes aan te sturen
void writeLed(int a);

//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;
byte ledArray[10];



void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Adres voor 0x3D 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay(); //clear het display

  display.setTextSize(7); //zet de fontsize
  display.setTextColor(WHITE); //zet kleur
  

  pulseSensor.analogInput(PulseWire); //de pulse sensor  
  
  pulseSensor.setThreshold(Threshold);
     if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //wanneer het scherm power krijgt
  } 
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  ledArray[0] = 0xFF; //11111111
  ledArray[1] = 0xFE; //11111110
  ledArray[2] = 0xFC; //11111100
  ledArray[3] = 0xF8; //11111000
  ledArray[4] = 0xF0; //11110000
  ledArray[5] = 0xE0; //11100000
  ledArray[6] = 0xC0; //11000000
  ledArray[7] = 0x80; //10000000
  ledArray[8] = 0x00; //00000000
  ledArray[9] = 0xE0; //11100000
 
}

void loop() {
    int myBPM = pulseSensor.getBeatsPerMinute();
    int bloodSignal = analogRead(PulseWire);
    Serial.println(bloodSignal);
    
    delay(100);
    if(myBPM < 40){ //lees alleen als bpm over 40 is, dus niet als er niemand in zit
      display.setCursor(0, 0);
      display.clearDisplay();
      display.println("---");
    
      display.display();
      writeLed(ledArray[8]);
    }else{
          if (pulseSensor.sawStartOfBeat()) { //als pulse sensor bets vindt
          display.setCursor(0, 0); //begin te printen in het begin
          display.clearDisplay(); //clear de display
          display.println(myBPM); //print de bpm van je hartslag
          
          display.display();
          delay(20);
          
         }
         //calibratie voor de ledjes vergeleken met je bloed druk
         if (bloodSignal < 150){
         writeLed(ledArray[8]);  //1e 2 ledjes
         }
         if(bloodSignal > 150 && bloodSignal < 225){
         writeLed(ledArray[7]); //2e 2 ledjes
         }
         if(bloodSignal > 225 && bloodSignal < 300){
         writeLed(ledArray[6]); //3e 2 ledjes
         }
         if(bloodSignal > 300 && bloodSignal < 375){
         writeLed(ledArray[5]); //4e 2 ledjes
         }
         if(bloodSignal > 375 && bloodSignal < 450){
         writeLed(ledArray[4]);  //5e 2 ledjes
         }
         if(bloodSignal > 450 && bloodSignal < 525){
         writeLed(ledArray[3]); //6e ledjes
         }
         if(bloodSignal > 525 && bloodSignal < 600){
         writeLed(ledArray[2]); //7e ledjes
         }
         if(bloodSignal > 600 && bloodSignal < 675){
         writeLed(ledArray[1]); //8e ledjes
         }    
         if(bloodSignal > 675){
         writeLed(ledArray[0]); //alle ledjes
         
         }

    }
  

}

    //functie om latch laag > stuur lampjes aan > latch hoog
    void writeLed(int a){
      digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, a);
    digitalWrite(latchPin, HIGH);
    }
