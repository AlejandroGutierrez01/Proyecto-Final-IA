// Colocar sus credenciales
#define BLYNK_TEMPLATE_ID "TMPL28rndTHcH"
#define BLYNK_TEMPLATE_NAME "PROYECTO FINAL"
#define BLYNK_AUTH_TOKEN "Q1-jgzuW2fAEJIo1WlKiG8NbUVJfIa60"
#define BLYNK_PRINT Serial

// Librería para conectar con el servidor de Blynk
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

// SSID y password red Wifi
char ssid[] = "CAMPUS_EPN";
char pass[] = "politecnica**"; 
char token[] = BLYNK_AUTH_TOKEN;

// Librería y pines para conectar al módulo wifi
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); // (RX, TX)
ESP8266 wifi(&EspSerial);

// Definición de pines
int pinLuz = A5;        // Sensor de luz (fotorresistencia)
int pinPIR = 9;         // Sensor de movimiento PIR
int pinRojo = 4;        // LED Rojo
int pinVerde = 5;       // LED Verde
int pinBlanco = 6;      // LED Blanco
int pinAzul = 7;        // LED Azul
int pinAmarillo = 8;    // LED Amarillo

void setup() {
  pinMode(pinRojo, OUTPUT);
  pinMode(pinBlanco, OUTPUT);
  pinMode(pinAzul, OUTPUT);
  pinMode(pinVerde, OUTPUT);
  pinMode(pinAmarillo, OUTPUT);
  pinMode(pinPIR, INPUT); // Configurar el PIR como entrada
  
  // Debug console
  Serial.begin(9600);
  
  // Configuración del módulo ESP8266
  EspSerial.begin(115200);
  EspSerial.write("AT+UART_DEF=9600,8,1,0,0\r\n");
  EspSerial.write("AT+RST\r\n");
  delay(100);
  EspSerial.begin(9600);
  delay(10);
  
  // Conexión a Blynk
  Blynk.begin(token, wifi, ssid, pass);
}

void loop() {
  // Sensor de luz (fotorresistencia)
  int valorLuz = analogRead(pinLuz);
  String nivelLuz;
  
  if (valorLuz < 40) {
    nivelLuz = "Bajo";
    Blynk.virtualWrite(V6, "Bajo");  
    digitalWrite(pinAmarillo, HIGH); // Enciende LED amarillo
  } else {
    digitalWrite(pinAmarillo, LOW);  // Apaga LED amarillo
    if (valorLuz >= 40 && valorLuz < 300) {
      nivelLuz = "Medio";
      Blynk.virtualWrite(V6, "Medio");  

    } else {
      nivelLuz = "Alto"; 
      Blynk.virtualWrite(V6, "Alto");  

    }
  }

  Serial.println("Luz: " + String(valorLuz) + " (" + nivelLuz + ")");

// Sensor de movimiento PIR
  int movimiento = digitalRead(pinPIR);
  
  if (movimiento == HIGH) {
    Serial.println("Movimiento detectado");
    Blynk.virtualWrite(V5, "Movimiento detectado");  
  } else {
    Blynk.virtualWrite(V5, "Sin movimiento");  
  }
  
  // Ejecuta Blynk.run() para mantener la conexión con la app
  Blynk.run();
}

// Funciones para encender y apagar los LEDs con los switches de Blynk

BLYNK_WRITE(V0) { // Pin virtual V0 - LED Rojo
  int estado = param.asInt(); 
  digitalWrite(pinRojo, estado);
}

BLYNK_WRITE(V1) { // Pin virtual V1 - LED Azul
  int estado = param.asInt();
  digitalWrite(pinAzul, estado);
}

BLYNK_WRITE(V2) { // Pin virtual V2 - LED Verde
  int estado = param.asInt();
  digitalWrite(pinVerde, estado);
}

BLYNK_WRITE(V3) { // Pin virtual V3 - LED Blanco
  int estado = param.asInt();
  digitalWrite(pinBlanco, estado);
}

BLYNK_WRITE(V4) { // Pin virtual V4 - LED Amarillo del Patio
  int estado = param.asInt();
  digitalWrite(pinAmarillo, estado);
}