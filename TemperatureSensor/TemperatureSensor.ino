#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device address
DeviceAddress insideThermometer;

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

float temp;
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;
const int LED = 7;
const int buzzer = A2;

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");

  // initialize the LCD
  lcd.begin();

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(LED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // locate devices on the bus
  Serial.print("Locating devices...");
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0"); 
  
  // show the addresses we found on the bus
  Serial.print("Device 0 Address: ");
  printAddress(insideThermometer);
  Serial.println();

  // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
  sensors.setResolution(insideThermometer, 9);
 
  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(insideThermometer), DEC); 
  Serial.println();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.print("Welcome!");
}

// function to print the temperature for a device
float printTemperature(DeviceAddress deviceAddress)
{
  // method 2 - faster
  float tempC = sensors.getTempC(deviceAddress);
  if(tempC == DEVICE_DISCONNECTED_C) 
  {
    Serial.println("Error: Could not read temperature data");
    return;
  }
  Serial.print("Temp C: ");
  Serial.print(tempC);
  Serial.print(" Temp F: ");
  Serial.println(DallasTemperature::toFahrenheit(tempC)); // Converts tempC to Fahrenheit
  lcd.setCursor(0, 0); // top left
  lcd.print("Temp C: ");
  lcd.print(tempC);
  lcd.setCursor(0, 1); // bottom left
  lcd.print("Temp F: ");
  lcd.print(DallasTemperature::toFahrenheit(tempC));

  return tempC;
}
/*
 * Main function. It will request the tempC from the sensors and display on Serial.
 */
void loop(void)
{ 
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");

  lcd.clear();
  
  // It responds almost immediately. Let's print out the data
  temp = printTemperature(insideThermometer); // Use a simple function to print out the data

  delay(1000);

  if (temp <=32)
  {
    setColor(0, 255, 0); //green
    noTone(buzzer);
  }
  else if (temp > 32 && temp <= 40)
  {
    setColor(255, 255, 0); //yellow
    noTone(buzzer);
  }
  else if (temp > 40)
  {
    setColor(255, 0, 0); //red

    tone(buzzer, 800);
    digitalWrite(LED, HIGH);
    delay(500);

    tone(buzzer, 900);
    digitalWrite(LED, LOW);
    delay(500);
  }

}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

void setColor(int red, int green, int blue)
{
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

