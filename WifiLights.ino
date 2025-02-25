#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Robotics";
const char* password = "Hope@2023!";

// Predefined accounts (username and password)
const char* validUsernames[] = {"admin", "user1"};
const char* validPasswords[] = {"admin123", "user1123"};
const int numUsers = 2;

const int trigPin = D5;  // Trigger pin of the ultrasonic sensor
const int echoPin = D6;  // Echo pin of the ultrasonic sensor
const int LedPins[] = {D0,D1,D2,D3}; // LED pins
#define SENSOR_PIN D7    // Pin to control the sensor
bool isSwitchOn=false;

ESP8266WebServer server(80);

bool IsSensorOn[] = {false, false, false, false};
bool isAuthenticated = false;
long distance;

// Declare function prototypes
void handleLogin();
void handleLogout();
void handleRoot();
void toggleLED(int ledIndex);
void turnOnLED(int ledIndex);
void turnOffLED(int ledIndex);
void turnOnSensor(int sensorIndex);
void turnOffSensor(int sensorIndex);


const char* loginPage = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Login</title>
  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css">
  <style>
    body, html {
      height: 100%;
      display: flex;
      justify-content: center;
      align-items: center;
      background: #f7f9fc;
    }
    .login-container {
      background: #fff;
      padding: 30px;
      border-radius: 10px;
      box-shadow: 0 4px 8px rgba(0,0,0,0.1);
    }
    .login-title {
      font-size: 24px;
      font-weight: bold;
      margin-bottom: 20px;
    }
  </style>
</head>
<body>
  <div class="login-container">
    <div class="login-title">Login</div>
    <form action="/login" method="post">
      <div class="form-group">
        <label for="username">Username:</label>
        <input type="text" class="form-control" id="username" name="username" required>
      </div>
      <div class="form-group">
        <label for="password">Password:</label>
        <input type="password" class="form-control" id="password" name="password" required>
      </div>
      <button type="submit" class="btn btn-primary btn-block">Login</button>
    </form>
  </div>
</body>
</html>

)=====";



const char* controlPage = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Admin Dashboard</title>
  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css">
  <style>
    body {
      display: flex;
      min-height: 100vh;
      font-family: Arial, sans-serif;
    }
    .sidebar {
      min-width: 250px;
      max-width: 250px;
      background: #343a40;
      color: #fff;
      transition: all 0.3s;
    }
    .sidebar a {
      color: #ddd;
      padding: 15px;
      display: block;
      text-decoration: none;
      transition: background 0.3s;
    }
    .sidebar a:hover {
      background: #007bff;
    }
    .main-content {
      flex-grow: 1;
      padding: 20px;
      background: #f1f1f1;
    }
    .navbar {
      background: #007bff;
      color: #fff;
    }
    .toggle-btn {
      cursor: pointer;
      color: #fff;
    }
    .btn-toggle {
      border-radius: 50%;
      width: 100px;
      height: 100px;
      display: flex;
      justify-content: center;
      align-items: center;
      text-align: center;
      line-height: 20px;
    }
    .divider {
      border-left: 2px solid #343a40;
      height: 100%;
    }
    .table-control {
      width: 100%;
      margin-bottom: 20px;
    }
  </style>
