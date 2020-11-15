#include <Servo.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#pragma region Globals
const char* ssid = "NOMEREDE";                           // WIFI network name
const char* password = "SENHA";                       // WIFI network password
uint8_t connection_state = 0;                    // Connected to WIFI or not
ESP8266WebServer server(80);
Servo servo; 
int val;
int auxLoop=0; 
boolean flag = false;
#pragma endregion Globals


uint8_t WiFiConnect(const char* nSSID = nullptr, const char* nPassword = nullptr)
{
  static uint16_t attempt = 0;
  Serial.print("Connecting to ");
  if (nSSID) {
    WiFi.begin(nSSID, nPassword);
    Serial.println(nSSID);
  } else {
    WiFi.begin(ssid, password);
    Serial.println(ssid);
  }

  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 50)
  {
    delay(200);
    Serial.print(".");
  }
  ++attempt;
  Serial.println("");
  if (i == 51) {
    Serial.print("Connection: TIMEOUT on attempt: ");
    Serial.println(attempt);
    if (attempt % 2 == 0)
      Serial.println("Check if access point available or SSID and Password\r\n");
    return false;
  }
  Serial.println("Connection: ESTABLISHED");
  Serial.print("Got IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}

void Awaits()
{
  uint32_t ts = millis();
  while (!connection_state)
  {
    delay(50);
    if (millis() > (ts + 10000) && !connection_state) {
      connection_state = WiFiConnect();
      ts = millis();
    }
  }
}

void setup() {
  delay(1000);
  Serial.begin(9600); // Vamos usar para debug

  // Conectando à rede WiFi
  Serial.println();
  Serial.println();
  Serial.print("Conectando com: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado com sucesso");

  server.on("/abertura", handleAbertura);   //Associate the handler function to the path

  // Startando o servidor
  server.begin();
  Serial.print("Servidor startado em ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D0, OUTPUT);
  servo.attach(D0);
  servo.write(90); 
}

void loop() {
  if(flag && auxLoop==0)
  {
    Serial.println("Entrou");
    servo.write(360); 
    digitalWrite(D8, HIGH); 
    digitalWrite(D5, HIGH); 
    digitalWrite(D6, LOW);    
    digitalWrite(D7, LOW); 
    delay(2000); 
    auxLoop++;
  }
  server.handleClient();
  servo.write(90);    
  digitalWrite(D8, LOW);                   
  digitalWrite(D5, LOW);   
  digitalWrite(D6, HIGH);  
  digitalWrite(D7, HIGH);
}

void handleAbertura() {  
  String message = "";
  if (server.arg("password") == "") {   //Parameter not found
    message = "Password Argument not found";
  } else {    //Parameter found
    message = "Password Argument = ";
    message += server.arg("password");     //Gets the value of the query parameter
  }
  if (server.arg("function") == "") {   //Parameter not found
    message += "\nFunction Argument not found";
  } else {    //Parameter found
    message += "\nFunction Argument = ";
    message += server.arg("function");     //Gets the value of the query parameter
  }

  if(server.arg("password") == "senha")
  {
    if (server.arg("function") == "0") {
        message += "\nMessage Turn Off send.";
        flag = false;
        auxLoop=0;
    }
    else if (server.arg("function") == "1") {
        message += "\nMessage Turn On send.";
        flag = true; 
    }
    else
    {
        message += "\nInvalid Function.";
    }
  }
  else{
        message += "\nInvalid Password";    
  }
  server.send(200, "text/plain", message);          //Returns the HTTP response
}
