int IN1=8;
int IN2=9;
int ENA=5;
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
int pwm = 0;
int pwm_r = 0;
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
  if(pwm_r != 0){
    pwm = pwm_r;
    Serial.println(pwm);
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
}

void updateEncoder() {
  // This function is called when the encoder state changes
  if (digitalRead(encoderPinB) == digitalRead(encoderPinA))
    encoderCount++;
  else
    encoderCount--;
}