</head>
<body>
  <div class="sidebar" id="sidebar">
    <a href="/control">Home</a>
    <a href="/settings">Settings</a>
    <a href="/profile">Profile</a>
    <a href="/logout">Logout</a>
  </div>
  <div class="main-content">
    <nav class="navbar navbar-expand-lg navbar-light">
      <span class="toggle-btn" id="toggle-btn">&#9776;</span>
      <div class="navbar-brand">Admin Dashboard</div>
    </nav>
    <div class="container mt-4">
      <h2>Welcome to the Admin Dashboard</h2>
      <p>Use the sidebar to navigate through the options.</p>
      
      <div class="row mb-4">
        <div class="col-md-5">
          <h4>Managing LEDs</h4>
          <table class="table table-control">
            <tbody>
              <tr>
                <td><button id="LED1" class="btn btn-primary btn-toggle mt-3">Toggle LED 1</button></td>
                <td><button id="LED2" class="btn btn-primary btn-toggle mt-3">Toggle LED 2</button></td>
              </tr>
              <tr>
                <td><button id="LED3" class="btn btn-primary btn-toggle mt-3">Toggle LED 3</button></td>
              <!--  <td><button id="LED4" class="btn btn-primary btn-toggle mt-3">MAIN SWITCH</button></td>-->
              </tr>
            </tbody>
          </table>
        </div>
        <div class="col-md-1">
          <div class="divider"></div>
        </div>
        <div class="col-md-5">
          <h4>Managing Sensors</h4>
          <table class="table table-control">
            <tbody>
              <tr>
                <td><button id="Sensor1" class="btn btn-secondary btn-toggle mt-3">Toggle Sensor 1</button></td>
                <td><button id="Sensor2" class="btn btn-secondary btn-toggle mt-3">Toggle Sensor 2</button></td>
              </tr>
              <tr>
                <td><button id="Sensor3" class="btn btn-secondary btn-toggle mt-3">Toggle Sensor 3</button></td>
                <!--<td><button id="Sensor4" class="btn btn-secondary btn-toggle mt-3">Toggle Sensor 4</button></td>-->
              </tr>
            </tbody>
          </table>
        </div>
      </div>
    </div>
  </div>
  
  <script>
    document.getElementById('toggle-btn').addEventListener('click', function() {
      document.getElementById('sidebar').classList.toggle('sidebar-hidden');
    });

    function toggleButton(buttonId, urlOn, urlOff) {
      const button = document.getElementById(buttonId);
      let isOn = false;
      button.addEventListener('click', function() {
        const url = isOn ? urlOff : urlOn;
        fetch(url)
          .then(response => response.text())
          .then(state => {
            console.log(state);
            button.classList.toggle('btn-success', !isOn);
            button.classList.toggle('btn-danger', isOn);
            button.textContent = isOn ? `Turn On ${buttonId}` : `Turn Off ${buttonId}`;
            isOn = !isOn;
          });
      });
    }

    toggleButton('LED1', '/turn_on_led1', '/turn_off_led1');
    toggleButton('LED2', '/turn_on_led4', '/turn_off_led4');
    toggleButton('LED3', '/turn_on_led3', '/turn_off_led3');
   <!-- toggleButton('toggleLED4', '/turn_on_led2', '/turn_off_led2');-->
    toggleButton('Sensor1', '/turn_on_sensor1', '/turn_off_sensor1');
    toggleButton('Sensor2', '/turn_on_sensor4', '/turn_off_sensor4');
    toggleButton('Sensor3', '/turn_on_sensor3', '/turn_off_sensor3');
   <!-- toggleButton('Sensor4', '/turn_on_sensor2', '/turn_off_sensor2');-->
  </script>
</body>
</html>


)=====";


void setup() {
  Serial.begin(9600);
  
  // Initialize LED and sensor pins
  for (int i = 0; i < 4; i++) {
    pinMode(LedPins[i], OUTPUT);  // Set the LED pins as outputs
    pinMode(SENSOR_PIN, OUTPUT);  // Set the sensor pin as an output
  }
  
  pinMode(trigPin, OUTPUT);       // Set the trigger pin as an output
  pinMode(echoPin, INPUT);        // Set the echo pin as an input

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.print ("IP address: ");
  Serial.println (WiFi.localIP ()); // Print the IP address
  Serial.println("Connected to WiFi");
  

  // Set up HTTP server
  server.on("/", HTTP_GET, handleLogin); // Show login page initially
  server.on("/login", HTTP_POST, []() { // Handle login form submission
    String username = server.arg("username");
    String password = server.arg("password");
    bool isValidUser = false;
    for (int i = 0; i < numUsers; i++) {
      if (username == validUsernames[i] && password == validPasswords[i]) {
        isValidUser = true;
        break;
      }
    }
    if (isValidUser) {
      isAuthenticated = true;
      server.sendHeader("Location", "/control");
      server.send(302);
    } else {
      server.send(200, "text/html", "<h1>Invalid credentials</h1><a href=\"/\">Try again</a>");
    }
  });
  server.on("/control", HTTP_GET, handleRoot); // Show control page after login
  server.on("/logout", HTTP_GET, handleLogout); 
  for (int i = 0; i < 4; i++) {
    String ledIndex = String(i + 1);
    server.on("/toggle_led" + ledIndex, HTTP_GET, [i]() { toggleLED(i); });
    server.on("/turn_on_led" + ledIndex, HTTP_GET, [i]() { turnOnLED(i); });
    server.on("/turn_off_led" + ledIndex, HTTP_GET, [i]() { turnOffLED(i); });
    server.on("/turn_on_sensor" + ledIndex, HTTP_GET, [i]() { turnOnSensor(i); });
    server.on("/turn_off_sensor" + ledIndex, HTTP_GET, [i]() { turnOffSensor(i); });
  }
  server.begin();
  Serial.println("HTTP server started");

  // Turn off the LEDs initially
  for (int i = 0; i < 4; i++) {
    digitalWrite(LedPins[i], HIGH);
    Serial.println("LED " + String(i + 1) + " turned Off");
  }
}

