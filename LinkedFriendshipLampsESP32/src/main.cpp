#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>

// Network Credentials
const char *ssid = "YOUR_WIFI_SSID"; // CHANGE TO YOUR WIFI SSID

const char *password = "YOUR_WIFI_PASSWORD"; // CHANGE TO YOUR WIFI PASSWORD

// MQTT BROKER
const char *mqtt_server = "YOUR_PUBLIC_MQTT_SERVER"; // CHANGE TO A PUBLICALLY ACCESSIBLE MQTT SERVER
const int mqtt_port = 1883;                          // Change to a different if needed
const char *mqtt_topic = "YOUR_MQTT_TOPIC";          // CHANGE TO YOUR MQTT TOPIC

// MQTT Init
WiFiClient espClient;
PubSubClient client(espClient);

// Device states
enum DeviceState
{
  IDLE,
  RED,
  BLUE,
  GREEN,
  YELLOW,
  ORANGE,
  PURPLE,
  INIT
};

// Make the state the command listening state, IDLE
DeviceState currentState = IDLE;

/*

DEFINE PINS HERE ---- v ---- v ---- v ----

*/

// OTHER LEDS
#define WIFI_LED 32
#define MQTT_LED 12

// LED STRIP
#define LED_PIN 27
#define NUM_LEDS 15
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// BUTTON
#define BUTTON_PIN 25
bool lastButtonState = HIGH;
bool buttonPressed = false;
unsigned long lastPressTime = 0;
const unsigned long POST_DELAY = 3000; // 5 seconds

// PREDEFINED COLORS
struct RGB
{
  uint8_t r, g, b;
};

uint32_t redRGB = strip.Color(255, 0, 0); // Start with red

// ====== COLOR STORAGE ======
uint32_t colors[] = {
    strip.Color(255, 0, 0),  // Red 0
    strip.Color(255, 50, 0), // Orange 1
    strip.Color(255, 94, 0), // Yellow 2
    strip.Color(38, 255, 0), // Green 3
    strip.Color(0, 0, 255),  // Blue 4
    strip.Color(144, 0, 255) // Violet 5
};
int colorIndex = 0;
uint32_t selectedColor = colors[colorIndex];

uint32_t initWhiteColor = strip.Color(255, 255, 255);

/*
DEFINE PINS HERE ---- ^ ---- ^ ---- ^ ----
*/

// METHODS -------------

// v v v  INIT METHODS  v v v

// Declaring function names at top to prevent build error
void connectWiFi();
void callback(char *topic, byte *payload, int length);
void setColor(uint32_t color);
void sendColorChangeCommand(String colorName);

void setup()
{
  // Init commands, such as starting the serial port, servers, and status LEDs
  Serial.begin(115200);
  pinMode(WIFI_LED, OUTPUT);
  pinMode(MQTT_LED, OUTPUT);
  connectWiFi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  // v v v Hardware Inits v v v
  strip.begin();
  strip.show();
  setColor(initWhiteColor);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

// Connects the ESP32 to WiFi -- The connection status is displayed on the WIFI_LED (on for connecting, off for connected)
void connectWiFi()
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(WIFI_LED, HIGH);
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  digitalWrite(WIFI_LED, LOW);
}

