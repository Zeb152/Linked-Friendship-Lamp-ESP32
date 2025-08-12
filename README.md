# Linked-Friendship-Lamp-ESP32
The short description of this project is that they are synced LED lamps for long-distance relationships using the ESP32.

## About
The so-called "Friendship Lamps" are devices with LEDs that sync from anywhere with WiFi. They can rotate between the different colors of the rainbow at the press of a button. After a color is selected, it is sent to the other device as a command to change to that same color -- linking the lamps. It is a two-way form of communication.

Many people set the colors as a code to mean certain things. For example:
- **Red:** I'm angry
- **Blue:** I'm sad
- **Green:** I miss you

Etc.

## How it works

On boot, the ESP32 immediately tries to connect to the Wifi that is preprogrammed in. On connection to the Wifi and MQTT server, all the LEDs turn white. This is to indicate a successful connection. After the LEDs turn white, the user can then press the button to move between different colors. The available colors are red, orange, yellow, green, blue, and purple. Once the user finds a color they want, they simply must stop pressing the button to 'choose' that color. To make sure the device doesn't send a ton of messages through the MQTT server (causing a risk of being blocked by the server), the device waits for 3 seconds for the button to stop being pressed. If the button isn't pressed after 3 seconds, the color is sent to the other device. This is to limit the amount of 'spam' on the server and also prevent a bunch of different colors flashing quickly on the other person's device.

## Hardware

This project is powered by an ESP32 (wroom). It is connected to a 3-pin addressable LED strip, a button, and a separate red LED (connected between a 220Ω resistor).

I tested all the hardware on a breadboard before permanently soldering it together.

