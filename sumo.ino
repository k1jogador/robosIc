// Definições de pinos
#define pinMotorDireitoBack 11   // Pino PWM para mover o motor direito para trás
#define pinMotorDireitoFront 9   // Pino PWM para mover o motor direito para frente
#define pinMotorEsquerdoBack 10  // Pino PWM para mover o motor esquerdo para trás
#define pinMotorEsquerdoFront 6  // Pino PWM para mover o motor esquerdo para frent

#define sensor0 A0  // Pino de leitura do Sensor SHARP esquerdo
#define sensor1 A1  // Pino de leitura do Sensor SHARP direito

#define infraFEsq A2  // pino logico sensor infravermelhor FRENTE ESQUERDA
#define infraFDir A3  // pino logico sensor infravermelhor FRENTE DIREITA
#define infraT A4     // pino logico sensor infravermelhor TRAS

// Índices para identificar os motores
#define MOTOR_DIREITO 0
#define MOTOR_ESQUERDO 1

// Termos para funcao MOTOR
#define TRAS 0
#define FRENTE 1

int sharpEsq;
int sharpDir;

#define LIMITE_OPONENTE 170
#define AJUSTE 5

void moverMotor(int motor, int direcao, int velocidade) {
  if (motor == MOTOR_DIREITO) {
    if (direcao == FRENTE) {
      analogWrite(pinMotorDireitoFront, velocidade);
      analogWrite(pinMotorDireitoBack, 0);

    } else {
      analogWrite(pinMotorDireitoFront, 0);
      analogWrite(pinMotorDireitoBack, velocidade);
    }
  } else if (motor == MOTOR_ESQUERDO) {
    if (direcao == FRENTE) {
      analogWrite(pinMotorEsquerdoFront, velocidade);
      analogWrite(pinMotorEsquerdoBack, 0);
    } else {
      analogWrite(pinMotorEsquerdoFront, 0);
      analogWrite(pinMotorEsquerdoBack, velocidade);
    }
  }
}

// Função de radar (mapeamento de oponente)
void radar() {
  moverMotor(MOTOR_DIREITO, FRENTE, 255);
  moverMotor(MOTOR_ESQUERDO, TRAS, 255);

void setup() {
  // Define os pinos dos motores como saída
  pinMode(pinMotorDireitoBack, OUTPUT);
  pinMode(pinMotorDireitoFront, OUTPUT);
  pinMode(pinMotorEsquerdoBack, OUTPUT);
  pinMode(pinMotorEsquerdoFront, OUTPUT);

  // Define os pinos dos sensores como entrada
  pinMode(sensor0, INPUT);
  pinMode(sensor1, INPUT);
  pinMode(infraFEsq, INPUT);
  pinMode(infraFDir, INPUT);
  pinMode(infraT, INPUT);

  // Energiza os sensores de baixo
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
}

int muitoProx = 0;
bool canto = false;

void loop() {
  // Lê os valores dos sensores
  delay(50);
  moverMotor(MOTOR_DIREITO, FRENTE, 0);
  moverMotor(MOTOR_ESQUERDO, TRAS, 0);

  int sharpEsq = analogRead(sensor0);
  int sharpDir = analogRead(sensor1);

  bool iFE = digitalRead(infraFEsq);
  bool iFD = digitalRead(infraFDir);
  bool iT = digitalRead(infraT);

  if (!iFE || !iFD) {
    canto = true;
    moverMotor(MOTOR_DIREITO, TRAS, 255);
    moverMotor(MOTOR_ESQUERDO, TRAS, 255);
    delay(100);
  }
  if (!iT) {
    canto = true;
    moverMotor(MOTOR_DIREITO, FRENTE, 255);
    moverMotor(MOTOR_ESQUERDO, FRENTE, 255);
    delay(100);
  }

  if (canto == false) {
    if ((sharpEsq < 170) && (sharpDir < 170)) {
      moverMotor(MOTOR_DIREITO, FRENTE, 255);
      moverMotor(MOTOR_ESQUERDO, TRAS, 255);
    } else {
      // Controle de movimentação
      if (sharpEsq > LIMITE_OPONENTE && sharpDir > LIMITE_OPONENTE) {
        int velocidadeEsq = 255;
        int velocidadeDir = 255;

        if (sharpEsq < (sharpDir + 40)) {
          velocidadeEsq = 200;

        } else if (sharpDir < (sharpEsq + 40)) {
          velocidadeDir = 200;
        }

        if (muitoProx >= 200) {
          Serial.print(muitoProx);
          moverMotor(MOTOR_DIREITO, TRAS, 255);
          moverMotor(MOTOR_ESQUERDO, TRAS, 255);
          delay(300);
          muitoProx = 0;
        }
        if ((sharpEsq > 320) || (sharpDir > 320)) {
          muitoProx = muitoProx + 1;
          Serial.print("p");
        } else {
          muitoProx = 0;
        }

        moverMotor(MOTOR_DIREITO, FRENTE, velocidadeDir);
        moverMotor(MOTOR_ESQUERDO, FRENTE, velocidadeEsq);
      }
    }
  }
  canto = false;
}
