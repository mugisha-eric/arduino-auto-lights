
# Arduino UTO Lights

An Arduino project for automatic lighting control using NodeMCU, ultrasonic sensors, bulbs, a relay, and a breadboard. This project aims to turn on the lights automatically when someone enters a room and turn them off when the room is empty.

![Project Image](https://github.com/mugisha-eric/arduino-auto-lights/blob/main/R09A0041.JPG?raw=true)

## Features

- Automatic light control based on room occupancy
- Energy-saving by turning off lights when the room is empty
- Easy to set up with detailed instructions and code

## Hardware Components

![Components](https://github.com/mugisha-eric/arduino-auto-lights/blob/main/R09A0042.JPG?raw=true)

- NodeMCU
- Ultrasonic sensor
- Light bulbs
- Relay module
- Breadboard
- Jumper wires

## Circuit Diagram

![Circuit Diagram](https://github.com/mugisha-eric/arduino-auto-lights/blob/main/IMG_0012.JPG?raw=true)

## Sample Code

```cpp
#include <Ultrasonic.h>
#include <ESP8266WiFi.h>

#define TRIG_PIN D1
#define ECHO_PIN D2
#define RELAY_PIN D5

Ultrasonic ultrasonic(TRIG_PIN, ECHO_PIN);

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF); // Disable WiFi to save power
}

void loop() {
  long distance = ultrasonic.Ranging(CM);

  if (distance < 100) { // Adjust the distance threshold as needed
    digitalWrite(RELAY_PIN, HIGH); // Turn on the light
  } else {
    digitalWrite(RELAY_PIN, LOW); // Turn off the light
  }

  delay(1000); // Adjust the delay as needed
}
```

## Developers

- **Irasubiza Tharcisse** - [GitHub Profile](https://github.com/irasubiza-tharcisse)
- **Mugisha Eric** - [GitHub Profile](https://github.com/mugisha-eric)
- **Gwiza Elvis** - [Github Profile](https://github.com/burnaghost)
- **Iradufasha Gratien** - [Github Profile](https://github.com/iradufasha001)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

![MIT License](https://www.svgrepo.com/show/444064/legal-license-mit.svg)
