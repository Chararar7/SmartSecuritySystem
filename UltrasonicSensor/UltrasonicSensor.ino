#include <Ultrasonic.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

Ultrasonic ultrasonic(12, 13);

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int redPin = 8;
const int greenPin = 9;
const int bluePin = 110;
const int LED = 7;
const int buzzer = A2;

int distance;

//String password = "";  

void setup() {
  Serial.begin(9600);

  // initialize the LCD
  lcd.begin();

  pinMode(LED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.print("......");
}

void loop() {

  lcd.clear();

  // Pass INC as a parameter to get the distance in inches
  distance = ultrasonic.read();
  
  Serial.print("Distance in CM: ");
  Serial.println(distance);

  lcd.setCursor(0, 0); // top left
  lcd.print("Distance: ");
  lcd.print(distance);
  lcd.print("CM");

  if (distance >= 20)
  {
    setColor(0, 255, 0); //green
    noTone(buzzer);
  }

  else if (distance < 20 && distance >= 10 )
  {
    setColor(255, 255, 0); //yellow
    noTone(buzzer);
  }
  if (distance < 10)
  {
    setColor(255, 0, 0); //red
    
    Serial.println("Password Required.");

    lcd.setCursor(0, 1); // bottom left
    lcd.print("Give Password.");

    for (int i = 0; i < 3; i++)
    {
      if (checkPassword() == true)
      {
        break;
      }
      else {
        Serial.print("Attempt Nb ");
        Serial.println(i + 1);

        lcd.setCursor(0, 1); // bottom left
        lcd.print("Attempt Nb ");
        lcd.print(i + 1);
        lcd.print("    ");

        delay(1000);

        if (i == 2)
        {
          
          lcd.setCursor(0, 1); // bottom left
          lcd.print("Entry Denied !  ");

          while (true){
            AlertSystem();
          }
        }

        else
        {
          lcd.setCursor(0, 1); // bottom left
          lcd.print("Give Password.  ");
        }
      }
    }
  }
  delay(1000);
}

bool checkPassword()
{
  String password;

  while(Serial.available() == 0);

  password = Serial.readStringUntil('\n');

  if (password == "DoorPassword")
  {
    Serial.println("Welcome.");

    lcd.setCursor(0, 1); // bottom left
    lcd.print("Welcome!        ");

    delay(1000);

    return true;
  }
  else
  {
    Serial.println("Incorrect Password.");

    lcd.setCursor(0, 1); // bottom left
    lcd.print("Wrong Password.");

    delay(2000);
    
    return false;
  }
}

void AlertSystem()
{
  tone(buzzer, 800, 500);
  digitalWrite(LED, HIGH);
  delay(500);

  tone(buzzer, 900, 500);
  digitalWrite(LED, LOW);
  delay(500);
}

void setColor(int red, int green, int blue)
{
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}
