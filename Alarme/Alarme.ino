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

boolean changePassActive = false;
boolean newPassword = false;
boolean confirmNewPassword = false;

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
      
    } else if (( action == 100 ) && (!newPassword)) {
      newPassword = validPassword(1);
      
    } else if (( action == 100 ) && (newPassword) && (!confirmNewPassword))  {
      confirmNewPassword = validPassword(2);
      
    } else if (( action == 100 ) && (newPassword) && (confirmNewPassword))  {
      confirmNewPassword = validPassword(3);
      
      if( confirmNewPassword ) {
        passActive = newPass;
      }
      
      confirmNewPassword = false;
      changePassActive = false;
      newPassword = false;
      newPass = "";
      
    } else if (action == 200) {
      getPassword = true;
      
    } else if (action == 300) {
      if( changeAlarmStatus() ) {
        if( alarmActive) {
          Serial.println("Alarme ativado");
        } else {
          Serial.println("Alarme desativado");        
        }
      }
      
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
  } else if ((changePassActive) && (keyPress = '*')) {
    ret = 100;
  } else if((seqKeyWord == "") && (keyPress == '*')) {
    ret = 200;
  } else if ((getPassword) && (keyPress = '#')) {
    ret = 300;
  }
  
  return ret;
  
}

void changePass(int type) {

    changePassActive = true;
    
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

boolean validPassword(int level) {

    boolean ret = true;

    switch ( level )
    {
       case 1 :
              ret = (passActive == seqKeyWord);
              if(passActive == seqKeyWord) {
                Serial.println("Informe a nova senha");
              } else {
                Serial.println("Senha inválida");
                Serial.println("Processo de troca abortado");    
              }
              break;
       
       case 2 : 
              newPass = seqKeyWord;
              Serial.println("Confirme a nova senha");
              break;
              
       case 3 : 
              ret = (newPass == seqKeyWord);
              if(newPass == seqKeyWord) {
                Serial.println("Senha alterada");
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

