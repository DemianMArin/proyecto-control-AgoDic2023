#include <Servo.h>   // include servo library to use its related functions
#define Servo_PWM 5  // A descriptive name for D6 pin of Arduino to provide PWM signal
Servo MG995_Servo;   // Define an instance of of Servo with the name of "MG995_Servo"

const int sensorIzqPin = 10;  // El sensor izq irá conectado al pin analógico A0
const int sensorDerPin = 12;  // El sensor derecho irá conectado al pin analógico A1
const int sensorCentralPin = 11;

int IN1 = 8;
int IN2 = 9;
int ENA = 5;

int servo_val = 90;
int step = 5;
void setup() {
  // put your setup code here, to run once:
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  digitalWrite(IN1, LOW);  // rotate forward
  digitalWrite(IN2, HIGH);
  
  Serial.begin(115200);

  pinMode(sensorIzqPin, INPUT);
  pinMode(sensorDerPin, INPUT);
  pinMode(sensorCentralPin, INPUT);
  MG995_Servo.attach(Servo_PWM);  // Connect D6 of Arduino with PWM signal pin of servo motor

}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 60; i < 120; i=i+step) {
    MG995_Servo.write(i);  //Turn right
    delay(100);
  }
  for (int i = 120; i > 60; i=i-step) {
    MG995_Servo.write(i);  //Turn right
    delay(100);
  }

  analogWrite(ENA, 100);
}
