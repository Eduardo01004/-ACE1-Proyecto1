//------------------------------------------- MOTOR EJE Y ----------------------------
int ENB = 3;
int IN3 = 7;
int IN4 = 6;

//-------------------------------------------- MOTOR EJE X ----------------------------
int IN1 = 9;
int IN2 = 8;






//----------------------------------------------- JOYSTICK ------------------------------------------------------------
#define botonJoystick 52
#define xJoystick A14
#define yJoystick A15

void setup() {
  pinMode(botonJoystick, INPUT);
  Serial.begin(9600);

  // pinMode (ENA, OUTPUT);
  pinMode (ENB, OUTPUT);

  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);

}

void loop() {

  
  int x = moverX(analogRead(xJoystick));
  int y = moverY(analogRead(yJoystick));
  Serial.print("X-axis: ");
  Serial.print(x);
  Serial.print(" ");
  Serial.print("Y-axis: ");
  Serial.println(y);
  controlGarra(x, y);
  //delay(100);
  

}



int moverY(int valor) {
  if (valor < 505 && valor >=  0) return 1;
  else if (valor > 506 && valor <= 1023) return -1;
  return 0;
}

int moverX(int valor) {
  if (valor < 492 && valor >= 0) return -1;
  else if (valor > 494 && valor <= 1023) return 1;
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
