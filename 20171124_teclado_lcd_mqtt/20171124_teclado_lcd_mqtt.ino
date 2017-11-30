/*/*//*/*Conexão MQTT*//*/*/
#include <UIPEthernet.h>
#include <utility/logging.h>

#include <SPI.h>
#include <PubSubClient.h>

byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0x02 };

void callback(char* topic, byte* payload, unsigned int length) {
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

/*/*//*/*Teclado*//*/*/

#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {A3, A2, A1, A0}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, A5, A4}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

bool tapyingPass = false;

String senhaDigitada;
String senha = "1557";

/*/*//*/*LCD*//*/*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 9, en = 8, d4 = 4, d5 = 3, d6 = 2, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup(){
  serialSetup();
  lcdSetup();
  keypadSetup();
  ethernetSetup();
  
}
  
void loop(){
  Serial.println("Loop");
  getPassword();
  timerMQTTconnect();

}
void serialSetup() {

  Serial.begin(9600);
  while (!Serial) {
  }
  Serial.println("Setup");
  
}
void lcdSetup(){
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
}
void keypadSetup(){
  pinMode(A0, INPUT);
}
void getPassword(){
  char key = keypad.getKey();
    
    if (key){
      
      if((key == '#') &&(senhaDigitada != "")) {
          tapyingPass = false;
          
          Serial.println("Terminou de digitar a senha.");
          Serial.println("Senha: ");
          Serial.println(senhaDigitada);
          
          if (senhaDigitada == senha){
            printLCD(0,1,"Alarme desativado");
            Serial.println("Alarme desativado");
          } else {
            printLCD(0,1,"Corre!");
            Serial.println("Corre!");
          }
          
          senhaDigitada = "";
        } else {
            Serial.println("Digite uma senha");
     }
      
      if(tapyingPass){
        senhaDigitada += key; 
        Serial.print("Senha digitada :");
        Serial.println(senhaDigitada);
      }
      
      if(key == '*'){
        tapyingPass = true;
        Serial.println("Começou a digitar a senha...");
      }

    }
}
void printLCD(int y, int x, String messageLCD){
    // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(y, x);
  // print the number of seconds since reset:
  lcd.print("                ");
  lcd.setCursor(y, x);
  lcd.print(messageLCD);
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

