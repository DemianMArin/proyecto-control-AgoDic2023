#include <Servo.h> // include servo library to use its related functions
#define Servo_PWM 5 // A descriptive name for D6 pin of Arduino to provide PWM signal
Servo MG995_Servo;  // Define an instance of of Servo with the name of "MG995_Servo"

// Definición de variables y constantes relacionadas con los sensores IR

//0=blanco, 1=Negro
int sensIzq; // Almacena el valor de la lectura del sensor izquierdo
int sensDer; // Almacena el valor de la lectura del sensor derecho
int sensCent;
const int sensorIzqPin = 10; // El sensor izq irá conectado al pin analógico A0
const int sensorDerPin = 12; // El sensor derecho irá conectado al pin analógico A1
const int sensorCentralPin = 11;
int sens = 0;
int dir = 0;
int servo_val = 90;
int servo_last = 90;
int servo_setPoint = 90;

int lecturaSensorIR();

int IN1=8;
int IN2=9;
int ENA=6;
const int encoderPinA = 2;
const int encoderPinB = 3;

// Variables for tracking encoder state
volatile long encoderCount = 0;
volatile long lastEncoderCount = 0;
unsigned long lastUpdateTime = 0;
unsigned long lastUpdateSpeed = 0;
float dt = 100;
int changeSpeedTim = 100;
int sense = 1;
int pwm = 30;
int pwm_r = 30;
char buf[12];
int bufR[3];

// Variables for calculating speed
float speed = 0.0;

void setup()
{
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);  
  pinMode(ENA,OUTPUT);
  
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderPinA), updateEncoder, CHANGE);
  
  Serial.begin(115200);

  digitalWrite(IN1,LOW);// rotate forward
  digitalWrite(IN2,HIGH);

  pinMode( sensorIzqPin  , INPUT) ;
  pinMode( sensorDerPin  , INPUT) ;
  pinMode( sensorCentralPin  , INPUT) ;
  MG995_Servo.attach(Servo_PWM);  // Connect D6 of Arduino with PWM signal pin of servo motor
  analogWrite(ENA,pwm);

}

void loop()
{  
// Commented section: to change the pwm signal over time
//  unsigned long currentTimSpeed = millis();
//  if(currentTimSpeed - lastUpdateSpeed >= changeSpeedTim){
//    analogWrite(ENA,pwm); //motor speed
////    Serial.print("New pwm: ");
////    Serial.println(pwm);
//    if(pwm >= 254 || pwm <= 70)
//      sense *= -1;
//    pwm+=sense;
//    lastUpdateSpeed = millis();
//  }

  
 if(Serial.available()){
   pwm_r = Serial.parseInt();
 }
 if(pwm_r != 0 && pwm_r != pwm){
   pwm = pwm_r;
//    Serial.println(pwm);
   analogWrite(ENA,pwm);
 }

 
  // Calculate speed every 1000 ms (1 second)
  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime >= dt) {
    noInterrupts(); // Disable interrupts to safely read encoderCount
    speed = (encoderCount - lastEncoderCount) / dt; // Calculate speed (counts per second)
    lastEncoderCount = encoderCount;
    lastUpdateTime = currentTime;
    interrupts(); // Re-enable interrupts
    
    buf[0] = (currentTime/10000)%10 + '0';
    buf[1] = (currentTime/1000)%10 + '0';
    buf[2] = '.';
    buf[3] = (currentTime/100)%10 + '0';
    buf[4] = (int)(speed) + '0';
    buf[5] = '.';
    buf[6] = ((int)(speed*10.0))%10 + '0';
    buf[7] = ((int)(speed*100.0))%10 + '0';
    buf[8] = (pwm/100) + '0';
    buf[9] = (pwm/10)%10 + '0';
    buf[10] = (pwm%10) + '0';
    buf[11] = '/';
    Serial.write(buf,12);
    Serial.println();
  }

  int sens = lecturaSensorIR(); // Se lee el valor de los sensores IR
  switch(sens){
    case 1:
      dir = 2;
      break;
    case 2:
      dir = 0;
      break;
    case 3:
      dir = 1;
      break;
    case 4:
      dir = -2;
      break;
    case 6:
      dir = -1;
      break;
    case 7:
      dir = 0;
      break;
  }
  servo_val = map(dir, -2, 2, 60, 100);
  if(servo_last != servo_val){
    MG995_Servo.write(servo_val); //Turn right
  }
  servo_last = servo_val;

}

void updateEncoder() {
  // This function is called when the encoder state changes
  if (digitalRead(encoderPinB) == digitalRead(encoderPinA))
    encoderCount++;
  else
    encoderCount--;
}

int lecturaSensorIR(){
  sensIzq = digitalRead(sensorIzqPin); // Almacena la lectura del sensor izquierdo
  sensDer = digitalRead(sensorDerPin); // Almacena la lectura del sensor derecho
  sensCent = digitalRead(sensorCentralPin);
  return (sensIzq << 2) | (sensCent << 1) | sensDer;
}