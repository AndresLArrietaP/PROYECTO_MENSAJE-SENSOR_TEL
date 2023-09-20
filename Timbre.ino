#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

const char* ssid = "Prueba123";
const char* password = "CesarAHP";
#define token_Bot "6424814048:AAHl7JDGSrGiTmZZTONxB-w4R-SRLBsG7NM"
#define ID_Chat "1476469007" 

WiFiClientSecure client;
UniversalTelegramBot bot(token_Bot, client);

const int trigPin = 22;
const int echoPin = 23;
const int luz = 19;

int ledPin = 18; // Led
int potPin = 34; // Sensor de gas
int buzzerPin = 5; // Buzzer
// Potenciómetro

// Velocidad de transmisión
const int serialSpeed = 9600;

// Variable para almacenar el estado del sensor de gas
int sensorValue = 0;

// Variable para almacenar el volumen
int volume = 0;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(luz, OUTPUT);

  Serial.print("Conectando a la red wifi... ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println("");
  Serial.println("Conectado a la red wifi!!!");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
  bot.sendMessage(ID_Chat, "Sistema preparado!!!", "");
  
  // Inicializa los pines como salidas o entradas
  pinMode(ledPin, OUTPUT);
  pinMode(potPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  long duration;
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  int distancia = (duration / 2) / 29.1;

  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

  if (distancia <= 5) {
    digitalWrite(luz, HIGH);
    Serial.println("Luz Encendida!");
    
    // Envía un mensaje a Telegram cuando detecta algo a 5 cm o menos
    String distanciaStr = String(distancia);
    String mensaje = "¡Algo se está acercando a " + distanciaStr + " cm!";
    bot.sendMessage(ID_Chat, mensaje, "");

    delay(5000);
    digitalWrite(luz, LOW);
    Serial.println("Luz apagada!");
  }

  // Lee el valor del sensor de gas
  sensorValue = analogRead(potPin);

  // Si el valor del sensor de gas supera el umbral, enciende el LED y el buzzer
  if (sensorValue > 400) {
    digitalWrite(ledPin, HIGH);
    String mensaje = "¡Alguien esta tocando el timbre";
    bot.sendMessage(ID_Chat, mensaje, "");

    // Lee el volumen del potenciómetro
   

    // Establece el volumen del buzzer
    analogWrite(buzzerPin, volume);

    // Emite una señal de audio con la duración especificada
    tone(buzzerPin, 440, 1000);
  }
  // Si el valor del sensor de gas no supera el umbral, apaga el LED y el buzzer
  else {
    digitalWrite(ledPin, LOW);
    analogWrite(buzzerPin, 0);
  }

  // Imprime las lecturas de todos los sensores en el monitor serie
  Serial.print("Potenciómetro: ");
  Serial.println(sensorValue);

  Serial.println("-----------------");
  delay(2000);
}