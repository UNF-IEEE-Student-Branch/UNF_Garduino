#include <DHT.h>
#include <LiquidCrystal.h>

#define Type DHT11
int sensePin = 2;
int rs = 7;
int en = 8;
int d4 = 9;
int d5 = 10;
int d6 = 11;
int d7 = 12;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
DHT HT(sensePin,Type);
float humidity;
float tempC;
float tempF;
int setTime = 500;
int dt = 1000;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  HT.begin();
  delay(setTime);
  lcd.begin(16,2);
}

void loop() {
  // put your main code here, to run repeatedly:
  humidity = HT.readHumidity();
  tempC = HT.readTemperature();
  tempF = HT.readTemperature(true);

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" Temperature ");
  Serial.print(tempC);
  Serial.print(" C ");
  Serial.print(tempF);
  Serial.println(" F ");
  delay(dt);

  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.setCursor(7,0);
  lcd.print(tempF);
  lcd.setCursor(0,7);
  lcd.print("Humid: ");
  lcd.setCursor(7,7);
  lcd.print(humidity);
 
}
