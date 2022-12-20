#include <Servo.h>

#define DIRC 12    // Gira motor 2 en sentido horario
#define DIRD 11   // Gira motor 2 en sentido contrahorario
#define MOT2 10   // Habilita motor 2
#define MOT1 9   // Habilita motor 1
#define DIRB 8    // Gira motor 1 en sentido contrahorario
#define DIRA 7    // Gira motor 1 en sentido horario

#define TRIG 6    // Pin TRIG del sensor ultrasónico
#define ECHO 5    // Pin ECHO del sensor ultrasónico

const float sonido = 34300.0;  // velocidad del sonido en cm/s

const float lejos = 100.0;    // 1 metro
const float medio = 50.0;   // 50 centímetros
const float cerca = 10.0;   // 10 centímetros

Servo volante;

int distancia_R = 0, distancia_L = 0, distancia_centro = 0;

int medir(){
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);  
  unsigned long tiempo = pulseIn(ECHO, HIGH);  
  float distancia = tiempo * 0.000001 * sonido / 2.0;
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial. println(" cm");
  return (int)distancia;
}

void encender(){
  analogWrite(MOT1, 255);
  analogWrite(MOT2, 255);  
}

void apagar(){
  digitalWrite(MOT1, LOW);
  digitalWrite(MOT2, LOW);
}

void avance() {
  // Derecha
  digitalWrite(DIRA, HIGH);
  digitalWrite(DIRB, LOW); // Derecha avance
  // Izquierda
  digitalWrite(DIRC, HIGH);
  digitalWrite(DIRD, LOW); // Izquierda avance
}

void reversa(){
  // Derecha
  digitalWrite(DIRA, LOW); // Derecha reversa
  digitalWrite(DIRB, HIGH);
  // Izquierda
  digitalWrite(DIRC, LOW); // Izquierda reversa
  digitalWrite(DIRD, HIGH);  
}

void derecha(){
  // Derecha
  digitalWrite(DIRA, HIGH); // Derecha reversa
  digitalWrite(DIRB, LOW);
  // Izquierda
  digitalWrite(DIRC, LOW);
  digitalWrite(DIRD, HIGH); // Izquierda avance
}

void izquierda(){
  // Derecha
  digitalWrite(DIRA, LOW); // Derecha reversa
  digitalWrite(DIRB, HIGH);
  // Izquierda
  digitalWrite(DIRC, HIGH);
  digitalWrite(DIRD, LOW); // Izquierda avance
}

void setup(){
  /* Pines de motores [7-12] como salida */
  for (int pin = 7; pin <= 12; pin++) {
    pinMode(pin, OUTPUT);
  }

  pinMode(TRIG, OUTPUT);    // Pin TRIG como salida
  pinMode(ECHO, INPUT);     // Pin ECHO como entrada

  volante.attach(4);
  volante.write(90);
  
  Serial.begin(9600);     // Inicia monitor de serie
}

void loop(){
  
  volante.write(90); // Centro
  Serial.println("Centro");
  distancia_centro = medir();
  delay(2000);
  
  if (distancia_centro < cerca){
    reversa();
    delay(1000);
    apagar();
    delay(500);
    
    volante.write(0);  // Derecha
    Serial.println ("Derecha");
    distancia_R = medir();
    delay(2000);
    
    volante.write(180); // Izquierda
    Serial. println("Izquierda");
    distancia_L = medir();
    delay(2000);  
  
    volante.write(90); // Centro
    delay(2000);
    
    if (distancia_L < distancia_R){
      encender();
      derecha();
      delay(500);
      }
    else if (distancia_L > distancia_R){
      encender();
      izquierda();
      delay(500);
      }
    else if ((distancia_L < cerca) || (distancia_R < cerca)) {
      reversa();
      delay(200);
      }
    else {
      encender();
      avance();
      }
    }
  else {
    encender();
    avance();
  }
}