const int Sensor1=2;
const int Sensor2=3;
const int Sensor3=4;
const int Espera=10000; /*Espera entre eventos en ms*/
const int Hold=100; /*Hold de la señal en estado activo en ms*/
const int Dist1=8;
const int Dist2=13;

void setup() {
  pinMode(Sensor1,OUTPUT);
  pinMode(Sensor2,OUTPUT);
  pinMode(Sensor3,OUTPUT);
  digitalWrite(Sensor1,HIGH);
  digitalWrite(Sensor2,HIGH);
  digitalWrite(Sensor3,HIGH);
}

void loop() {
  delay(Espera);
  digitalWrite(Sensor1,LOW);
  delay(Hold);
  digitalWrite(Sensor1,HIGH);
  /*delay(Dist1);
  digitalWrite(Sensor2,LOW);
  delay(Hold);
  digitalWrite(Sensor2,HIGH);
  delay(Dist2);
  digitalWrite(Sensor3,LOW);
  delay(Hold);
  digitalWrite(Sensor3,HIGH);*/
}
