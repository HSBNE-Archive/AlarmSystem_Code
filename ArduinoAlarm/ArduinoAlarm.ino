/*
 * ATI  = Get Current State
 * ATS1 = Toggle Alarm Pin
 * ATS2 = RED LED
 * ATS3 = YELLOW LED
 * ATS4 = GREEN LED
 *
 * For LED'S
 * ATS2=1 Turns Red on
 * ATS2=0 Turns Red off
 */


#include <SoftwareSerial.h>

#define ALARM_PIN  12
#define LED_RED    9
#define LED_YELLOW 10
#define LED_GREEN  11

#define ARM_DELAY  2000

SoftwareSerial mySerial(3, 4); // RX, TX
char incomingByte = 0;
char buffer[10];
int  bufferInc=0;
unsigned long ArmSystem=0;
boolean pinOn = false;

void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // set the data rate for the SoftwareSerial port
  mySerial.begin(2400);
  pinMode(ALARM_PIN, OUTPUT);
  digitalWrite(ALARM_PIN, HIGH); 
  
  pinMode(LED_RED, OUTPUT);
  digitalWrite(LED_RED, LOW);
  pinMode(LED_YELLOW, OUTPUT);
  digitalWrite(LED_YELLOW, LOW);
  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_GREEN, LOW);
  
  Serial.println("ARDUINO: Starting..");
}

void loop() // run over and over
{
  if (mySerial.available())
  {
    Serial.write(mySerial.read());
  }
    
  if (Serial.available())
  {
      incomingByte = Serial.read();
      if (incomingByte != '\n' && incomingByte != '\r')
      {
        if(bufferInc < 10)
        {
          buffer[bufferInc++] = incomingByte;
        }
        else
        {
          bufferInc = 0;
        }
      }
      else
      {
        // Check for the first two chars AT
        if (bufferInc > 2 && (buffer[0] == 'a' || buffer[0] == 'A') && (buffer[1] == 't' || buffer[1] == 'T'))
        {
          // Is the next Char a I?
          if(buffer[2] == 'i' || buffer[2] == 'I')
          {
            Serial.print("ARDUINO: ALARM_PIN - ");
            Serial.print(digitalRead(ALARM_PIN) == LOW);
            Serial.print(" - ");
            Serial.print(ArmSystem);
            Serial.print(" - ");
            Serial.println(pinOn);
            Serial.print("ARDUINO: LED_RED - ");
            Serial.println(digitalRead(LED_RED) == HIGH);
            Serial.print("ARDUINO: LED_YELLOW - ");
            Serial.println(digitalRead(LED_YELLOW) == HIGH);
            Serial.print("ARDUINO: LED_GREEN - ");
            Serial.println(digitalRead(LED_GREEN) == HIGH);
          }
          else if((buffer[2] == 's' || buffer[2] == 'S') && bufferInc > 3)
          {
            if(buffer[3] == '1')
            {
              // Trigger a Arm/Disarm..
              digitalWrite(ALARM_PIN, LOW);
              ArmSystem = millis();
              pinOn = true;
              Serial.println("ARDUINO: ALARM_TOGGLE");
            }
            else if(buffer[4] == '=' &&  bufferInc > 5)
            {
              switch (buffer[3])
              {
                // Red LED
                 case '2':
                     if(buffer[5] == '1')
                     {
                       digitalWrite(LED_RED, HIGH);
                     }
                     else
                     {
                       digitalWrite(LED_RED, LOW);
                     }
                     break;
                 // YELLOW LED
                 case '3':
                     if(buffer[5] == '1')
                     {
                       digitalWrite(LED_YELLOW, HIGH);
                     }
                     else
                     {
                       digitalWrite(LED_YELLOW, LOW);
                     }
                     break;
                 // GREEN LED
                 case '4':
                     if(buffer[5] == '1')
                     {
                       digitalWrite(LED_GREEN, HIGH);
                     }
                     else
                     {
                       digitalWrite(LED_GREEN, LOW);
                     }
                     break;
              }
            }
          }
        }
        
        bufferInc = 0;
      }
  }
  
  if ((millis() > ArmSystem + ARM_DELAY) && pinOn)
  {
    digitalWrite(ALARM_PIN, HIGH);    // turn the LED off by making the voltage LOW
    pinOn = false;
  }
}

