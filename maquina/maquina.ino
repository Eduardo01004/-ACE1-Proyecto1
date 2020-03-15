#include <Timer.h>
#include <LedControl.h>
#include <EEPROM.h>
#include <Servo.h>

//----------------------------------------------- BUZZER --------------------------------------------------------------
const int salida1 = 22;//entrada1
const int salida2 = 23;

int estado = -1;

//------------------------------------------ CONTROL DE VELOCIDAD DEL MOTOR ------------------------------------------
float distancia = 26.5 / 8 ;//Es la distancia a la cual le corresponde cada segmento
//------------------------------------------------- VELOCIDAD PARA ATRAS --------------------------------------
float velocidad = 26.5 / 4; //CM/S
float tiempo = (distancia / velocidad) * 1000; //milisegundos por segmento
//-------------------------------------------------- VELOCIDAD PARA ADELANTE ---------------------------------------------
float velocidadY = 26.5 / 3.9; //CM/S
float tiempoY = (distancia / velocidadY) * 1000; //milisegundos por segmento

//-------------------------------------------------- VELOCIDAD PARA IZQUIERDA ---------------------------------------------
float velocidadX = 26.5 / 5.5; //CM/S
float tiempoX = (distancia / velocidadX) * 1000; //milisegundos por segmento

//-------------------------------------------------- VELOCIDAD PARA DERECHA ---------------------------------------------
float velocidadXX = 26.5 / 3.3; //CM/S
float tiempoXX = (distancia / velocidadXX) * 1000; //milisegundos por segmento


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
int IN1 = 9;
int IN2 = 8;

//-------------------------------------- MOTOR GARRA ----------------------------------
int IN1G = 24;
int IN2G = 25;
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
LedControl lc = LedControl(45, 47, 49, 1); // DIN,CLK,CS,#


//---------------------------------------------------------- TOBOGAN -----------------------------------------------------------
Servo servo;
Servo servo2;

int pinServo1 = 28;
int pinServo2 = 29;
int sensorTobo = 4;


//-------------------------------------------------------- BOTON AGARRAR ------------------------------------------------------
int botonAgarrar = 26;
//---------------------------------------------------------BOTON SOLTAR --------------------------------------------------------
int botonSoltar = 27;
void setup() {
  tiempoActual = millis();
  pinMode(botonJoystick, INPUT);
  Serial.begin(9600);

  //----------------------------------------------------------- TOBOGAN -----------------------------------------------------------------
  servo.attach(pinServo1);
  servo2.attach(pinServo2);
  pinMode(sensorTobo, INPUT);

  // pinMode (ENA, OUTPUT);
  pinMode (ENB, OUTPUT);
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
  pinMode (IN1G, OUTPUT);
  pinMode (IN2G, OUTPUT);

  //-----------------------------------------------------SALIDAS A MUSICA------------------------------------------------------------
  pinMode(salida1, OUTPUT);
  pinMode(salida2, OUTPUT);


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
  //-------------------------------------------------- Boton Agarrar--------------------------------------------------
  pinMode(botonAgarrar, INPUT);

  //-------------------------------------------------- Boton Soltar---------------------------------------------------
  pinMode(botonSoltar, INPUT);
}

