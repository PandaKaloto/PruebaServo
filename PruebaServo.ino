#include <Servo.h>

Servo miServo;

//Sensor Ultrasonido
#define trigger 7
#define eco 8

//Motores Derechos
#define der1 2
#define der2 3

//Motores Izquierdos
#define izr1 4
#define izr2 5

#define s0 28
#define s1 30
#define s2 24
#define s3 22
#define out 26

char NOMBRE[21]  = "El Amarillo"; // Nombre de 20 caracteres maximo
char PASS[7]    = "1211";   // PIN O CLAVE de 4 caracteres numericos

int val;

int distanciaC;
int color;
int velocidad = 130;

void setup(void) {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);
  digitalWrite(s0, HIGH);
  digitalWrite(s1, HIGH);

  pinMode (trigger, OUTPUT);
  pinMode (eco, INPUT);

  pinMode(der1, OUTPUT);
  pinMode(der2, OUTPUT);
  pinMode(izr1, OUTPUT);
  pinMode(izr2, OUTPUT);

  Serial1.print("AT");  // Inicializa comando AT
  delay(1000);

  Serial1.print("AT+NAME"); // Configura el nuevo nombre
  Serial1.print(NOMBRE);
  delay(1000);

  Serial1.print("AT+PIN");   // Configura el nuevo PIN
  Serial1.print(PASS);
  delay(1000);

  miServo.attach(6);// Pin donde conectaremos el servo
  val = map(700, 0, 1023, 0, 120);
  miServo.write(val);

}
void loop() {
  Avanzar();
  distanciaC = CalcularDistacia();
  Serial.println(distanciaC);
  if (distanciaC < 20) {
    Parar();
    delay(150);
    //    Serial1.print("Distancia ");
    //    Serial1.print(distanciaC);
    //    Serial1.println(" cm");
    color = Color();
    if (color == 1) {
      //Serial1.print("Rojo, Gira a la Izquierdad");
      Serial1.print("R");
      GirarIzquierda();
      delay(450);
      Parar();
      delay(100);
      Avanzar();
    }
    if (color == 2) {
      //Serial1.print("Azul, Gira a la Derecha");
      Serial1.print("A");
      GirarDerecha();
      delay(450);
      Parar();
      delay(100);
      Avanzar();
    }
    if (color == 3) {
      //Serial1.print("Verde, Llega a la meta");
      Serial1.print("V");
      Parar();
      delay(6000);
    }
  }
}

//Mueve Los Motores Hacia Adelante
void Avanzar() {
  analogWrite(der1, velocidad);
  analogWrite(der2, 0);
  analogWrite(izr1, velocidad);
  analogWrite(izr2, 0);
}
//Mueve Los Motores Hacia Atrás
void Retroceder() {
  analogWrite(der1, 0);
  analogWrite(der2, velocidad);
  analogWrite(izr1, 0);
  analogWrite(izr2, velocidad);
}

//Detiene Los Motores
void Parar() {
  analogWrite(der1, 0);
  analogWrite(der2, 0);
  analogWrite(izr1, 0);
  analogWrite(izr2, 0);
}

//Mueve los motores de la izquierda hacia adelante y los de la derecha hacia atrás
void GirarDerecha() {
  analogWrite(der1, 0);
  analogWrite(der2, velocidad);
  analogWrite(izr1, velocidad);
  analogWrite(izr2, 0);
}

//Mueve los motores de la derecha hacia adelante y los de la izquierda hacia atrás
void GirarIzquierda() {
  analogWrite(der1, velocidad);
  analogWrite(der2, 0);
  analogWrite(izr1, 0);
  analogWrite(izr2, velocidad);
}

//Esta función determina la distacia de un objeto con el sensor ultra sónico
int CalcularDistacia() {
  float duracion, distancia;
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  duracion = pulseIn(eco, HIGH);
  distancia = (duracion / 2) / 29.15;
  return distancia;
}

//Esta función determina el color
int Color() {
  int rojo = 0;
  int verde = 0;
  int azul = 0;
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  rojo = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  digitalWrite(s3, HIGH);
  azul = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  digitalWrite(s2, HIGH);
  verde = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  if (rojo < azul && verde > azul && rojo < 35) {
    Serial.println("   Rojo");
    return 1;
  } else if (azul < rojo && azul < verde && verde < rojo) {
    Serial.println("   Azul");
    return 2;
  } else if (rojo > verde && azul > verde ) {
    Serial.println("   Verde");
    return 3;
  } else {
    Serial.println("    SIN COLOR");
    return 0;
  }
}
