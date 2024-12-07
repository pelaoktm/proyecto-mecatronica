#include <Servo.h>

// pines
const int servo = 5;
const int servo2 = 11;
const int LED = 7;
const int interruptor = 6;
const int trig = 2;
const int echo = 3;

// Puente H 1
const int ENA1 = 10;
const int IN1_1 = 13;
const int IN2_1 = 12;
const int ENB1 = 9;
const int IN3_1 = 8;
const int IN4_1 = 4;

// Configuración del servo 1
Servo puerta;
const int cerrado = 90;    ///////////////////////////
const int abierto = 0;    //////////////////////////////

// config servo 2
Servo premio;
const int cerrado2 = 90;
const int abierto2 = 0;


// Threshold de distancia
const int threshold = 2; ////////////////////////// // en cm

// Variables
long tiempo;
int distancia;
bool LEDon = true;
bool LEDoff = false;

void setup() {


  // setup pines
  pinMode(LED, OUTPUT);
  pinMode(interruptor, INPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(ENA1, OUTPUT);
  pinMode(IN1_1, OUTPUT);
  pinMode(IN2_1, OUTPUT);
  pinMode(ENB1, OUTPUT);
  pinMode(IN3_1, OUTPUT);
  pinMode(IN4_1, OUTPUT);

  // setup servos
  puerta.attach(servo);
  puerta.write(cerrado);

  premio.attach(servo2);
  premio.write(cerrado2);

  // setup LED
  digitalWrite(LED, LOW);

  // setup monitor serial
  Serial.begin(9600);

}

void loop() {
  // si el switch esta ON:
  if (digitalRead(interruptor) == HIGH) {
    digitalWrite(LED, HIGH); // LED ON

    // monitoreo de distancia
    distancia = sensorDistancia();
    Serial.println(distancia);
    
    if (distancia > threshold) {      // si la distancia es mayor al threshold
      // puerta cerrada
      puerta.write(cerrado);

      // motores OFF
      motoresOFF();
    } else {                 // si la pelota esta frente al sensor
      // la LED parpadea
      digitalWrite(LED, LEDon);
      delay(50); // Parpadeo
      digitalWrite(LED, LEDoff);
      delay(50);
      digitalWrite(LED, LEDon);
      delay(50); // Parpadeo
      digitalWrite(LED, LEDoff);
      delay(50);
      digitalWrite(LED, LEDon);
      delay(50); // Parpadeo
      digitalWrite(LED, LEDoff);
      delay(50);
      digitalWrite(LED, LEDon);

      // CAE PREMIO
      // el motor del alimentador gira con torque elevado
      premio.write(abierto2);  
      delay(1000);       ///////////////////////////////////////// tiempo para que caiga la comida
      premio.write(cerrado2);
  
      delay(4000); // tiempo para que el perro coma

      // la cinta gira a gran velocidad para lanzar la pelota
      cinta(255, true); 
      delay(2000);  // tiempo para que tome inercia
      puerta.write(abierto); // se abre la puerta

      delay(2500);  // chao pelota 
      motoresOFF();

      // Servo a posición cerrada
      puerta.write(cerrado);      // se cierra la puerta
      delay(500);         // no cambiar       // pequeño delay para resetear las lecturas del sensor
    }
  } else {
    // si el interruptor esta en OFF, nada pasa
    motoresOFF();
    puerta.write(cerrado);
    digitalWrite(LED, LOW);         // LED apagada para indicar esto
  }
}

// funcion de distancia
int sensorDistancia() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  tiempo = pulseIn(echo, HIGH);
  return tiempo * 0.034 / 2; // conversion a cm
}

// funcion para apagar motores
void motoresOFF() {
  digitalWrite(IN1_1, LOW);
  digitalWrite(IN2_1, LOW);
  digitalWrite(IN3_1, LOW);
  digitalWrite(IN4_1, LOW);
}

// funcion de los motores de la cinta
void cinta(int velocidad, bool sentido) {
  analogWrite(ENA1, velocidad);
  analogWrite(ENB1, velocidad);
  digitalWrite(IN1_1, sentido);
  digitalWrite(IN2_1, !sentido);
  digitalWrite(IN3_1, sentido);
  digitalWrite(IN4_1, !sentido);
}
