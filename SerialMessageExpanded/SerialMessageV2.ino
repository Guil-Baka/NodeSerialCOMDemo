#include <Adafruit_SSD1306.h>
#include <splash.h>

#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 32     // OLED display height, in pixels
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String separatedData[10];
const byte numChars = 32;
char receivedChars[numChars];
bool newData = false;

void receiveWithEndMarker(){
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;
  
  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();
    
    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}

void showNewData(){
  if(newData == true){
    Serial.print("This just in ... ");
    Serial.println(receivedChars);
    Serial.print("Separated data: ");
    for(int i = 0; i < 10; i++){
      Serial.print(separatedData[i]);
      Serial.print(" \n");
    }
    Serial.println();
    newData = false;
  }
}

// this function will receive the receivedChars array and split it every time it finds a comma and store it in the separatedData array

void splitData(){
  char *token = strtok(receivedChars, ",");
  int i = 0;
  while(token != NULL){
    separatedData[i] = token;
    token = strtok(NULL, ",");
    i++;
  }
}

void alternateLeds(String ledState[]){
  if (ledState[0] == "1"){
    //TX high
    digitalWrite(LED_BUILTIN_TX, HIGH);
  }
  else{
    //TX low
    digitalWrite(LED_BUILTIN_TX, LOW);
  }
  if (ledState[1] == "1"){
    //RX high
    digitalWrite(LED_BUILTIN_RX, HIGH);
  }
  else{
    //RX low
    digitalWrite(LED_BUILTIN_RX, LOW);
  }
}

void endTransmission(String state[]0{
  // if pos 2 is 0 serial print "EOL"
  if (state[2] == "0"){
    Serial.println("EOL");
  }
})

void setup(){
  Serial.begin(115200);

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

void loop(){
  receiveWithEndMarker();
  splitData();
  showNewData();
  alternateLeds(separatedData);
}