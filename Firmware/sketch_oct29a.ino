#include <Adafruit_GFX.h> // graphics library
#include <Adafruit_ST7789.h> // driver for the ST7789 screen
#include <SPI.h> 

// Defining pins for the display, change according to your setup!!! Uses the white numbers on the ESP
#define TFT_SCLK 0 // labeled SCL on the screen
#define TFT_MOSI 1 // labeled SDA on the screen
#define TFT_RST 2
#define TFT_DC 3
#define TFT_CS 4
#define TFT_BL 5

int timeHour = 12;
int timeMin = 59;
int timeSec = 59;

int clockType = 12;

unsigned long previousMillis = 0;
const long oneSec = 1000;

// Fix setColRowStart() by exposing it via a subclass
class MyST7789 : public Adafruit_ST7789 {
public:
  MyST7789(int8_t cs, int8_t dc, int8_t mosi, int8_t sclk, int8_t rst)
    : Adafruit_ST7789(cs, dc, mosi, sclk, rst) {}
  void setOffsets(uint8_t col, uint8_t row) {
    _colstart = _colstart2 = col;
    _rowstart = _rowstart2 = row;
  }
};

MyST7789 tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void printTime() {

  String text = "";
  
  if (timeHour < 10) text += "0";
  text += String(timeHour) + ":";
  
  if (timeMin < 10) text += "0";
  text += String(timeMin) + ":";
  
  if (timeSec < 10) text += "0";
  text += String(timeSec);

  Serial.println(text); 

  int16_t x1, y1;
  uint16_t textWidth, textHeight;

  tft.getTextBounds(text, 0, 0, &x1, &y1, &textWidth, &textHeight);

  int xPos = 142 - (textWidth / 2);
  int yPos = 38 - (textHeight / 2);

  Serial.println(text); 

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(6);
  tft.setCursor(xPos, yPos);
  tft.print(text);
}

// setup() runs ONCE when the board powers on
void setup() {
  Serial.begin(115200); // lets the board talk to your computer

  pinMode(TFT_BL, OUTPUT); // Set the backlight pin mode, or just wire it to 3.3V
  digitalWrite(TFT_BL, LOW); // Turns the backlight ON, for some reason this screen is active Low, so setting it to LOW is really HIGH

  tft.init(76, 284); // Our panel size (portrait)
  tft.setOffsets(82, 18); // Offsets for the weird resolution
  tft.invertDisplay(false); // Invert the colors (This display is flipped from normal)
  tft.setRotation(1); // Landscape, if it's upside down use 3!
  Serial.println("TFT Initialized!");

  tft.fillScreen(ST77XX_BLACK); // clear the screen

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(6);
  tft.setCursor(0,0); // Where the text is drawn, 0,0 is top left
  tft.print("12:00"); // Show whatever you want! Draws from the top left of the text/number/shape 
}

// loop() runs OVER and OVER, forever
void loop() {
  unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= oneSec) {
      previousMillis = currentMillis;

      timeSec++;

      if (timeSec >= 60) {
        timeSec = 0;
        timeMin++;

        if (timeMin >= 60) {
          timeMin = 0;
          timeHour++;

          if (timeHour > clockType) {
            timeHour = 1;
          }
        }
      }



  tft.fillScreen(ST77XX_BLACK);

  printTime();

  }
}
