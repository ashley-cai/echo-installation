#include <FastLED.h>

//LED Setup code
#define LED_PIN     5
#define NUM_LEDS    300
#define BRIGHTNESS  64 //change? variable
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
CRGBPalette16 savedPalette;
TBlendType    currentBlending;

// Pressure Sensor code
// Analog input pin that the Velostat is connected to
const int analogInPin = A0;  
const int ledPin = 9;  //pin 9 has PWM funtion

// value read from the Velostat
int sensorValue1 = 0; 
int sensorValue2 = 0; 
int sensorValue3 = 0; 
int sensorValue4 = 0; 
int sensorValue5 = 0;   
int averageSensor = 0;

void setup() {
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    
    setupNeutralPalette();
    currentBlending = LINEARBLEND;

    // initialize serial communications at 9600 bps:
    Serial.begin(9600);
    //init Analog Pin as PULLUP (meaning it sends out voltage)
    digitalWrite(analogInPin, HIGH);
    //Output LED
    pinMode(ledPin, OUTPUT); 
}


void loop()
{
    sensorValue1 = analogRead(analogInPin);
    sensorValue1 = map(sensorValue1, 10, 150, 0, 255);
    sensorValue2 = analogRead(analogInPin);
    sensorValue2 = map(sensorValue2, 10, 150, 0, 255); 
    sensorValue3 = analogRead(analogInPin);
    sensorValue3 = map(sensorValue3, 10, 150, 0, 255);  
    sensorValue4 = analogRead(analogInPin);
    sensorValue4 = map(sensorValue4, 10, 150, 0, 255);  
    sensorValue5 = analogRead(analogInPin);
    sensorValue5 = map(sensorValue5, 10, 150, 0, 255);  

    averageSensor = (sensorValue1 + sensorValue4 + sensorValue3 + sensorValue4 + sensorValue5) / 5;
    Serial.print("sensor = " );
    Serial.println(averageSensor);
//    Serial.println(sensorValue2);
//    Serial.println(sensorValue3);
//    Serial.println(sensorValue4);
//    Serial.println(sensorValue4);

    if (averageSensor < 20) {
      flashMood(CRGB(255, 100, 100));
      addMood(20, 0, 0);
    } else if (averageSensor < 40 && averageSensor > 20) {
      flashMood(CRGB(100, 100, 255));
      addMood(0, 0, 20);
    } else if (averageSensor < 100 && averageSensor > 40) {
      flashMood(CRGB(100, 255, 100));
      addMood(0, 20, 0);
    }

    int x = 0;
    while(x < 20){
        static uint8_t startIndex = 0;
        startIndex = startIndex + 1; /* motion speed */
        
        FillLEDsFromPaletteColors( startIndex);
        
        FastLED.show();
        FastLED.delay(1000 / UPDATES_PER_SECOND);
        x++;
    }
    
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

void flashMood(CRGB color){
     savedPalette = currentPalette;
//     uint8_t maxChanges = 24; 
//     nblendPaletteTowardPalette( savedPalette, currentPalette, maxChanges);
     currentPalette = CRGBPalette16(
                                   CRGB(0,0,0),   CRGB(0,0,0),  color,  color,
                                   CRGB(0,0,0),   CRGB(0,0,0),  color,  color,
                                   CRGB(0,0,0),   CRGB(0,0,0),  color,  color,
                                   CRGB(0,0,0),   CRGB(0,0,0),  color,  color);

    int x = 0;
    while(x < 40){
        static uint8_t startIndex = 0;
        startIndex = startIndex + 3; /* motion speed */
        
        FillLEDsFromPaletteColors( startIndex);
        
        FastLED.show();
        FastLED.delay(1000 / UPDATES_PER_SECOND);
        x++;
    }
}

void addMood(int red, int blue, int green){
    for( int i = 0; i < 16; i++) {
        CRGB lastcolor = currentPalette[i];
        int rand = random(2);
        if (rand == 0) {
          currentPalette[i] = CRGB( lastcolor.r + red, lastcolor.g + green, lastcolor.b + blue);
        }
        if(lastcolor.r >= 255 || lastcolor.g >= 255 || lastcolor.b >= 255) {
            //reset when color is too high
            currentPalette[i] = CRGB(100, 100, 100);
        }
//          Serial.print("red: ");
//          Serial.println(lastcolor.r);
//          Serial.print("blue: ");
//          Serial.println(lastcolor.g);
//          Serial.print("green: ");
//          Serial.println(lastcolor.b);
    }
  }


void setupNeutralPalette()
{
    CRGB neutral = CRGB(100, 100, 100);
    currentPalette = CRGBPalette16(
                                   CRGB(0,0,0),   CRGB(0,0,0),  neutral,  neutral,
                                   CRGB(0,0,0),   CRGB(0,0,0),  neutral,  neutral,
                                   CRGB(0,0,0),   CRGB(0,0,0),  neutral,  neutral,
                                   CRGB(0,0,0),   CRGB(0,0,0),  neutral,  neutral);
}
