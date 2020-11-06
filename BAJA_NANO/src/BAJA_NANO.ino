#include "Nextion.h"
#include "max6675.h"
#define thermoDO 12
#define thermoCS 9
#define thermoCLK 13
#define rotacaoPin 3  //3
#define veloPin 2     //2
#define raio 0.467// fala fiot

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

int vel = 0, rot = 0;
int pvel = 0, prot = 0;
int velo1 = 0, velo2 = 0;
int temperatura;
unsigned long temp1 = 0, temp2 = 0, temp3 = 0;
bool isPage1 = true;
char txt0[10],
     txt1[10],
     txt2[10];


NexText txtVelo               = NexText(1, 1, "t0");
NexText txtRpm                = NexText(1, 2, "t1");
NexText txtTemp               = NexText(1, 3, "t2");

NexPage page0 = NexPage(0, 0, "page0");
NexPage page1 = NexPage(1, 0, "page1");

void setup()
{
  pinMode(rotacaoPin, INPUT);
  pinMode(veloPin, INPUT);
  //Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(rotacaoPin), bordarot, RISING);
  attachInterrupt(digitalPinToInterrupt(veloPin), bordavel, RISING);
  temp1 = millis();
  nexInit();

  delay(3000);
  page1.show();
  isPage1 = 1;
}

void loop()
{ 
 if(isPage1){
   if (millis() - temp1 >= 1000) {
    temp1 = millis();
    calculaTemp();  
    calculaRpm();
    calculaVelo();
   }
 }  
}

void bordavel(void) {
  pvel++;
}

void bordarot (void) {
  prot++;
}

void calculaTemp(){
  if (millis() - temp2 > 500) {
    temp2 = millis();
    temperatura = thermocouple.readCelsius();
    
    memset(txt2, 0, sizeof(txt2));         //Transforma inteiros para string
    itoa(temperatura, txt2, 10);           //
    txtTemp.setText(txt2);
  }
}

void calculaRpm(){
  detachInterrupt(rotacaoPin);
  rot = (prot * 60);

 // Serial.println("Rotacao: ");
  //Serial.println(rot);

  
  memset(txt1, 0, sizeof(txt1));
  itoa(rot, txt1, 10); 
  txtRpm.setText(txt1);
  
  prot=0;
}

void calculaVelo(){
  velo1 = calculaVelo1();
  delay(250);
  velo2 = calculaVelo2();

  vel = velo1 + velo2 / 2;
  
  memset(txt0, 0, sizeof(txt0));
  itoa(vel, txt0, 10); 
  txtVelo.setText(txt0);

  pvel=0;
}

int calculaVelo1(){
  detachInterrupt(veloPin);
  velo1 = (pvel * 2 * PI * raio)*3.6;
  return velo1;
}

int calculaVelo2(){
  detachInterrupt(veloPin);
  velo2 = (pvel * 2 * PI * raio)*3.6;
  return velo2;
}
