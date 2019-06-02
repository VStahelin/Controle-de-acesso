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
}

//Cleam this code

void loop() {
  //Serial.println(checkingPresence(rightSensor()));
  //Serial.println(checkingPresence(leftSensor()));
  
  
  if (pass()) {
    Serial.println("Authenticated");
    Serial.println("Open the DOOR");
    servoActionCounterClockwise();
    delay(5000);
    Serial.println("Closing the DOOR");
    servoActionClockwise();
  } else if (reject()) {
    Serial.println("Access denied");
    delay(1000);
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
  if( cm > 10 && cm < 60 ){
    delay(500);
    return true; 
  } else {
    delay(500);
    return false;
  }
}
