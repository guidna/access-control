#include <UIPEthernet.h>
#include <utility/logging.h>

#include <Wire.h>
#include <Ultrasonic.h>

#include <RestClient.h>

#include <SPI.h>

Ultrasonic ultrasonic(8, 9);
long timeAlarmOff = 60000;
int safeDistance = 15;
long invasionTime = 0;
long countdown = 0;
boolean lAlarmActive = true;
boolean invasionDetect = false;


EthernetClient ethClient;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xA0, 0xA3 };
IPAddress ip (192, 162, 1, 3);
const int porta = 3000;

RestClient client("192.168.3.186", porta, ethClient);

const char* sid = "AC539dcdc0b21a3a42e6cd3cba5ee363d9";
const char* token = "8fbecef387ee99ffaf81e7c3ff65893e";
const char* to = "5511981669192";
const char* from = "12402198343";

String parametros = "";
String response = "";



void setup() {
  serialSetup();
  wireSetup();
  ethernetSetup(); 
}

void loop() {
  
  if( lAlarmActive ) {

    if( !invasionDetect ) {
      
      if( invasion() ) {
        invasionDetect = true;
        invasionTime = millis();
        Serial.println( timeAlarmOff );
        Serial.println("Contagem regressiva");
      }
      
    } else {
      
      if( countdown <= 0 ) {
         Serial.println("POLICIA");
         smsSend();
      } else {
        countdown = (timeAlarmOff - (millis() - invasionTime));

        Serial.println("==========================");

        Serial.println((millis() - invasionTime));
        Serial.println( timeAlarmOff );
        Serial.println( countdown );
      
      }

    }

  } else {
    invasionDetect = false;
    invasionTime = 0;
    countdown = 0;
  }
  
}

boolean invasion() {
  int distancia = ultrasonic.distanceRead();
  
  Serial.println(distancia);
  
  return (distancia < safeDistance);

  
}

void serialSetup() {
  Serial.begin(9600);
  while (!Serial) {
  }
  Serial.println("Início");
}

void wireSetup() {
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
}

void receiveEvent(int length) {

  while (Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
  }
  
}

void ethernetSetup() {
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Falha em configurar Ethernet usando DHCP");
    Ethernet.begin(mac, ip);
  }
  delay(1000);
  Serial.println("Ethernet Ok...");
}

void smsSend() {

  int statusCode = client.post("/sms", parametros.c_str(), &response);  
  
  parametros.concat("sid=");
  parametros.concat(sid);

  parametros.concat("&token=");
  parametros.concat(token);

  parametros.concat("&to=");
  parametros.concat(to);

  parametros.concat("&from=");
  parametros.concat(from);

  parametros.concat("&body=A casa foi invadida e o alarme não foi desarmado");

  Serial.print("Status da resposta: ");
  Serial.println(statusCode);
  
  Serial.print("Resposta do servidor: ");
  Serial.println(response);

}