void loop() {

  tiempoActual = millis();
  switch (estado) {
    //---------------------------------------- REGRESAR LA GARRA A SU POSICION ORIGINAL------------------------------------------------

    case -1:
      while (posY < 8 && posY >= 0) {
        tiempoActual = millis();
        ejeY = -1;
        ejeX = 0;
        moverMotores();
        if (posY >= 8) break;
      }

      ejeY = 0;
      ejeX = 0;
      posY = 8;
      Serial.println("Regresamos la garra en Y");
      digitalWrite (IN3, LOW);
      digitalWrite (IN4, LOW);

      while (posX <= 8 && posX > 0) {
        tiempoActual = millis();
        ejeX = -1;
        ejeY = 0;
        moverMotores();
      }
      Serial.println("Regresamos la garra en X");
      digitalWrite (IN1, LOW);
      digitalWrite (IN2, LOW);
      ejeY = 0;
      ejeX = 0;
      posX = 0;
      estado = 0;
      digitalWrite(salida1, LOW);
      digitalWrite(salida2, LOW);
      break;

    //---------------------------- ESPERANDO LA CANTIDAD DE MONEDAS ------------------------
    case 0:
      controlMonedas();
      break;
    //--------------------------- STANDBY ESPERANDO UN MODO DE JUEGO ----------------------
    case 1:
      escojerModo();
      break;
    //-------------------------- MANUAL -----------------------------------------------------
    case 2:
      modoManual();
      break;

    //--------------------------- BLUETOOTH ------------------------------------------------
    case 3:
      modoBluetooth();
      break;

    case 4:
      ganar();
      break;

  }


  pintarRecorrido();
  showMatriz();

  /*infrarrojo();
    delay(150);//delay de 50 para que detecte la moneda*/


}



/**
   #########################################################################################################################################
   ############################################################# TOBOGAN ###################################################################
   #########################################################################################################################################
*/

void ganar() {
  servo.write(180);
  servo2.write(90);
  delay(2000);
  servo.write(90);
  servo2.write(180);
  Serial.println("Ganaste Puto");

  estado = -1;
  total = 0;
}


/**
   #########################################################################################################################################
   ############################################################# JUEGO ###################################################################
   #########################################################################################################################################
*/

void game() {
  moverMotores();

  if (Serial.available()) {
    char c = Serial.read();
    Serial1.write(c);
  }
}

/**
   #########################################################################################################################################
   ############################################################# ESCOGER MODO ###################################################################
   #########################################################################################################################################
*/
void escojerModo() {
  String entrada = "";
  if (Serial1.available() > 0) {
    char c = Serial1.read();
    Serial.println(c);
    if (c == '0') {
      //estadoMusica = 2;
      Serial.println("Modo Manual");
      estado = 2; //Modo Manual
      digitalWrite(salida1, HIGH);
      digitalWrite(salida2, LOW);
    }
    else if (c == '1') {
      Serial.println("Modo Bluetooth");
      estado = 3; //Modo Bluetooth
      digitalWrite(salida1, HIGH);
      digitalWrite(salida2, LOW);
    }
  }


  if (Serial.available()) {
    char c = Serial.read();
    Serial1.write(c);
  }

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

  int k = (posX == 8) ? 7 : posX;
  int j = (posY == 8) ? 7 : posY;
  matriz[j][k] = 1;



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

  game();
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

    //--------------------------------------------- PERDER ----------------------------------------------
    case '5':
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          matriz[i][j] = tristeza[i][j];
        }
      }
      showMatriz();
      delay(3000);
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          matriz[i][j] = 0;
        }
      }
      total = 0;
      estado = -1;
      break;

    //----------------------------------------------- AGARRAR -----------------------------------------------
    case '6':
      agarrar();
      break;

    //------------------------------------------------ SOLTAR -----------------------------------------------
    case '7':
      soltar();
      break;

    case '8':
      ganar();
      break;
  }
}


