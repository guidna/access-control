/*/*//*/*Conexão MQTT*//*/*/
#include <UIPEthernet.h>
#include <utility/logging.h>

#include <SPI.h>
#include <PubSubClient.h>

byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0x02 };

void callback(char* topic, byte* payload, unsigned int length) {
  // Converter pointer do tipo `byte` para typo `char`
  char* payloadAsChar = payload;
  // Workaround para pequeno bug na biblioteca
  payloadAsChar[length] = 0;
  // Converter em tipo String para conveniência
  String msg = String(payloadAsChar);
  // https://www.arduino.cc/en/Reference/StringToInt
  int statusAlarmMQTT = msg.toInt();
  
  
  Serial.print("Topic received: "); Serial.println(topic);
  Serial.print("Message: "); Serial.println(msg);

  // Dentro do callback da biblioteca MQTT, 
  // devemos usar Serial.flush() para garantir que as mensagens serão enviadas
  Serial.flush();

  Serial.print("Numero recebido: "); Serial.println(statusAlarmMQTT);

  if (statusAlarmMQTT){
    ultrasoundActive();
    if (ultrasoundActive()){
      
    }
  }
  
  Serial.flush();
  
  turnLed(msgComoNumero);
}

EthernetClient ethClient;
PubSubClient client("m14.cloudmqtt.com", 19138, callback, ethClient);

long lastReconnectAttempt = 0;

boolean reconnect() {
  Serial.println("conectando...");
  if (client.connect("arduinoClient", "casa01", "senha01")) {
    Serial.println("MQTT conectado");
    // Once connected, publish an announcement...
    client.publish("alarme","hello world");
    Serial.println("MQTT enviado");
    // ... and resubscribe
    client.subscribe("alarme");
  }else{
    Serial.println("Erro ao conectar");  
  }
  return client.connected();
}

/***********Ultrassom***********/

#define LedR 7
#define LedG 8

#include <Ultrasonic.h>

Ultrasonic ultrasonic(12, 13);

void setup() {
  serialSetup();
  ethernetSetup();
  ledSetup();
}

void loop() {
  Serial.println("Loop");
  timerMQTTconnect();

}
void serialSetup() {

  Serial.begin(9600);
  while (!Serial) {
  }
  Serial.println("Setup");
  
}
void ethernetSetup(){
  Ethernet.begin(mac);
  delay(1500);
  lastReconnectAttempt = 0;
}
void timerMQTTconnect(){
  if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
        Serial.print("Conectando ao MQTT");
      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    }
  } else {
    Serial.print("Conectado ao MQTT");
    client.loop();
  }
}

void ledSetup(){
  pinMode(LedR, OUTPUT);
  pinMode(LedG, OUTPUT);
}
void turnLed(int state) {
    digitalWrite(LedR,!state);
    digitalWrite(LedG, state);

}

int ultrasoundActive(){
  Serial.print("Distance in CM: ");
  Serial.println(ultrasonic.distanceRead());
  int distancia = ultrasonic.distanceRead();
  int invasao;
  
    if (distancia<30){
      delay(100);
      Serial.println("invasao 1"); 
      invasao = 1;
    }
  return invasao;
}

