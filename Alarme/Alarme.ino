#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {6, 5, 3, 2};
byte colPins[COLS] = {9, 8, 7};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

String passActive = "000000";
String newPass;

int levelChangePass = 0;
int action = 0;

boolean getPassword = false;
boolean alarmActive = false;

String seqKeyWord = "";


void setup(){

  serialSetup();
  
}
  
void loop(){

  char key = keypad.getKey();

  if(key) {

    action = keyWord(key);
    
    if(( action >= 1 ) && (action <= 3)) {
      changePass(action);
      
    } else if ( action == 100 ) {
      levelCheckPass();
      
    } else if (action == 200) {
      getPassword = true;
      
    } else if (action == 300) {
      changeAlarm();

    }
      
    if( action > 0) {
      cleanVariable();
    }  
    
  }

}

void serialSetup() {
  
  Serial.begin(9600);
  while (!Serial) {    
  }
  Serial.println("Inicio");
  
}

int keyWord(char keyPress) {

int ret = 0;

  // 1 - Mudar senha ativação alarme
  // 2 - Mudar senha desativação alarme
  // 3 - Mudar senha pânico alarme

  if((seqKeyWord == "") && (keyPress == '#')) {
    seqKeyWord += keyPress;
  } else if ((seqKeyWord == "#") && (keyPress == '1')) {
    ret = 1;
  } else if ((seqKeyWord == "#") && (keyPress == '2')) {
    ret = 2;
  } else if ((seqKeyWord == "#") && (keyPress == '3')) {
    ret = 3;
  } else if ((keyPress != '#') && (keyPress != '*')) {
    seqKeyWord += keyPress;
  } else if ((levelChangePass > 0) && (keyPress = '*')) {
    ret = 100;
  } else if((seqKeyWord == "") && (keyPress == '*')) {
    ret = 200;
  } else if ((getPassword) && (keyPress = '#')) {
    ret = 300;
  }

  Serial.println(seqKeyWord);
  
  return ret;
  
}

void changePass(int type) {

    levelChangePass = 1;
    
    switch ( type )
    {
       case 1 : 
              Serial.println("Alterar senha ativação");
              break;
       case 2 : 
              Serial.println("Alterar senha desativação"); 
              break;
       case 3 : 
              Serial.println("Alterar senha pânico"); 
              break;
    }
     
}

int validPassword(int level) {

    int ret = 0;

    switch ( level )
    {
       case 1 :
              if(passActive == seqKeyWord) {
                Serial.println("Informe a nova senha");
                ret = 2;
              } else {
                Serial.println("Senha inválida");
                Serial.println("Processo de troca abortado");    
              }
              break;
       
       case 2 : 
              newPass = seqKeyWord;
              Serial.println("Confirme a nova senha");
              ret = 3;
              break;
              
       case 3 : 
              if(newPass == seqKeyWord) {
                Serial.println("Senha alterada");
                ret = 4;
              } else {
                Serial.println("Nova senha não confere");
                Serial.println("Processo de troca abortado");   
              }
              break;
    }

    return ret;
  
}

void cleanVariable() {
  seqKeyWord = "";
  action = 0;
}

boolean changeAlarmStatus() {

  boolean ret = false;
  
  if(passActive == seqKeyWord) {
    alarmActive = !alarmActive;
    ret = true;
  } else {
    Serial.println("Senha inválida");
  }

  return ret;
  
}

void levelCheckPass() {
  
  if (levelChangePass == 1) {
    levelChangePass = validPassword(1);
    
  } else if (levelChangePass == 2)  {
    levelChangePass = validPassword(2);
      
  } else if (levelChangePass == 3)  {
    levelChangePass = validPassword(3);
      
    if( levelChangePass == 4 ) {
      passActive = newPass;
    }
      
    levelChangePass = 0;
    newPass = "";
  }

}

void changeAlarm() {

  if( changeAlarmStatus() ) {
    if( alarmActive) {
      Serial.println("Alarme ativado");
    } else {
      Serial.println("Alarme desativado");        
    }
  }
  
}
