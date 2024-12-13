#include <Wire.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void setup() {
  Serial.begin(115200); 
  delay(1000);

 
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);  // Font
  u8g2.drawStr(0, 10, "OLED screen test!!"); 
  u8g2.sendBuffer();                  

  Serial.println("OLED Test ok");
}

void loop() {

}
