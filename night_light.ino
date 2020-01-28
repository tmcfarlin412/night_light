int ON_OFF_SWITCH = 2;
#define RED 3
#define GREEN 5
#define BLUE 6
int MINUS_BUTTON = 4;
int PLUS_BUTTON = 7;
bool isPlusPressed;
bool isMinusPressed;
int MODE_BRIGHTNESS = 0;
int MODE_RED = 1;
int mode = MODE_BRIGHTNESS;
float brightnessFactor = 0.5;
float brightnessDelta = 0.1;
int brightnessValues[10] = {0, 30, 60, 90, 120, 150, 180, 210, 240, 255};
int rednessValues[10] = {0, 1, 3, 6, 15, 30, 50, 80, 130, 255};
int brightnessIndex = 3;
int rednessIndex = 0;
int brightnessIndexMax = 9;
int rednessIndexMax = 9;
int DELAY_AFTER_UPDATE = 500;
bool flagModeSwitched;
bool isOn = true;
bool wasOn = false;

void setup() {
  Serial.begin(9600);
  pinMode(ON_OFF_SWITCH, INPUT);
  pinMode(MINUS_BUTTON, INPUT_PULLUP);
  pinMode(PLUS_BUTTON, INPUT_PULLUP);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  updateRGBLED();
}

void loop() {
  isMinusPressed = digitalRead(MINUS_BUTTON) == LOW;
  isPlusPressed = digitalRead(PLUS_BUTTON) == LOW;
  //isOn = digitalRead(ON_OFF_SWITCH) == HIGH;
  
  if (isOn) {
    if (!wasOn) {
      updateRGBLED();
      wasOn = true;
    } else if (isMinusPressed && isPlusPressed) {
      mode = (mode + 1) % 2;  // Toggle mode
      Serial.print("Mode switched to ");
      Serial.println(mode);

      // Loop until buttons are released
      while (isMinusPressed && isPlusPressed) {
        delay(100);
        isMinusPressed = digitalRead(MINUS_BUTTON) == LOW;
        isPlusPressed = digitalRead(PLUS_BUTTON) == LOW;
      }
    } else if (isMinusPressed) {
      Serial.println("Minus");
      if (mode == MODE_BRIGHTNESS && brightnessIndex > 0) {
        brightnessIndex -= 1;// brightnessIndex - 1;
      } else if (mode == MODE_RED && rednessIndex > 0) {
        rednessIndex -= 1;   
      }
      updateRGBLED();
      delay(DELAY_AFTER_UPDATE);
    } else if (isPlusPressed) {
          Serial.println("Plus");
      if (mode == MODE_BRIGHTNESS && brightnessIndex < brightnessIndexMax) {
        brightnessIndex += 1;// brightnessFactor + brightnessDelta;
      } else if (mode == MODE_RED && rednessIndex < rednessIndexMax) {
        rednessIndex += 1;//redFactor + redDelta;      
      }
      updateRGBLED();
      delay(DELAY_AFTER_UPDATE);
    }
  } else {
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);
    wasOn = false;
  }
}

void updateRGBLED() {
  float brightness = brightnessValues[brightnessIndex];
  float greenBlue = rednessValues[rednessIndex];//brightnessFactor*(1.0 - redFactor)*(1.0 - redFactor)*(1.0 - redFactor)*255;
  Serial.print("Brightness: ");
  Serial.print(brightness);
  Serial.print(" | greenBlueness: ");
  Serial.println(greenBlue);
  analogWrite(RED, brightness);
  analogWrite(GREEN, greenBlue);
  analogWrite(BLUE, greenBlue);
}