void moverMotores() {
  analogWrite(ENB, 100);

  if (ejeY == 1) {

    digitalWrite (IN3, HIGH);
    digitalWrite (IN4, LOW);
    unsigned long temporal = tiempoActual - tiempoDiferenciaY;
    if (temporal >= tiempoY && posY > 0) {
      posY--;
      EEPROM.put(2, posY);
      Serial.println("Arriba");
      tiempoDiferenciaY = tiempoActual;
    }
  }
  else if (ejeY == -1) {
    digitalWrite (IN3, LOW);
    digitalWrite (IN4, HIGH);
    unsigned long temporal = tiempoActual - tiempoDiferenciaY;
    if (temporal >= tiempo && posY < 8) {
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

  if (ejeX == 1) {
    digitalWrite (IN1, HIGH);
    digitalWrite (IN2, LOW);
    unsigned long temporal = tiempoActual - tiempoDiferenciaX;
    if (temporal >= tiempoXX && posX < 8) {
      posX++;
      EEPROM.put(0, posX);
      Serial.println("Derecha");
      tiempoDiferenciaX = tiempoActual;
    }
  } else if (ejeX == -1) {
    digitalWrite (IN1, LOW);
    digitalWrite (IN2, HIGH);
    unsigned long temporal = tiempoActual - tiempoDiferenciaX;
    if (temporal >= tiempoX && posX > 0) {
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
  game();

  int estado = digitalRead(botonAgarrar);
  if (estado == 1) {
    Serial.println("Agarrar");
    agarrar();
    delay(200);
  }
  estado = digitalRead(botonSoltar);
  if (estado == 1) {
    Serial.println("Soltar");
    soltar();
    delay(200);
  }
}


//####################################################################################################################################
//################################################ GARRA ############################################################################
//####################################################################################################################################
void agarrar() {
  digitalWrite(IN1G, LOW);
  digitalWrite(IN2G, HIGH);
  delay(350);
  digitalWrite(IN1G, LOW);
  digitalWrite(IN2G, LOW);
  delay(3000);
  digitalWrite(IN1G, HIGH);
  digitalWrite(IN2G, LOW);
  delay(490);
  digitalWrite(IN1G, LOW);
  digitalWrite(IN2G, LOW);
  delay(90);
}


void soltar() {
  digitalWrite(IN1G, LOW);
  digitalWrite(IN2G, HIGH);
  delay(350);
  digitalWrite(IN1G, LOW);
  digitalWrite(IN2G, LOW);
  delay(500);
  digitalWrite(IN1G, HIGH);
  digitalWrite(IN2G, LOW);
  delay(490);
  digitalWrite(IN1G, LOW);
  digitalWrite(IN2G, LOW);
  delay(90);
}

//####################################################################################################################################
//################################################ FICHAS ############################################################################
//####################################################################################################################################
void controlMonedas() {
  if (Serial.available()) {
    char c = Serial.read();
    Serial1.write(c);
    if (c == '2') {
      Serial.println("Pasamos a StandBy");
      estado = 1; // Standby
      digitalWrite(salida1, HIGH);
      digitalWrite(salida2, HIGH);
      delay(100);
      digitalWrite(salida1, LOW);
      digitalWrite(salida2, LOW);
    }

  }

  int resultado = Moneda_100();
  if (resultado == 1) {
    Serial1.write('2'); // Q1.00
    Serial1.write('\n');
  }
  resultado = Moneda_50();
  if (resultado == 1) {
    Serial1.write('1'); // Q0.50
    Serial1.write('\n');
  }
  resultado = Moneda_25();
  if (resultado == 1) {
    Serial1.write('0');// Q0.25
    Serial1.write('\n');
  }

  if (total >= 1) {
    estado = 1;
    digitalWrite(salida1, HIGH);
    digitalWrite(salida2, HIGH);
    delay(100);
    digitalWrite(salida1, LOW);
    digitalWrite(salida2, LOW);
  }

}


int Moneda_100() {
  int valor;
  valor = digitalRead(sensor);
  double precio = 1;
  if (valor == 0) {
    total = total + precio;
    Serial.println("Moneda Q1.00");
    delay(50);
    return 1;
  } else  return 0;
}

int Moneda_50() {
  int valor = digitalRead(sensor2);
  //Serial.println(valor);
  double precio = 0.50;

  if (valor == 0) {
    total = total + precio;
    Serial.println("Moneda Q0.50");
    delay(200);
    return 1;
  } else return 0;
}

int Moneda_25() {
  int valor = digitalRead(sensor3);
  //Serial.println(valor);
  double precio = 0.25;

  if (valor == 0) {
    total = total + precio;
    Serial.println("Moneda Q0.25");
    delay(50);
    return 1;
  } else return 0;

}
