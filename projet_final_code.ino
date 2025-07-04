#include "HX711.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int LOADCELL_DOUT = D5;
const int LOADCELL_SCK = D6;

HX711 scale;
float calibration_factor = 1820; 
float units; 
int ounces;

void setup()
{
  Serial.begin(115200);
  Wire.begin(D2, D1);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Circuit Digest");
  lcd.setCursor(0, 1);
  lcd.print("NodeMCU Wg Scale");

  delay(1000);

  lcd.clear();

  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on the scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");

  scale.begin(LOADCELL_DOUT, LOADCELL_SCK);
  scale.set_scale(calibration_factor); 
  scale.tare();  
}

void loop()
{
  
  units = scale.get_units(10); 


  if (units < 0)
  {
    units = 0;
  }
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Weight: ");
  lcd.print(units, 2); 
  lcd.print(" g");

  Serial.print("Weight: ");
  Serial.print(units, 2);
  Serial.println(" grams");

  if (Serial.available())
  {
    char temp = Serial.read();
    if (temp == '+' || temp == 'a')
      calibration_factor += 1;
    else if (temp == '-' || temp == 'z')
      calibration_factor -= 1;

    scale.set_scale(calibration_factor);
  }
}
