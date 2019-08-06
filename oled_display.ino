#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

void initDisplay() 
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void showDataOnOled(float temp_f, float temp_c, float humidity) 
{
    // text display tests
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Temp. C:");
  display.print(temp_c);
  display.setCursor(0,12);
  display.print("Temp. F:");
  display.print(temp_f);
  display.setCursor(0,24);
  display.print("Humidity %: "); 
  display.print(humidity);
  display.display(); // actually display all of the above
}
