// Паламарчук Ярослав 14.12.18 

void dw(int p, int v);

char junk;// змінна для збереження даних
#define IN1 7 // ініціалізація виводів підключення драйвера моторів L298 до Arduino
#define IN2 6
#define IN3 5
#define IN4 4
#define ENA 9
#define ENB 3

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
  pinMode(7, OUTPUT);// встановлення режиму роботи портів на вихід
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(13, OUTPUT);

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
    
    if (junk == 'B') { //КОМАНДА 'F'= вперед
      dw(IN1, 0); //Встановлює зворотний напрям ДВ� ГУНА Каналу А
      dw(IN2, 1); //
      analogWrite(ENA, speed); // двигун на Каналі А МАЄ половинУ швидкості
      dw (IN3, 1);//лівий мотор вперед
      dw (IN4, 0);
      analogWrite(ENB, speed); // двигун на Канал B МАЄ половинУ швидкості
    }else if (junk == 'L') { //'L'= вліво
      dw(IN1, 1); //Встановлює передній напрям Каналу А
      dw(IN2, 0); //
      analogWrite(ENA, 145); // двигун на Каналі А повним ходом
      dw(IN3, 1); //Встановлює передній напрям Каналу B
      dw(IN4, 0); //
      analogWrite(ENB, 145); // двигун на Канал B повним ходом
    }else if (junk == 'S') { // 'S'= СТОП
      dw(ENA, 0);; //СТОП канал А
      dw(ENB, 0); //СТОП канал B
    }else if (junk == 'F') { // 'B'= назад
      dw(IN1, 1); //Встановлює напрям ВПЕРЕД Каналу А
      dw(IN2, 0); //
      analogWrite(ENA, speed); // двигун на Каналі А повним ходом
      dw(IN3, 0); //Встановлює напрям НАЗАД У КаналІ B
      dw(IN4, 1); //
      analogWrite(ENB, speed); // двигун на Канал B половина швидкості
    }else if (junk == 'R') { // 'R'= вправо
      // Робот рухається НАЗАД
      dw (IN2, 1);//правий мотор назад
      dw (IN1, 0);
      analogWrite(ENA, 145); // двигун на Каналі А повним ходом
      dw (IN4, 1);//лівий мотор назад
      dw (IN3, 0);
      analogWrite(ENB, 145); // двигун на КаналІ B повним ходом
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
