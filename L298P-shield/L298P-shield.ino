// Паламарчук Ярослав 14.12.18 

void dw(int p, int v);

char junk;// змінна для збереження даних
int E1 = 10;
int M1 = 12;
int E2 =11;
int M2 = 13;

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
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);

  // Фари на вихід
  pinMode(frontLights, OUTPUT);
  pinMode(extraPin, OUTPUT);
  pinMode(backLights, OUTPUT);

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
      dw(M1, 1); 
      dw(M2, 1); 
      analogWrite(E1, speed); 
      analogWrite(E2, speed); 
    }else if (junk == 'L') { //'L'= вліво
      dw(M1, 0); 
      dw(M2, 1); 
      analogWrite(E1, 195); 
      analogWrite(E2, 195);
    }else if (junk == 'S') { // 'S'= СТОП
      analogWrite(E1, 0); 
      analogWrite(E2, 0);
    }else if (junk == 'B') { // 'B'= назад
      dw(M1, 0); 
      dw(M2, 0); 
      analogWrite(E1, speed); 
      analogWrite(E2, speed);
    }else if (junk == 'R') { // 'R'= вправо
      dw(M1, 1); 
      dw(M2, 0); 
      analogWrite(E1, 195); 
      analogWrite(E2, 195);
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
