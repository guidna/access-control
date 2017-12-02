/*/*//*/*Conexão MQTT*//*/*/
#include <UIPEthernet.h>
#include <utility/logging.h>

#include <SPI.h>
#include <PubSubClient.h>

byte mac[] = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0x02 };

boolean callback(char* topic, byte* payload, unsigned int length) {
  // Converter pointer do tipo `byte` para typo `char`
  char* payloadAsChar = payload;
  // Workaround para pequeno bug na biblioteca
  payloadAsChar[length] = 0;
  // Converter em tipo String para conveniência
  String msg = String(payloadAsChar);
  // https://www.arduino.cc/en/Reference/StringToInt
  int statusMQTTAlarm = msg.toInt();


  Serial.print("Topic received: "); Serial.println(topic);
  Serial.print("Message: "); Serial.println(msg);

  // Dentro do callback da biblioteca MQTT,
  // devemos usar Serial.flush() para garantir que as mensagens serão enviadas
  Serial.flush();

  Serial.print("Numero recebido: "); Serial.println(statusMQTTAlarm);

  Serial.flush();

  return statusMQTTAlarm;
}

EthernetClient ethClient;
PubSubClient client("m14.cloudmqtt.com", 19138, callback, ethClient);

long lastReconnectAttempt = 0;

boolean reconnect() {
  Serial.print("...reconectando: ");
  Serial.println(client.connected());
  return client.connected();
}

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
  Serial.println("Loop");

  timerMQTTconnect();
  subscribeMQTTAlarm();


  if (callback) {
    boolean callback1 = callback;
    boolean callback2 = callback();    
    
    Serial.print("Callback: ");
    Serial.println(callback1);

    Serial.print("Callback(): ");
    Serial.println(callback2);
    
    turnLed(callback);
    Serial.print("Turn Led ON/OFF: ");
    Serial.println(callback2);
    
    if (ultrasoundActivate()) {
      publishMQTTAlarm();
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
  Ethernet.begin(mac);
  delay(1500);
  lastReconnectAttempt = 0;
  Serial.println("Ethernet Setup");
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

void turnLed(int state) {
  digitalWrite(LedR, state);
  digitalWrite(LedG, state);
}

boolean ultrasoundActivate() {

  delay(100);//5000

  Serial.println("Alarme ativado!");

  int invasao;

  int distancia = ultrasonic.distanceRead();

  Serial.print("Distância: ");
  Serial.println(ultrasonic.distanceRead());

  if (distancia < 30) {
    Serial.println("invasao 1");
    invasao = 1;
  } else {
    Serial.println("invasao 0");
    invasao = 0;
  }

  return invasao;

}
void subscribeMQTTAlarm() {
  if (client.connect("arduinoClient", "casa01", "senha01")) {
    Serial.println("MQTT conectado");
    // ... and resubscribe
    client.subscribe("alarme");
    Serial.println("MQTT assinado: alarme");
  } else {
    Serial.println("Erro no subscribe");
  }
}
void publishMQTTAlarm() {
  if (client.connect("arduinoClient", "casa01", "senha01")) {
    Serial.println("Casa invadida!");
    // Once connected, publish an announcement...
    client.publish("invasao", "Casa invadida!");
  } else {
    Serial.println("Problema ao publicar mensagem 'Casa invadida'");
  }
}
void verifyStatusAlarm(){

}

