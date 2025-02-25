
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

![MIT License](https://www.bing.com/images/search?q=mit%20license%20icon&view=detailv2&FORM=IQFRBA&id=61A74DE41EF97E8F4CA9C5868A0AB96AD79A8D34&selectedindex=0&&expw=1024&exph=1024&ccid=7ezd6%2b7A&ck=192EDC7B65C1E780D272D463AEA8952A&simid=607989116135431111&thid=OIP.7ezd6-7ATqvDElDim9M2WwHaHa&idpp=serp&idpbck=1)


Feel free to customize the links and images according to your needs! If you have any specific questions or need further assistance, I'm here to help.
