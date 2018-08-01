//#define DEBUG       TRUE            //Serial Debug Mode (Comment out to stop serial printlns)

#define PIN_LED_ON  8                 //Pin to turn on the LED (Common Anode)
#define PIN_LED_R   9                 //Red pin for the LED
#define PIN_LED_G   6                 //Green pin for the LED
#define PIN_LED_B   5                 //Blue pin for the LED

#define PIN_TEMP_ON 14                //Pin to turn on the Temp Sensor
#define PIN_TEMP    A0                //Pin to read the ambient temp

#define PIN_LDR_ON  2                 //Pin to turn the LDR on
#define PIN_LDR     A3                //Pin to read the LDR ambient light value

#define BRIGHTNESS_MIN 1              //Minimum value to use when analog writing out the LED
#define BRIGHTNESS_MAX 10             //Maximum value to use when analog writing out the LED

#define TEMP_IDEAL_MIN 18             //Set the LED to BLUE below this temperature
#define TEMP_IDEAL_MAX 23             //Set the LED to RED above this temperature
#define TEMP_WARNING_MIN 15           //Flash BLUE below this temperature
#define TEMP_WARNING_MAX 25           //Flash RED above this temperature
#define TEMP_SAMPLES 10               //Number of samples to use when calculating average temp

#define V_REF 4.7                     //Voltage to use when reading analogue in for the LM335z Temperature Sensor

byte ledBrightness = BRIGHTNESS_MIN;  //The current brightness of the indicator LED
float currentTemp = TEMP_IDEAL_MIN;   //Current ambient temperature (averaged)
float tempReadings[TEMP_SAMPLES];     //The most recent temperature readings



/**
 * Setup
 */
void setup() {
  #ifdef DEBUG 
  //Init the Serial Monitor
  Serial.begin(9600);
  #endif

  //Setup the LED pins
  pinMode(PIN_LED_ON, OUTPUT);
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);

  //Setup the LDR pins
  pinMode(PIN_LDR_ON, OUTPUT);
  pinMode(PIN_LDR, INPUT);

  //Setup the TEMP pins
  pinMode(PIN_TEMP_ON, OUTPUT);
  pinMode(PIN_TEMP, INPUT);

  //Turn off the LED
  digitalWrite(PIN_LED_ON, LOW);
  digitalWrite(PIN_LED_R, LOW);
  digitalWrite(PIN_LED_G, LOW);
  digitalWrite(PIN_LED_B, LOW);

  //Turn off the Temp sensor
  digitalWrite(PIN_TEMP_ON, LOW);
}



/**
 * Main Loop
 */
void loop() {
  updateTemp();
  updateBrightness();
  updateLed();

  //Ensure the LED is ON
  digitalWrite(PIN_LED_ON, HIGH);
  delay(500);

  //If the current temperature is above or below the warning threshold, flash the LED
  if ((currentTemp < TEMP_WARNING_MIN) || (currentTemp > TEMP_WARNING_MAX)) {
    digitalWrite(PIN_LED_ON, LOW);
  }

  delay(500);
}



/**
 * set the LED colour and brightness
 */
void setLed(byte R, byte G, byte B) {
  analogWrite(PIN_LED_R, 255 - (R * (float(ledBrightness) / 255)));
  analogWrite(PIN_LED_G, 255 - (G * (float(ledBrightness) / 255)));
  analogWrite(PIN_LED_B, 255 - (B * (float(ledBrightness) / 255)));
}



/**
 * Measure the current room temperature
 */
void updateTemp() {
  //Power the Sensor
  digitalWrite(PIN_TEMP_ON, HIGH);

  //Give the sensor some time to get going
  delay(10);

  //Take the reading
  float input = (float) analogRead(PIN_TEMP);
  input = (input * V_REF) / 1024.0;
  float newTemp = (input / 0.01) - 273.15;

  float avgTemp = 0;
  byte readingCount = 0;

  //Shift the readings
  for(byte i = TEMP_SAMPLES - 1; i > 0; i--)
  {
    tempReadings[i] = tempReadings[i-1];
  }
  tempReadings[0] = newTemp;

  //Calculate the average temp
  for(byte i = 0; i < TEMP_SAMPLES; i++)
  {
    if (tempReadings[i] > 0) {
      avgTemp += tempReadings[i];
      readingCount++;
    }
  }
  avgTemp = avgTemp / readingCount;

  //Apply the average temp
  currentTemp = avgTemp;

  //Turn off the Sensor
  digitalWrite(PIN_TEMP_ON, LOW);

  //Serial Debug (If enabled)
  #ifdef DEBUG 
  Serial.print("Temp Readings: [");
  for(byte i = 0; i < TEMP_SAMPLES; i++)
  {
    Serial.print(tempReadings[i]);
    Serial.print(", ");
  }
  Serial.println("]");
  Serial.print("Temp: ");
  Serial.print(currentTemp);
  Serial.println("c");
  #endif
}



/**
 * Measure the ambient light and update the LED Brightness
 */
void updateBrightness() {
  //Power the LDR Sensor
  digitalWrite(PIN_LDR_ON, HIGH);

  //Give the sensor some time to get going
  delay(10);
  
  //Take the reading
  ledBrightness = constrain(map(analogRead(PIN_LDR), 200, 1023, BRIGHTNESS_MIN, BRIGHTNESS_MAX), BRIGHTNESS_MIN, BRIGHTNESS_MAX);

  //Serial Debug (if enabled)
  #ifdef DEBUG 
  Serial.print("Brightness: ");
  Serial.println(ledBrightness);
  #endif

  //Turn off the Sensor
  digitalWrite(PIN_LDR_ON, LOW);
}



/**
 * Calculate the LED colour based on the thresholds
 */
void updateLed() {
  if (currentTemp < TEMP_IDEAL_MIN) {
    setLed(0, 0, 255); //This porridge is too cold!
  } else if (currentTemp > TEMP_IDEAL_MAX) {
    setLed(255, 0, 0); //This porride is too hot!
  } else {
    setLed(0, 255, 0); //This porride is JUUUUUST right. :)
  }
}

