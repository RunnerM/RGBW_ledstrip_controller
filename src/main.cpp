#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#define ONBOARD_LED 2

char auth[] = "ZnABayXauLZkF4xKsn1gfcgXKnhY6Kfz";       // You should get Auth Token in the Blynk App.
char ssid[] = "SSID";                    // Your Wi-Fi Credentials
char pass[] = "PASSWORD";

int ledred = 13;
int ledgreen = 12;
int ledblue = 27;
int ledwhite =14;
int buttonInput = 4;

const int freq = 300;
const int ledChannelRed = 0;
const int ledChannelGreen = 1;
const int ledChannelBlue = 2;
const int ledChannelWhite = 3;
const int resolution = 8;

int ledState = LOW;
int btnState = HIGH;

BlynkTimer timer;
void checkPhysicalButton();

void checkPhysicalButton()
{
  if (digitalRead(buttonInput) == LOW) {
    // btnState is used to avoid sequential toggles
    if (btnState != LOW) {

      // Toggle LED state
      ledState = !ledState;
      //digitalWrite(, ledState);

      // Update Button Widget
      Blynk.virtualWrite(V4, ledState);
    }
    btnState = LOW;
  } else {
    btnState = HIGH;
  }
}

void setup() {
  //Blynk
  pinMode(ONBOARD_LED, OUTPUT);
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  int wifi_ctr = 0;
  while (WiFi.status() != WL_CONNECTED) {
    for (int i = 0; i <= 10; i++) {
      digitalWrite(ONBOARD_LED, HIGH);
      delay(20);
      digitalWrite(ONBOARD_LED, LOW);
      delay(20);
    }
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Blynk.begin(auth, ssid, pass);


  // configure LED PWM functionalitites
  ledcSetup(ledChannelRed, freq, resolution);
  ledcSetup(ledChannelGreen, freq, resolution);
  ledcSetup(ledChannelBlue, freq, resolution);
  ledcSetup(ledChannelWhite, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledred, ledChannelRed);
  ledcAttachPin(ledgreen, ledChannelGreen);
  ledcAttachPin(ledblue, ledChannelBlue);
  ledcAttachPin(ledwhite, ledChannelWhite);

  pinMode(buttonInput,INPUT_PULLDOWN);
  timer.setInterval(100L, checkPhysicalButton);
}

void loop() {
  Blynk.run();
}
//Red-Pin set by virtual pin1 
BLYNK_WRITE(V1) {
  int pinV1 = param.asInt();
  Serial.println("Redpin: " + pinV1);
  ledcWrite(ledChannelRed, pinV1);
}

BLYNK_WRITE(V2) {
  int pinV2 = param.asInt();
  Serial.println("Greenpin: " + pinV2);
  ledcWrite(ledChannelGreen, pinV2);
}

BLYNK_WRITE(V3) {
  int pinV3 = param.asInt();
  Serial.println("Bluepin: " + pinV3);
  ledcWrite(ledChannelBlue, pinV3);
}

BLYNK_WRITE(V4) {
  int pinV4 = param.asInt();
  Serial.println("Whitepin: " + pinV4);
  ledcWrite(ledChannelWhite, pinV4);
}
