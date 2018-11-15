const int Sensor1=2;
const int Sensor2=3;
const int Sensor3=4;
const int Espera=10000; /*Espera entre eventos en ms*/
const int Hold=100; /*Hold de la señal en estado activo en ms*/
const int velsound=331;
const int Dist1=2.45; /*En metros*/
const int Dist2=4.12; /*En metros*/
int Dist1_aux;
int Dist2_aux;

int Disttosec(float Param){
  int result;
  result=Param/velsound;
  if(result>=5){
    result++;
  } else {
  }
  return result;
}

void setup() {
  pinMode(Sensor1,OUTPUT);
  pinMode(Sensor2,OUTPUT);
  pinMode(Sensor3,OUTPUT);
  digitalWrite(Sensor1,HIGH);
  digitalWrite(Sensor2,HIGH);
  digitalWrite(Sensor3,HIGH);
  Dist1_aux=Disttosec(Dist1);
  Dist2_aux=Disttosec(Dist2);
}

void loop() {
  delay(Espera);
  digitalWrite(Sensor1,LOW);
  delay(Hold);
  digitalWrite(Sensor1,HIGH);
  delay(Dist1_aux);
  digitalWrite(Sensor2,LOW);
  delay(Hold);
  digitalWrite(Sensor2,HIGH);
  delay(Dist2_aux);
  digitalWrite(Sensor3,LOW);
  delay(Hold);
  digitalWrite(Sensor3,HIGH);
}
