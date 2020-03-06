#include <Timer.h>
int estado = 1;

//------------------------------------------- MOTOR EJE Y ----------------------------
int ENB = 10;
int IN3 = 7;
int IN4 = 6;

//-------------------------------------------- MOTOR EJE X ----------------------------
int ENA = 11;
int IN1 = 9;
int IN2 = 8;


int ejeY = 0;
int ejeX = 0;



//---------------------------------------------MONEDAS---------------------------------
int sensor = 46;//1er infrarrojo 1Q
int sensor2 = 48; //0.50 centavos
int sensor3 = 50; //0.25 centavos
double total = 0;

//----------------------Matriz de led
int columnas[8] = {25, 33, 32, 28, 35, 27, 23, 22};
int filas[8] = {29, 24, 37, 26, 30, 36, 31, 34};

int tristeza [8][8] = {
  {0, 1, 1, 1, 1, 1, 1, 0},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 1, 0, 0, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 1, 1, 0, 0, 1},
  {1, 0, 1, 0, 0, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {0, 1, 1, 1, 1, 1, 1, 0}
};

//----------------------------------------------- JOYSTICK ------------------------------------------------------------
#define botonJoystick 52
#define xJoystick A14
#define yJoystick A15

Timer temporal;
void setup() {
  pinMode(botonJoystick, INPUT);
  Serial.begin(9600);

  // pinMode (ENA, OUTPUT);
  pinMode (ENB, OUTPUT);
  pinMode (ENA, OUTPUT);
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);

  for (int i = 0; i < 8; i++)pinMode(columnas[i], OUTPUT);
  for (int i = 0; i < 8; i++)pinMode(filas[i], OUTPUT);


  //----------------------------MONEDAS-------------------------
  pinMode(sensor , INPUT);
  pinMode(sensor2 , INPUT);
  pinMode(sensor3 , INPUT);

  //######################### BLUETOOTH ###########################
  Serial1.begin(38400);


}

void loop() {
  switch (estado) {
    //-------------------------- MANUAL -----------------------------------------------------
    case 0:
      break;

    //--------------------------- BLUETOOTH ------------------------------------------------
    case 1:
      modoBluetooth();
      break;


  }
  //moverMotores();
  //int x = moverX(analogRead(xJoystick));
  //int y = moverY(analogRead(yJoystick));
  /*
    Serial.print("X-axis: ");
    Serial.print(x);
    Serial.print(" ");
    Serial.print("Y-axis: ");
    Serial.println(y);
  */
  // controlGarra(x, y);
  //delay(100);
  //showMatriz();

  /*infrarrojo();
    delay(150);//delay de 50 para que detecte la moneda*/


}

void  showMatriz() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (tristeza[i][j] == 1) digitalWrite(filas[j], LOW);
      else digitalWrite(filas[j], HIGH);
    }
    digitalWrite(columnas[i], HIGH);
    delay(1);
    digitalWrite(columnas[i], LOW);
  }
}



/**
   ##############################################################################################################################################
   ######################################################## MODO BLUETOOTH #####################################################################
   #############################################################################################################################################
*/



void modoBluetooth() {
  String entrada = "";
  if (Serial1.available() > 0) {
    char c = Serial1.read();
    Serial.println(c);
    controlarMovimiento(c);
  }


  if (Serial.available()) {
    char c = Serial.read();
    Serial1.write(c);
  }

  moverMotores();
}


/**
   0 -> Parar
   1 -> Adelante
   2 -> Atras
   3-> Derecha
   4 -> Izquierda
*/
void controlarMovimiento(char c) {
  switch (c) {
    //---------------------------------------------- Motores Apagados
    case '0':
      ejeX = 0;
      ejeY = 0;
      break;
    //----------------------------------------------- ADELANTE -----------------
    case '1':
      ejeY = 1;
      ejeX = 0;
      break;
    //----------------------------------------------- ATRAS ------------------------
    case '2':
      ejeY = -1;
      ejeX = 0;
      break;
    //---------------------------------------------- DERECHA ----------------------------------
    case '3':
      ejeY = 0;
      ejeX = 1;
      break;
    //----------------------------------------------- IZQUIERDA ----------------------------------
    case '4':
      ejeY = 0;
      ejeX = -1;
      break;
  }
}





/**
   #########################################################################################################################################
   ############################################################# MOTORES ###################################################################
   #########################################################################################################################################
*/

void moverMotores() {
  analogWrite(ENB, 50);
  if (ejeY == 1) {
    digitalWrite (IN3, HIGH);
    digitalWrite (IN4, LOW);
  }
  else if (ejeY == -1) {
    digitalWrite (IN3, LOW);
    digitalWrite (IN4, HIGH);

  }
  else {
    digitalWrite (IN3, LOW);
    digitalWrite (IN4, LOW);

  }

  if (ejeX == 1) {
    digitalWrite (IN1, HIGH);
    digitalWrite (IN2, LOW);
  } else if (ejeX == -1) {
    digitalWrite (IN1, LOW);
    digitalWrite (IN2, HIGH);
  }
  else {
    digitalWrite (IN1, LOW);
    digitalWrite (IN2, LOW);
  }



}



int moverY(int valor) {
  if (valor ==  0) return 1;
  else if (valor == 1023) return -1;
  return 0;
}

int moverX(int valor) {
  if (valor == 0) return -1;
  else if (valor == 1023) return 1;
  return 0;
}


void controlGarra(int x, int y ) {
  if (y == 1) {
    digitalWrite (IN3, HIGH);
    digitalWrite (IN4, LOW);
  } else if (y == -1) {
    digitalWrite (IN3, LOW);
    digitalWrite (IN4, HIGH);
  } else {
    digitalWrite (IN3, LOW);
    digitalWrite (IN4, LOW);
  }




  if (x == 1) {
    digitalWrite (IN1, HIGH);
    digitalWrite (IN2, LOW);
  } else if (x == -1) {
    digitalWrite (IN1, LOW);
    digitalWrite (IN2, HIGH);
  } else {
    digitalWrite (IN1, LOW);
    digitalWrite (IN2, LOW);
  }
}



void Moneda_100() {
  int valor;
  valor = digitalRead(sensor);
  double precio = 1;

  if (valor == 0) {
    total = total + precio;
  } else {

  }
}

void Moneda_50() {
  int valor = digitalRead(sensor2);
  //Serial.println(valor);
  double precio = 0.50;

  if (valor == 0) {
    total = total + precio;
    Serial.println(total);
  } else {

  }
}

void Moneda_25() {
  int valor = digitalRead(sensor3);
  //Serial.println(valor);
  double precio = 0.25;

  if (valor == 0) {
    total = total + precio;
  } else {

  }

}
void infrarrojo() {
  Moneda_25();
  Moneda_50();
  Moneda_100();

  if (total == 1) {
    Serial.println("Se inserto la cantidad de Q1.00, Ya puede jugar");
  }
  else if ( total > 1) {
    Serial.println("Se supero la cantidad de dinero");
    Serial.print("Total: ");
    Serial.println(total);
  } else {
    Serial.print("No se ha completado el total todavia ");
    Serial.print("Total: ");
    Serial.println(total);
  }
}