void loop() {
  server.handleClient();
  for (int i = 0; i < 4; i++) {
    if (IsSensorOn[i]) {
      long duration, distance;

      // Send a pulse to the ultrasonic sensor
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);

      // Read the echo signal
      duration = pulseIn(echoPin, HIGH);

      // Calculate the distance
      distance = (duration / 2) / 29.1;
      if (distance < 50) {
        digitalWrite(LedPins[i], LOW);
      } else {
        digitalWrite(LedPins[i], HIGH);
      }
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.println(" cm");

      delay(1000);  // Wait for 1 second before the next measurement
    }
  }
}

void handleLogin() {
  server.send(200, "text/html", loginPage);
}

void handleRoot() {
  if (isAuthenticated) {
    server.send(200, "text/html", controlPage);
  } else {
    server.sendHeader("Location", "/");
    server.send(302);
  }
}

void handleLogout(){
  isAuthenticated = false;
  server.sendHeader("Location","/");
  server.send(302);
}

void toggleLED(int ledIndex) {
  if (isAuthenticated) {
    if(ledIndex==1)
    {
      isSwitchOn = !isSwitchOn;
      Serial.print("Switch turned ");
      Serial.println(isSwitchOn ? "ON" : "OFF");
    }
    else
    {
      if(isSwitchOn)
      {
        digitalWrite(LedPins[ledIndex], !digitalRead(LedPins[ledIndex]));  // Toggle LED state
      }
      
    }

    server.send(200, "text/plain", digitalRead(LedPins[ledIndex]) ? "LED is ON" : "LED is OFF");

  } else {
    server.sendHeader("Location", "/");
    server.send(302);
  }
}

void turnOnLED(int ledIndex) {
  if (isAuthenticated){
    digitalWrite(LedPins[ledIndex], LOW);  // Turn on the LED
    IsSensorOn[ledIndex] = false;
    server.send(200, "text/plain", "LED turned ON");
  }else{
    server.sendHeader("Location", "/");
    server.send(302);
  }
}

void turnOffLED(int ledIndex) {
  if (isAuthenticated){
    digitalWrite(LedPins[ledIndex], HIGH);  // Turn off the LED
    IsSensorOn[ledIndex] = false;
    server.send(200, "text/plain", "LED turned OFF");
  }else{
    server.sendHeader("Location", "/");
    server.send(302);
  }
}

void turnOnSensor(int sensorIndex) {
  if (isAuthenticated){
    IsSensorOn[sensorIndex] = true;
    
    long duration;

    // Send a pulse to the ultrasonic sensor
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Read the echo signal
    duration = pulseIn(echoPin, HIGH);

    // Calculate distance (assuming speed of sound is 343 m/s)
    distance = (duration / 2) * 0.0343;
    Serial.print("Distance: ");
    Serial.println(distance);

    // Control the LED based on distance
    if(IsSensorOn[sensorIndex]){
          if (distance < 20) {
            digitalWrite(LedPins[sensorIndex], HIGH);
          } else {
            digitalWrite(LedPins[sensorIndex], LOW);
          }
    }
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    delay(1000);  // Wait for 1 second before the next measurement

    server.send(200, "text/plain", "Sensor turned ON");
  }else{
    server.sendHeader("Location", "/");
    server.send(302);
  }
}

void turnOffSensor(int sensorIndex) {
  if (isAuthenticated){
    IsSensorOn[sensorIndex] = false;
    digitalWrite(LedPins[sensorIndex],LOW);
    server.send(200, "text/plain", "Sensor turned OFF");
  }else{
    server.sendHeader("Location", "/");
    server.send(302);
  }
}
