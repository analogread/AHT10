/***************************************************************************************************/
/*
   This is an Arduino library for Aosong ASAIR AHT10, AHT15 Digital Humidity & Temperature Sensor

   written by : enjoyneering79
   sourse code: https://github.com/enjoyneering/

   This chip uses I2C bus to communicate, specials pins are required to interface
   Board:                                    SDA                    SCL                    Level
   Uno, Mini, Pro, ATmega168, ATmega328..... A4                     A5                     5v
   Mega2560................................. 20                     21                     5v
   Due, SAM3X8E............................. 20                     21                     3.3v
   Leonardo, Micro, ATmega32U4.............. 2                      3                      5v
   Digistump, Trinket, ATtiny85............. 0/physical pin no.5    2/physical pin no.7    5v
   Blue Pill, STM32F103xxxx boards.......... PB7                    PB6                    3.3v/5v
   ESP8266 ESP-01........................... GPIO0/D5               GPIO2/D3               3.3v/5v
   NodeMCU 1.0, WeMos D1 Mini............... GPIO4/D2               GPIO5/D1               3.3v/5v
   ESP32.................................... GPIO21/D21             GPIO22/D22             3.3v

   Frameworks & Libraries:
   ATtiny  Core          - https://github.com/SpenceKonde/ATTinyCore
   ESP32   Core          - https://github.com/espressif/arduino-esp32
   ESP8266 Core          - https://github.com/esp8266/Arduino
   STM32   Core          - https://github.com/stm32duino/Arduino_Core_STM32
                         - https://github.com/rogerclarkmelbourne/Arduino_STM32


   GNU GPL license, all text above must be included in any redistribution,
   see link for details  - https://www.gnu.org/licenses/licenses.html
*/
/***************************************************************************************************/
#include <AHT10.h>
#include <Wire.h>
#define LED 3
float Temperature;
float Humidity;
bool state = true ;
uint8_t readStatus = 0;

AHT10 myAHT10(AHT10_ADDRESS_0X38);


void setup()
{
  Serial.begin(9600);
  Serial.println();
  pinMode(LED, OUTPUT);
  while (myAHT10.begin() != true)
  {
    Serial.println(F("AHT10 not connected or fail to load calibration coefficient")); //(F()) save string to flash & keeps dynamic memory free
    delay(5000);
  }
  Serial.println(F("\t\tAHT10 OK"));
  Wire.setClock(400000); //experimental I2C speed! 400KHz, default 100KHz
}


void loop()
{
  readStatus = myAHT10.readRawData(); //read 6 bytes from AHT10 over I2C
  if (readStatus != AHT10_ERROR)
  {
    if (state == true)
    {Serial.print(F("Humidity\t\t"));
    Serial.print(F("Temperature \n")); 
    state = false;
    }
    Temperature = myAHT10.readTemperature(AHT10_USE_READ_DATA);
    Humidity = myAHT10.readHumidity(AHT10_USE_READ_DATA);
    Serial.print(Humidity); Serial.print(F(" +-0.3C\t\t"));
    Serial.print(Temperature);    Serial.print(F(" +-2%"));


    if (Temperature >= 30) {
      digitalWrite(LED, HIGH);
      Serial.print("  : เปิดไฟ \n");
    } else {
      digitalWrite(LED, LOW);
      Serial.print("  : ปิดไฟ \n");
    }



  }
  else
  {
    Serial.print(F("Failed to read - reset: "));
    Serial.println(myAHT10.softReset());         //reset 1-success, 0-failed
    Serial.println("******************************");         //reset 1-success, 0-failed
  }
  delay(100); //recomended polling frequency 8sec..30sec






}
