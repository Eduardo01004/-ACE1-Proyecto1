#include <Timer.h>
#include <LedControl.h>
#include <EEPROM.h>

int estado = -1;

//------------------------------------------ CONTROL DE VELOCIDAD DEL MOTOR ------------------------------------------
float distancia = 42 / 8 ;//Es la distancia a la cual le corresponde cada segmento
float velocidad = 42 / 7; //CM/S
float tiempo = (distancia / velocidad) * 1000; //milisegundos por segmento

unsigned long tiempoActual = 0; //Tiempo de Ejecucion
unsigned long tiempoDiferenciaY = 0; //Tiempo que pasara cada vez que presionemos algo
unsigned long tiempoDiferenciaX = 0; //Tiempo que pasara cada vez que presionemos algo

//--------------------------------------------- POSICION DE LA GARRA --------------------------------------------------
int posX = 0;
int posY = 8;


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
int matriz[8][8];

//----------------------------------------------- JOYSTICK ------------------------------------------------------------
#define botonJoystick 52
#define xJoystick A14
#define yJoystick A15

//------------------------------------------------------ MATRIZ DE LEDS --------------------------------------------------------
LedControl lc = LedControl(45, 47, 49, 1); // DIN,CLCS,CS,#


void setup() {
  tiempoActual = millis();
  pinMode(botonJoystick, INPUT);
  Serial.begin(9600);

  // pinMode (ENA, OUTPUT);
  pinMode (ENB, OUTPUT);
  pinMode (ENA, OUTPUT);
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);



  //----------------------------MONEDAS-------------------------
  pinMode(sensor , INPUT);
  pinMode(sensor2 , INPUT);
  pinMode(sensor3 , INPUT);

  //######################### BLUETOOTH ###########################
  Serial1.begin(38400);

  //--------------------------- MATRIZ DE LEDS --------------------
  lc.shutdown(0, false);
  //Intensidad de los les de la matriz con modulo
  lc.setIntensity(0, 8);
  //Limpiamos la matriz con modulo
  lc.clearDisplay(0);

  posY = EEPROM.read(2);
  posX = EEPROM.read(0);

  posX = (posX == 255) ? 0 : posX;

  Serial.println(posX);
  Serial.println(posY);

}

void loop() {
  tiempoActual = millis();
  switch (estado) {
    //---------------------------------------- REGRESAR LA GARRA A SU POSICION ORIGINAL------------------------------------------------

    case -1:
      while (posY < 8 && posY > 0) {
        tiempoActual = millis();
        ejeY = -1;
        ejeX = 0;
        moverMotores();
      }
      ejeY = 0;
      Serial.println("Regresamos la garra en Y");
      while (posX < 8 && posX > 0) {
        tiempoActual = millis();
        ejeX = -1;
        ejeY = 0;
        moverMotores();
      }
      Serial.println("Regresamos la garra en X");
      
      
      estado = 0;
      
      break;
    //-------------------------- MANUAL -----------------------------------------------------
    case 0:
      modoManual();
      break;

    //--------------------------- BLUETOOTH ------------------------------------------------
    case 1:
      modoBluetooth();
      break;


  }
  pintarRecorrido();
  showMatriz();

  /*infrarrojo();
    delay(150);//delay de 50 para que detecte la moneda*/


}

/**
   #########################################################################################################################################
   ############################################################# MATRIZ DE LEDS ###################################################################
   #########################################################################################################################################
*/
void  showMatriz() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (matriz[i][j] == 1) lc.setLed(0, i, j, true);
      else lc.setLed(0, i, j, false);
    }
  }
}

void pintarRecorrido() {
  if (posY == 8 && posX != -1) matriz[7][posX] = 1;
  else if (posX == -1 && posY != 8) matriz[posY][0] = 1;
  else if (posX != -1 && posY != 8) matriz[posY][posX] = 1;

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
   #########################################################################################################################################
   ############################################################# MOTORES ###################################################################
   #########################################################################################################################################
*/
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


void moverMotores() {
  analogWrite(ENB, 50);

  if (ejeY == 1 && posY > 0) {

    digitalWrite (IN3, HIGH);
    digitalWrite (IN4, LOW);
    unsigned long temporal = tiempoActual - tiempoDiferenciaY;
    if (temporal >= tiempo) {
      posY--;
      EEPROM.put(2, posY);
      Serial.println("Arriba");
      tiempoDiferenciaY = tiempoActual;
    }
  }
  else if (ejeY == -1 && posY < 8) {
    digitalWrite (IN3, LOW);
    digitalWrite (IN4, HIGH);
    unsigned long temporal = tiempoActual - tiempoDiferenciaY;
    if (temporal >= tiempo) {
      posY++;
      EEPROM.put(2, posY);
      Serial.println("Abajo");
      tiempoDiferenciaY = tiempoActual;
    }
  }
  else {
    tiempoDiferenciaY = tiempoActual;
    digitalWrite (IN3, LOW);
    digitalWrite (IN4, LOW);

  }

  if (ejeX == 1 && posX < 8) {
    digitalWrite (IN1, HIGH);
    digitalWrite (IN2, LOW);
    unsigned long temporal = tiempoActual - tiempoDiferenciaX;
    if (temporal >= tiempo) {
      posX++;
      EEPROM.put(0, posX);
      Serial.println("Derecha");
      tiempoDiferenciaX = tiempoActual;
    }
  } else if (ejeX == -1 && posX > 0) {
    digitalWrite (IN1, LOW);
    digitalWrite (IN2, HIGH);
    unsigned long temporal = tiempoActual - tiempoDiferenciaX;
    if (temporal >= tiempo) {
      posX--;
      EEPROM.put(0, posX);
      Serial.println("Izquierda");
      tiempoDiferenciaX = tiempoActual;
    }
  }
  else {
    tiempoDiferenciaX = tiempoActual;
    digitalWrite (IN1, LOW);
    digitalWrite (IN2, LOW);
  }



}

/**
   #########################################################################################################################################
   ############################################################# MODO MANUAL ###################################################################
   #########################################################################################################################################
*/

int moverY(int valor) {
  if (valor < 30) return 1;
  else if (valor > 900) return -1;
  return 0;
}

int moverX(int valor) {
  if (valor < 30) return -1;
  else if (valor > 900) return 1;
  return 0;
}


void modoManual( ) {
  ejeX = moverX(analogRead(xJoystick));
  ejeY = moverY(analogRead(yJoystick));
  /*
    Serial.print("X-axis: ");
    Serial.print(x);
    Serial.print(" ");
    Serial.print("Y-axis: ");
    Serial.println(y);
  */
  moverMotores();
}



//####################################################################################################################################
//################################################ FICHAS ############################################################################
//####################################################################################################################################

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
