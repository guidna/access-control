/*/*//*/*Conexão MQTT*//*/*/
#include <UIPEthernet.h>
#include <utility/logging.h>

#include <SPI.h>
#include <PubSubClient.h>

byte mac[] = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0x02 };
boolean alarmActivated;
boolean messageSent;

void callback(char* topic, byte* payload, unsigned int length) {
    // https://www.arduino.cc/en/Reference/StringToInt
  byte statusMQTTAlarm = payload[0] - '0';

  Serial.print("Topic received: "); Serial.println(topic);
  Serial.print("Message: "); Serial.println(statusMQTTAlarm);

  // Dentro do callback da biblioteca MQTT,
  // devemos usar Serial.flush() para garantir que as mensagens serão enviadas
  Serial.flush();

  Serial.print("Numero recebido: "); Serial.println(statusMQTTAlarm);

  Serial.flush();

  setAlarm(statusMQTTAlarm);
}

EthernetClient ethClient;
PubSubClient client("m14.cloudmqtt.com", 19138, callback, ethClient);

long lastReconnectAttempt = 0;

/***********Ultrassom***********/

#define LedR 7
#define LedG 8

#include <Ultrasonic.h>

Ultrasonic ultrasonic(12, 13);

/*SETUP*//////
void setup() {
  serialSetup();
  ethernetSetup();
  ledSetup();
}

/*LOOP******/
void loop() {
  timerMQTTconnect();
  if(checkAlarm()) {
    if(!messageSent){
      publishMQTTAlarm();
      messageSent = true;
    }
  }
}
void serialSetup() {
  Serial.begin(9600);
  while (!Serial) {
  }
  Serial.println("Setup");
}
void ethernetSetup() {
  if(Ethernet.begin(mac)) {
    Serial.println(Ethernet.localIP());
  } else {
    Serial.println("Não pegou IP");
  }
}
void ledSetup() {
  pinMode(LedR, OUTPUT);
  pinMode(LedG, OUTPUT);
  Serial.println("LED Setup");
}
void timerMQTTconnect() {
  if (!client.connected()) {
    long now = millis();

    if (now - lastReconnectAttempt > 5000) {
      Serial.print("...reconectando: ");
      lastReconnectAttempt = now;
      Serial.print("Conectando ao MQTT");
      reconnect();
      if (client.connected()) {
        lastReconnectAttempt = 0;
      }
    }
  } else {
    client.loop();
  }
}

void turnLed(int state) {
  digitalWrite(LedR, state);
  digitalWrite(LedG, state);
}

void setAlarm(boolean activated) {
  alarmActivated = activated;
}

boolean checkAlarm() {
  if(!alarmActivated) {
    return false;
  }
  
  delay(100);//5000

  Serial.println("Alarme ativado!");

  boolean invasao;

  int distancia = ultrasonic.distanceRead();

  Serial.print("Distância: ");
  Serial.println(ultrasonic.distanceRead());

  if (distancia < 30) {
    Serial.println("invasao 1");
    invasao = true;
  } else {
    Serial.println("invasao 0");
    invasao = false;
  }

  return invasao;

}
void reconnect() {
  if (client.connect("arduinoClient", "casa01", "senha01")) {
    Serial.println("MQTT conectado");
    // ... and resubscribe
    client.subscribe("alarme");
    Serial.println("MQTT assinado: alarme");
  } else {
    Serial.println("Erro no subscribe");
  }
}
boolean publishMQTTAlarm() {
  return client.publish("invasao", "Casa invadida!");
}
void verifyStatusAlarm() {

}