// If the device loses connection to the MQTT server then try to reconnect
// There is an optional status LED that will turn on if there are connection issues (it will turn off when it has been reconnected)
void reconnect()
{
  while (!client.connected())
  {
    // Status LED
    digitalWrite(MQTT_LED, HIGH);
    Serial.println("Attempting MQTT connection...");

    String clientId = "YOUR_UNIQUE_CLIENT_ID"; // CHANGE TO YOUR UNIQUE CLIENT ID FOR MQTT CONNECTION

    // Attempt connection
    if (client.connect(clientId.c_str()))
    {
      // Turn off the MQTT status light
      digitalWrite(MQTT_LED, LOW);

      // Set the INIT color (white) to tell the other device that it just reconnected back to the network
      sendColorChangeCommand("INIT");
      currentState = INIT;

      // Print status and subscribe
      Serial.println("Connected to MQTT broker");
      client.publish("mqtt_topic", "ESP32 connected!");
      client.subscribe(mqtt_topic);
    }
    else
    {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      digitalWrite(WIFI_LED, HIGH);
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// ^ ^ ^ INIT METHODS ^ ^ ^

// v v v ACTIONS v v v

// Sends a color to the other device
void sendColorChangeCommand(String colorName)
{
  if (client.connected())
  {
    client.publish(mqtt_topic, colorName.c_str());
    Serial.println("Published color command to MQTT server");
  }
  else
  {
    Serial.println("Failed to publish: MQTT client not connected");
  }
}

// For testing purposes -> this is used in order to verify that the MQTT is working and a response is given
void publishResponse()
{
  if (client.connected())
  {
    client.publish(mqtt_topic, "COMPLETED COLOR CHANGE");
    Serial.println("Published response to MQTT server");
  }
  else
  {
    Serial.println("Failed to publish: MQTT client not connected");
  }
}

// Sets a color for the whole LED strip
void setColor(uint32_t color)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

// ^ ^ ^ ACTIONS ^ ^ ^

// v v v COLOR FUNCTIONS (using state machine) v v v

void changeToRed()
{

  Serial.println("Changing color to RED");

  // v v v Enter action below v v v

  setColor(colors[0]);

  // ^ ^ ^ Action is located above ^ ^ ^

  Serial.println("Changed to RED");

  // Uncomment for testing purposes
  // publishResponse();

  currentState = IDLE;
}

void changeToOrange()
{
  Serial.println("Changing color to ORANGE");

  // v v v Enter action below v v v

  setColor(colors[1]);

  // ^ ^ ^ Action is located above ^ ^ ^

  Serial.println("Changed to ORANGE");

  // Uncomment for testing purposes
  // publishResponse();

  currentState = IDLE;
}

void changeToYellow()
{
  Serial.println("Changing color to YELLOW");

  // v v v Enter action below v v v

  setColor(colors[2]);

  // ^ ^ ^ Action is located above ^ ^ ^

  Serial.println("Changed to YELLOW");

  // Uncomment for testing purposes
  // publishResponse();

  currentState = IDLE;
}

void changeToGreen()
{
  Serial.println("Changing color to GREEN");

  // v v v Enter action below v v v

  setColor(colors[3]);

  // ^ ^ ^ Action is located above ^ ^ ^

  Serial.println("Changed to GREEN");

  // Uncomment for testing purposes
  // publishResponse();

  currentState = IDLE;
}

void changeToBlue()
{
  Serial.println("Changing color to BLUE");

  // v v v Enter action below v v v

  setColor(colors[4]);

  // ^ ^ ^ Action is located above ^ ^ ^

  Serial.println("Changed to BLUE");

  // Uncomment for testing purposes
  // publishResponse();

  currentState = IDLE;
}

void changeToPurple()
{
  Serial.println("Changing color to PURPLE");

  // v v v Enter action below v v v

  setColor(colors[5]);

  // ^ ^ ^ Action is located above ^ ^ ^

  Serial.println("Changed to PURPLE");

  // Uncomment for testing purposes
  // publishResponse();

  currentState = IDLE;
}

void goToInitColor()
{
  Serial.println("Changing color to IDLE COLOR");

  // v v v Enter action below v v v

  setColor(initWhiteColor);

  // ^ ^ ^ Action is located above ^ ^ ^

  Serial.println("Changed to IDLE COLOR");

  // Uncomment for testing purposes
  // publishResponse();

  currentState = IDLE;
}

// ^ ^ ^ COLOR FUNCTIONS (using state machine) ^ ^ ^

// v v v LOOPING v v v

// Checks to see if the button was pressed. If it was, then the color index is raised, thus cycling through the list of colors.
void checkButton()
{
  bool currentState = digitalRead(BUTTON_PIN);

  // Detect falling edge (button press)
  if (lastButtonState == HIGH && currentState == LOW)
  {
    colorIndex = (colorIndex + 1) % (sizeof(colors) / sizeof(colors[0]));
    selectedColor = colors[colorIndex];
    setColor(selectedColor);
    lastPressTime = millis();
    buttonPressed = true;
    Serial.println("Button pressed!");
  }

  lastButtonState = currentState;
}

// Receives messages from MQTT broker address and decodes them
void callback(char *topic, byte *payload, int length)
{
  String message;
  Serial.print("Message arrived ->");
  for (int i = 0; i < length; i++)
  {
    message += (char)payload[i];
  }
  Serial.println(message);

  if (message == "RED")
  {
    currentState = RED;
  }
  if (message == "ORANGE")
  {
    currentState = ORANGE;
  }
  if (message == "YELLOW")
  {
    currentState = YELLOW;
  }
  if (message == "GREEN")
  {
    currentState = GREEN;
  }
  if (message == "BLUE")
  {
    currentState = BLUE;
  }
  if (message == "PURPLE")
  {
    currentState = PURPLE;
  }
  if (message == "INIT")
  {
    currentState = INIT;
  }
}

void loop()
{
  // Make sure we are still connected to the MQTT client
  if (!client.connected())
  {
    reconnect();
  }

  // loop the MQTT client
  client.loop();

  // Check the button
  checkButton();

  // Check to see if the button was pressed ->
  // If it hasn't been pressed in the POST_DELAY time, then send the MQTT message to change the light to that color.
  String colorToChangeTo;
  if (buttonPressed && (millis() - lastPressTime > POST_DELAY))
  {
    if (selectedColor == colors[0]) // RED
    {
      colorToChangeTo = "RED";
      currentState = RED;
    }
    if (selectedColor == colors[1]) // ORANGE
    {
      colorToChangeTo = "ORANGE";
      currentState = ORANGE;
    }
    if (selectedColor == colors[2]) // YELLOW
    {
      colorToChangeTo = "YELLOW";
      currentState = YELLOW;
    }
    if (selectedColor == colors[3]) // GREEN
    {
      colorToChangeTo = "GREEN";
      currentState = GREEN;
    }
    if (selectedColor == colors[4]) // BLUE
    {
      colorToChangeTo = "BLUE";
      currentState = BLUE;
    }
    if (selectedColor == colors[5]) // PURPLE
    {
      colorToChangeTo = "PURPLE";
      currentState = PURPLE;
    }
    sendColorChangeCommand(colorToChangeTo);
    buttonPressed = false;
  }

  // Command state machine to switch the colors
  switch (currentState)
  {
  case IDLE:
    // Listen for MQTT Commands in IDLE state
    break;

  case RED:
    changeToRed();
    break;

  case ORANGE:
    changeToOrange();
    break;

  case YELLOW:
    changeToYellow();
    break;

  case GREEN:
    changeToGreen();
    break;

  case BLUE:
    changeToBlue();
    break;

  case PURPLE:
    changeToPurple();
    break;

  case INIT:
    goToInitColor();
    break;

  default:
    Serial.println("");
    break;
  }
}

// ^ ^ ^ LOOPING ^ ^ ^