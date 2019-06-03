#define passButton 2
#define rejectButton 3
#include <Servo.h>
#define SERVO 6               // Data servo
Servo s;                      // Variável Servo
int pos;                      // Posição Servo
int rightTrigPin = 8;             // Trigger
int righEchoPin = 9;             // Echo
int leftTrigPin = 11;             // Trigger
int leftEchoPin = 12;             // Echo
long duration, cm;              // Distancia
int movimentation[1];
int movimentationDirection [1];

void setup() {
  Serial.begin (9600);
  pinMode(rightTrigPin, OUTPUT);
  pinMode(righEchoPin, INPUT);
  pinMode(leftTrigPin, OUTPUT);
  pinMode(leftEchoPin, INPUT);
  pinMode(passButton, INPUT_PULLUP);
  pinMode(rejectButton, INPUT_PULLUP);

  s.attach(SERVO);
  s.write(0); 
  movimentation[0] = 0;
  movimentation[1] = 0;
}

//TODO: Clean/refactor the code and translate everything into English
//TODO: People counter

void loop() {
  if (pass()) {
    Serial.println("Authenticated");
    Serial.println("Open the DOOR");
    servoActionCounterClockwise();
    delay(1000);
    orientation();
    while ( movimentationDirection[0] == 0 ) {
        if( movimentationDirection[1] == 1 ){
          Serial.println("Someone Entered"); 
          movimentationDirection[0] = 1;
        } else if( movimentationDirection[1] == 2 ) {
          Serial.println("Someone Left"); 
          movimentationDirection[0] = 1;
        } else {
          orientation();
        }
    }
    delay(5000);
    Serial.println("Closing the DOOR");
    servoActionClockwise();
  } else if (reject()) {
    Serial.println("Access denied");
    delay(1000);
  }
}

void orientation(){
  movimentation[0] = 0;
  movimentation[1] = 0;
  
  if(checkingPresence(rightSensor())){
    Serial.println("Direita");
    movimentation[0] = 1;
    delay(50);
    if(checkingPresence(leftSensor())){
      Serial.println("Esquerda");
      movimentation[1] = 2;
    }
    delay(100);
  }else if(checkingPresence(leftSensor())){
    Serial.println("Left");
    movimentation[0] = 2;
    delay(50);
    if(checkingPresence(rightSensor())){
      Serial.println("Rigth");
      movimentation[1] = 1;
    }
    delay(500);
  }
  
  if ( movimentation[0] == 0 && movimentation[1] == 0){
    Serial.println(" No one passed ");
    movimentationDirection[0] = 0; // Verificador se realmente houve passagem
    movimentationDirection[1] = 0; // Indicador do sentido da passagem, 1 = Direita, 2 = Esquerda
    
  } else {
    movimentationDirection[0] = 0;
    if( movimentation[0] == 1 && movimentation[1] == 2 ){
      movimentationDirection[1] = 1;
    }
    if( movimentation[0] == 2 && movimentation[1] == 1 ){
      movimentationDirection[1] = 2; 
    } 
  }
}

bool pass() {
   #define tempoDebounce 50 //(tempo para eliminar o efeito Bounce EM MILISEGUNDOS)
   bool estadoBotao;
   static bool estadoBotaoAnt; 
   static bool estadoRet = true;
   static unsigned long delayBotao = 0;
   if ( (millis() - delayBotao) > tempoDebounce ) {
       estadoBotao = digitalRead(passButton);
   }
   return !estadoBotao;
}

bool reject() {
   #define tempoDebounce 50 //(tempo para eliminar o efeito Bounce EM MILISEGUNDOS)
   bool estadoBotao;
   static bool estadoBotaoAnt; 
   static bool estadoRet = true;
   static unsigned long delayBotao = 0;
   if ( (millis() - delayBotao) > tempoDebounce ) {
       estadoBotao = digitalRead(rejectButton);
   }
   return !estadoBotao;
}

void servoActionCounterClockwise(){
  for(pos = 0; pos < 180; pos++){
    s.write(pos);
    delay(3);
  }
}
void servoActionClockwise(){
  for(pos = 180; pos >= 0; pos--){
    s.write(pos);
    delay(15);
  }
}

long rightSensor(){
  // Criando os pulsos do sensor ultrasônico
  digitalWrite(rightTrigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(rightTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(rightTrigPin, LOW);
  // Lendo o tempo da respostas em cada pulso
  pinMode(righEchoPin, INPUT);
  duration = pulseIn(righEchoPin, HIGH);
  // Convertendo o tempo em distancia
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343

  return cm;
}

long leftSensor(){
  // Criando os pulsos do sensor ultrasônico
  digitalWrite(leftTrigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(leftTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(leftTrigPin, LOW);
  // Lendo o tempo da respostas em cada pulso
  pinMode(leftEchoPin, INPUT);
  duration = pulseIn(leftEchoPin, HIGH);
  // Convertendo o tempo em distancia
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343

  return cm;
}

boolean checkingPresence(long distance){
  if( cm > 0 && cm < 60 ){
    delay(250);
    return true; 
  } else {
    delay(250);
    return false;
  }
}
