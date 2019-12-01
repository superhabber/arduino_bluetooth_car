// Паламарчук Ярослав 14.12.18 

#include <AFMotor.h>
AF_DCMotor motorFrontLeft(2);  //  передний левый
AF_DCMotor motorFrontRight(1); //  передний правый
AF_DCMotor motorRearLeft(3);   //  задний левый
AF_DCMotor motorRearRight(4);  //  задний правый

void dw(int p, int v);

char junk;// змінна для збереження даних

int speed;

#define frontLights A1 // Порт передніх світодіодів
#define backLights A2 // Порт задніх світодіодів

#define extraPin A0 // Порт світодіодів аварійки
int ledState = LOW;  // стан аварійки
long previousMillis = 0; 
long previousMillisE = 0; 

//int speed = 125; // початкова швидкість(макс. 225)
int extra = false; // чи включена аварійка
int checkspeed(); // функція шоб перевірити чи змінилась швидкість
#define interval  250  // інтервал для світодіодів аварійки

void setup(){
  Serial.begin(9600); // встановлення режиму послідовного порту

  // Фари на вихід
  pinMode(frontLights, OUTPUT);
  pinMode(extraPin, OUTPUT);
  pinMode(backLights, OUTPUT);

  motorFrontLeft.setSpeed(255);
  motorFrontRight.setSpeed(255); //  передний правый
  motorRearLeft.setSpeed(255); //  задний левый
  motorRearRight.setSpeed(255);

}


void loop() {
  while (1) {
  speed = 255;
  // якщо прийшла команда
  if (Serial.available()) {
    //зчитуємо дані
    junk = Serial.read();
    // Якщо код вкл пищалки то включаєм на частоті 1000, якщо викл то виключаєм
    if (junk == 'V') tone(13, 10);
     else if (junk == 'v') noTone(13);
    
    // Якщо код аварійки включаєм
    if(junk == 'X') extra = true;  
     else if (junk == 'x') extra = false;
    
    // Код передніх пар
    if (junk == 'W') dw(frontLights, 1);
     else if (junk == 'w')  dw(frontLights, 0);
    
  
    // Код задніх пар
    if (junk == 'U') dw(backLights, 1);
     else if (junk == 'u') dw(backLights, 0);
    
    if (junk == 'F') { //КОМАНДА 'F'= вперед
        motorFrontLeft.run(FORWARD);
        motorFrontRight.run(FORWARD); 
        motorRearLeft.run(FORWARD);
        motorRearRight.run(FORWARD);
    }else if (junk == 'L') {
        motorFrontLeft.run(BACKWARD);
        motorFrontRight.run(FORWARD); 
        motorRearLeft.run(BACKWARD);
        motorRearRight.run(FORWARD);
    }else if (junk == 'S') {
        motorFrontLeft.run(RELEASE);
        motorFrontRight.run(RELEASE); 
        motorRearLeft.run(RELEASE);
        motorRearRight.run(RELEASE);
    }else if (junk == 'B') {
        motorFrontLeft.run(BACKWARD);
        motorFrontRight.run(BACKWARD); 
        motorRearLeft.run(BACKWARD);
        motorRearRight.run(BACKWARD);
    }else if (junk == 'R') {
        motorFrontLeft.run(FORWARD);
        motorFrontRight.run(BACKWARD); 
        motorRearLeft.run(FORWARD);
        motorRearRight.run(BACKWARD);
    }
    
  }
  //код для синхронної аварійки
  unsigned long currentMillis = millis(); 
  if(currentMillis - previousMillis > interval && extra == true) {
    previousMillis = currentMillis; 
    ledState = ledState == 0 ? 1 : 0;
    dw(extraPin, ledState);
    
  }else if(extra == false) dw(extraPin, 0);
  
  } 
}

void dw(int p, int v){
  digitalWrite(p, (v==1) ? HIGH : LOW);
}
  
