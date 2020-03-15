
const int buzzerPin = 7;
int couter = 0;
int entrada1 = 5;
int entrada2 = 6;

//--------------------------------------------------------------------- STARWARS -------------------------------------------------

const int c = 261;
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 466;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int dSH = 622;
const int eH = 659;
const int fH = 698;
const int fSH = 740;
const int gH = 784;
const int gSH = 830;
const int aH = 880;
int counter = 0;
/*-----------------------------------------mario underworld----------------------------------------*/
const int c4 = 262;
const int c5 = 523;
const int a3 = 220;
const int a4 = 440;
const int as3 = 233;
const int as4 = 466;
const int zero = 0;
const int f3 = 175;
const int f4 = 349;
const int d3 = 147;
const int d4 = 294;
const int ds3 = 156;
const int ds4 = 311;
const int cs4 = 277;
const int gs3 = 208;
const int g3 = 196;
const int fs4 = 370;
const int e3 = 165;
const int gs4 = 415;
const int b3 = 247;

void setup() {
  // put your setup code here, to run once:
  //----------------------------- BUZZER --------------------------
  pinMode(buzzerPin, OUTPUT);
  pinMode(entrada1, INPUT);
  pinMode(entrada2, INPUT);

  Serial.begin(9600);

}

void loop() {
  int estado1 = digitalRead(entrada1);
  int estado2 = digitalRead(entrada2);
  if(estado1 == 1 && estado2 == 1) {
    firstSection();
  }
  else if(estado1 == 1 && estado2 == 0){
    marito();
    delay(100);
  }
  Serial.print(estado1);
  Serial.print(" ");
  Serial.println(estado2);
  

}


/**
   #########################################################################################################################################
   ############################################################# MUSICA MARIO ###################################################################
   #########################################################################################################################################

*/

void marito() {
  buzz(c4, 12);
  buzz(c5, 12);
  buzz(a3, 12);
  buzz(a4, 12);
  buzz(as3, 12);
  buzz(as4, 12);
  buzz(zero, 6);
  buzz(zero, 3);

  buzz(c4, 12);
  buzz(c5, 12);
  buzz(a3, 12);
  buzz(a4, 12);
  buzz(as3, 12);
  buzz(as4, 12);
  buzz(zero, 6);
  buzz(zero, 3);

  buzz(f3, 12);
  buzz(f4, 12);
  buzz(d3, 12);
  buzz(d4, 12);
  buzz(ds3, 12);
  buzz(ds4, 12);
  buzz(zero, 6);
  buzz(zero, 3);

  buzz(f3, 12);
  buzz(f4, 12);
  buzz(d3, 12);
  buzz(d4, 12);
  buzz(ds3, 12);
  buzz(ds4, 12);
  buzz(zero, 6);
  buzz(zero, 6);

  buzz(ds4, 18);
  buzz(cs4, 18);
  buzz(d4, 18);
  buzz(cs4, 6);
  buzz(ds4, 6);
  buzz(ds4, 6);
  buzz(gs3, 6);
  buzz(g3, 6);
  buzz(cs4, 6);

  buzz(c4, 18);
  buzz(fs4, 18);
  buzz(f4, 18);
  buzz(e3, 18);
  buzz(as4, 18);
  buzz(a4, 18);

  buzz(gs4, 10);
  buzz(ds4, 10);
  buzz(b3, 10);
  buzz(as3, 10);
  buzz(a3, 10);
  buzz(gs3, 10);
  buzz(zero, 3);
  buzz(zero, 3);
  buzz(zero, 3);


}

void buzz(int nota , int duracion) {
  int tiempo = 1000 / duracion;

  C(nota, tiempo);
  int pausa = tiempo * 1.30;
  delay(pausa);
  C(0, tiempo);






}

void C(long nota, long duracion) {
  long dela = 1000000 / nota / 2;
  long ciclo = nota * duracion / 1000;
  for (long i = 0; i < ciclo; i++) {
    digitalWrite(buzzerPin, HIGH);
    delayMicroseconds(dela);
    digitalWrite(buzzerPin, LOW);
    delayMicroseconds(dela);
  }

}
/**
   #########################################################################################################################################
   ############################################################# MUSICA STARWARS ###################################################################
   #########################################################################################################################################

*/

void firstSection()
{
  beep(a, 500);
  beep(a, 500);
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);

  delay(500);

  beep(eH, 500);
  beep(eH, 500);
  beep(eH, 500);
  beep(fH, 350);
  beep(cH, 150);
  beep(gS, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);

  delay(500);
}

void beep(int note, int duration)
{
  //Play tone on buzzerPin
  tone(buzzerPin, note, duration);


  delay(duration);

  //Stop tone on buzzerPin
  noTone(buzzerPin);

  delay(50);

  //Increment counter
  counter++;
}
