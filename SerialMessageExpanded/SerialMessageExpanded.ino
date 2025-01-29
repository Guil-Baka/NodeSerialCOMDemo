#include <Adafruit_SSD1306.h>
#include <splash.h>

#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 32     // OLED display height, in pixels
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const byte numChars = 32;

char receivedChars[numChars];

String complicatedData[4];

boolean newData = false;

// if i receive data that uses separators, i need to split the data
// and store it in an array of strings

void receiveComplicatedData(){
  static boolean receiving = false;
  static byte ndx = 0;
  char startInterval = '<';
  char endInterval = '>';
  char separator = ',';

  char message;

  while (Serial.available() > 0 && newData == false) {
    message = Serial.read();

    if (receiving == true) {
      if (message != endInterval) {
        receivedChars[ndx] = message;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      } else {
        receivedChars[ndx] = '\0';
        receiving = false;
        ndx = 0;
        newData = true;
      }
    } else if (message == startInterval) {
      receiving = true;
    }
  }

  // if i have received data, i need to split it
  if(newData == true){
    char *token = strtok(receivedChars, ",");
    int i = 0;
    while(token != NULL){
      complicatedData[i] = token;
      token = strtok(NULL, ",");
      i++;
    }
  }
}

void receiveInterval() {
  static boolean receiving = false;
  static byte ndx = 0;
  char startInterval = '<';
  char endInterval = '>';

  char message;

  while (Serial.available() > 0 && newData == false) {
    message = Serial.read();

    if (receiving == true) {
      if (message != endInterval) {
        receivedChars[ndx] = message;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      } else {
        receivedChars[ndx] = '\0';
        receiving = false;
        ndx = 0;
        newData = true;
      }
    } else if (message == startInterval) {
      receiving = true;
    }
  }
}

void showNewData() {
  if (newData == true) {
    Serial.print("Data: ");
    Serial.println(receivedChars);
    newData = false;
  }
}

void showNewDataOled() {
  if (newData == true) {
    resetCursorPos();
    display.clearDisplay();
    display.print("Data: ");
    display.println(receivedChars);
    display.display();
    newData = false;
  }
}

void showNewDataOledAndSerial() {
  if (newData == true) {
    resetCursorPos();
    display.clearDisplay();
    display.print("Data: ");
    display.println(receivedChars);
    display.display();
    Serial.print("Data: ");
    Serial.println(receivedChars);
    newData = false;
  }
}

void showComplicatedData(){
  if(newData == true){
    resetCursorPos();
    display.clearDisplay();
    display.print("Data: ");
    for(int i = 0; i < 4; i++){
      display.print(complicatedData[i]);
      display.print(" ");
      Serial.print("Data: ");
      Serial.print(complicatedData[i]);
      Serial.print(" ");
    }
    display.display();
    newData = false;
  }
}

void shineLEDS(unsigned int interval) {
  Serial.println("LED ON ");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(interval);
  Serial.println("LED OFF ");
  digitalWrite(LED_BUILTIN, LOW);
  delay(interval);
}

void resetCursorPos(){
  display.setCursor(0, 0);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  //display.begin(SSD1306_EXTERNALVCC, 0x3C);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.ssd1306_command(SSD1306_SETCONTRAST);
  display.ssd1306_command(0x81);
  display.ssd1306_command(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.setRotation(2);
  display.setTextSize(1);
  display.setCursor(0, 0);

  display.println("Ready to receive!");
  display.display();

  delay(2000);
  Serial.println("Ready to receive!");
}

void loop() {
  // put your main code here, to run repeatedly:
  // receiveInterval();
  receiveComplicatedData();
  // showNewDataOledAndSerial();
  showComplicatedData();
  // fakePWM(random(0, 1000));
  
}
