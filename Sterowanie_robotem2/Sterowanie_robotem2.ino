              //Code by Marcin Jankowski and Kacper Wierciński
#include <QTRSensors.h>

// Parametry czujników odbiciowych
const uint8_t Num_sensors = 6; // Liczba czujników
uint16_t sensorValues[Num_sensors]; // Wartości czujników
int emitterPin = 12;
QTRSensors qtr;

// Parametry regulatora PD
double kp = 0.5;
double kd = 1;
double pv = 0;
double error = 0;     // Uchyb sterowania
double lastError = 0; // Poprzedni uchyb sterowania

// Parametry sterowania silnikami
int manualSpeed = 100;      // Prędkość dla jazdy manualnej
int minSpeed = 60;    // Minimalna prędkość jazdy automatycznej
int leftSpeed = 0;    // Dodatkowa prędkość lewego silnika
int rightSpeed = 0;   // Dodatkowa prędkość prawego silnika
int maxSpeed = 255;   // Maksymalna wartość prędkości

// silnik 1
int dir1PinA = 2;
int dir2PinA = 3;
int speedPinA = 9;
int BluetoothData;  

// silnik 2
int dir1PinB = 5;
int dir2PinB = 4;
int speedPinB = 10;

int d6 = 6;
int d7 = 7;


void setup() {
  Serial.begin(9600);  
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5}, Num_sensors);
  qtr.setEmitterPin(emitterPin);

  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  pinMode(dir1PinA,OUTPUT);
  pinMode(dir2PinA,OUTPUT);
  pinMode(speedPinA,OUTPUT);
  pinMode(dir1PinB,OUTPUT);
  pinMode(dir2PinB,OUTPUT);
  pinMode(speedPinB,OUTPUT);

  sensorCalibration();
}

void loop() {
  uint16_t position = qtr.readLineBlack(sensorValues);

  for (uint8_t i = 0; i < Num_sensors; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print("|");
  }
  Serial.println(position);
  
  comunication(); // Obsługa komunikacji bezprzewodowej

  error = 3500 - position;    // Wyznaczenie uchybu sterowania
  int motorSpeed = kp * error + kd * (error - lastError);
  lastError = error;
  
  leftSpeed = minSpeed + motorSpeed;
  rightSpeed = minSpeed - motorSpeed;

  forwardFollow();
  
  }
  
void comunication()
{
  // delay(200)
  
  BluetoothData = Serial.read();             //przypisz zmiennej BluetoothData odczytane dane z modułu
   
   if(BluetoothData == '1')                      //jeżeli odebrane dane to 1
      {  
       forward();
      }
   
   if(BluetoothData == '0')                      //jeżeli odebrane dane to 0
      {                  
       stop1();      
      }
   if(BluetoothData == '2')                      //jeżeli odebrane dane to 2
      {                  
       backward();      
      }
    if(BluetoothData == '3')                      //jeżeli odebrane dane to 3
      {                  
       turnleft();      
      }
    if(BluetoothData == '4')                      //jeżeli odebrane dane to 4
      {                  
       turnright();      
      }
}

void forwardFollow(int leftSpeed, int rightSpeed)
{
  // Ograniczenie wartości prędkości maksymalnej oraz minimalnej do zakresu 0 - 255
  if(leftSpeed > maxSpeed)
    leftSpeed = maxSpeed;

  if(rightSpeed > maxSpeed)
    rightSpeed = maxSpeed;

  if(leftSpeed < 0)
    leftSpeed = 0;

  if(rightSpeed < 0)
    rightSpeed = 0;
    
  analogWrite(speedPinA, leftSpeed);
  digitalWrite(dir1PinA, LOW);
  digitalWrite(dir2PinA, HIGH);
  analogWrite(speedPinB, rightSpeed);
  digitalWrite(dir1PinB, LOW);
  digitalWrite(dir2PinB, HIGH);
}

void forward(){
  analogWrite(speedPinA, manualSpeed);
  digitalWrite(dir1PinA, LOW);
  digitalWrite(dir2PinA, HIGH);
  analogWrite(speedPinB, manualSpeed);
  digitalWrite(dir1PinB, LOW);
  digitalWrite(dir2PinB, HIGH);
}

void backward(){
  analogWrite(speedPinA, manualSpeed);
  digitalWrite(dir1PinA, HIGH);
  digitalWrite(dir2PinA, LOW);
  analogWrite(speedPinB, manualSpeed);
  digitalWrite(dir1PinB, HIGH);
  digitalWrite(dir2PinB, LOW);

}

void stop1(){
  analogWrite(speedPinA, 0);
  digitalWrite(dir1PinA, LOW);
  digitalWrite(dir2PinA, HIGH);
  analogWrite(speedPinB, 0);
  digitalWrite(dir1PinB, LOW);
  digitalWrite(dir2PinB, HIGH);
}
void turnleft() {
  analogWrite(speedPinA, manualSpeed);
  digitalWrite(dir1PinA, LOW);
  digitalWrite(dir2PinA, HIGH);
  analogWrite(speedPinB, manualSpeed);
  digitalWrite(dir1PinB, HIGH);
  digitalWrite(dir2PinB, LOW);
}
void turnright() {
  analogWrite(speedPinB, manualSpeed);
  digitalWrite(dir1PinB, LOW);
  digitalWrite(dir2PinB, HIGH);
  analogWrite(speedPinA, manualSpeed);
  digitalWrite(dir1PinA, HIGH);
  digitalWrite(dir2PinA, LOW);

}

void sensorCalibration()
{
  for (uint16_t i = 0; i < 400; i++)
  {
    qtr.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW);

 for (uint8_t i = 0; i < Num_sensors; i++)
  {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();
  
  for (uint8_t i = 0; i < Num_sensors; i++)
  {
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);
}

















      
