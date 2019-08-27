#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BH1750.h>

BH1750 lightMeter;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     LED_BUILTIN //4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const uint8_t LightStrengthDetectFrequency = 1000; // in ms

void setup() {
  Serial.begin(115200);
  Serial.println(" ");
  
  Serial.println("Initializing OLED screen...");
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  
  Serial.println("Done.");
  delay(500); // Pause for 500 ms

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white for debug
  display.drawPixel(10, 10, WHITE);
  display.display();
  delay(1000); // Pause for 1s

  Serial.println("Initializing BH1750...");
  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  // On esp8266 devices you can select SCL and SDA pins using Wire.begin(D4, D3);
  Wire.begin();

  lightMeter.begin();
  Serial.println("Done");

  Serial.println("All Initialization Finished.");
}


String str_start = "Lux: ";

void loop() {
  display.clearDisplay();
  uint16_t lux = lightMeter.readLightLevel();
  
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lux");

  String str_out = str_start + String(lux);
  OLED_StringPrint(str_out);


  display.display();
  delay(LightStrengthDetectFrequency);
}

void OLED_StringPrint (String _data)
{
  display.setTextSize(2.5);
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.println(_data);
}
