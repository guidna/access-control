#include <RestClient.h>
#include <Ethernet.h>
#include <SPI.h>

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
  ethernetSetup();
  smsSetup();
  
  int statusCode = client.post("/sms", parametros.c_str(), &response);

  Serial.print("Status da resposta: ");
  Serial.println(statusCode);
  
  Serial.print("Resposta do servidor: ");
  Serial.println(response);
  
  delay(1000);

}

void loop() {
  // put your main code here, to run repeatedly:

}

void serialSetup() {
  Serial.begin(9600);
  while (!Serial) {    
  }
  Serial.println("Inicio");
}

void ethernetSetup() {
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Falha em configurar Ethernet usando DHCP");
    Ethernet.begin(mac, ip);
  }
  delay(1000);
  Serial.println("Ethernet Ok...");
}

void smsSetup() {
  
  parametros.concat("sid=");
  parametros.concat(sid);

  parametros.concat("&token=");
  parametros.concat(token);

  parametros.concat("&to=");
  parametros.concat(to);

  parametros.concat("&from=");
  parametros.concat(from);

  parametros.concat("&body=oi Amor, este SMS foi enviado pelo Arduino aqui no curso do SENAI. Te amo. Boa Sorte HJ e sempre.");

}

