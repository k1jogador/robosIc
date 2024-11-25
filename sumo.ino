// Definições de pinos
#define pinMotorDireitoBack 9    // Pino PWM para mover o motor direito para trás
#define pinMotorDireitoFront 10  // Pino PWM para mover o motor direito para frente
#define pinMotorEsquerdoBack 5   // Pino PWM para mover o motor esquerdo para trás
#define pinMotorEsquerdoFront 6  // Pino PWM para mover o motor esquerdo para frent

#define sensor0 A0 // Pino de leitura do Sensor SHARP esquerdo
#define sensor1 A1 // Pino de leitura do Sensor SHARP direito

int sharpEsq;
int sharpDir;

#define tcrt0 13 // pino logico sensor infravermelhor FRENTE ESQUERDA
#define tcrt1 12 // pino logico sensor infravermelhor FRENTE DIREITA
#define tcrt2 8  // pino logico sensor infravermelhor TRAS ESQUERDA
#define tcrt3 7  // pino logico sensor infravermelhor TRAS DIREITA

// Índices para identificar os motores
#define MOTOR_DIREITO 0
#define MOTOR_ESQUERDO 1

// termos param funcao MOTOr
#define TRAS 0
#define FRENTE 1

int tcrts[4] = {tcrt0,tcrt1,tcrt2,tcrt3};
int tcrtsAction[4][2] = { {MOTOR_ESQUERDO , 0} , {MOTOR_DIREITO , 0} , {MOTOR_ESQUERDO , 1} , {MOTOR_DIREITO , 1} };




// listas contendo os pinos de a cada motor
int pinsMotorDir[2] = {pinMotorDireitoBack, pinMotorDireitoFront}; // Pinos do motor direito
int pinsMotorEsq[2] = {pinMotorEsquerdoBack, pinMotorEsquerdoFront}; // Pinos do motor esquerdo


void f_Motor(int ladoMotor, int direcao, int velocidade) {
    // ladoMotor = qual motor será controlado: 0 (esquerdo) ou 1 (direito).
    // direcao qual direcao o motor deve girar: 0 (trás) ou 1 (frente).
    // velocidade do motor (0 à 255)

    // Seleciona o conjunto correto de pinos baseado no lado do motor
    int* pins = (ladoMotor == MOTOR_ESQUERDO) ? pinsMotorEsq : pinsMotorDir;

    // Ativa o pino correspondente à direção e desativa o oposto
    analogWrite(pins[direcao], velocidade);      // Define a velocidade no pino de direção
    analogWrite(pins[abs(1 - direcao)], 0);           // Garante que o pino oposto esteja desativado
}

int calcDistance(int sensor) {
    int leitura = analogRead(sensor);
    int distancia = 10650.08 * pow(leitura, -0.935) - 10;
    return constrain(distancia, 0, 150); // Limita a distância entre 0 e 150 cm
}

void setup() {
    // define os pinos dos motores como saída
    pinMode(pinMotorDireitoBack, OUTPUT);
    pinMode(pinMotorDireitoFront, OUTPUT);
    pinMode(pinMotorEsquerdoBack, OUTPUT);
    pinMode(pinMotorEsquerdoFront, OUTPUT);


    // define os pinos dos sensores como entrada
    pinMode(sensor0, INPUT);
    pinMode(sensor1, INPUT);

    // define os pinos dos sensores como entrada (detecta chao)
    pinMode(tcrt0, INPUT);
    pinMode(tcrt1, INPUT);
    pinMode(tcrt2, INPUT);
    pinMode(tcrt3, INPUT);

    f_Motor(MOTOR_ESQUERDO, 1 , 255);
    f_Motor(MOTOR_DIREITO, 1 , 255);
    delay(500);
    f_Motor(MOTOR_ESQUERDO, 1 , 0);
    f_Motor(MOTOR_DIREITO, 1 , 0);
}



char ultimoLado = 'e';

void radar(){

  if (ultimoLado == 'e'){
    f_Motor(MOTOR_DIREITO , FRENTE , 255);
    f_Motor(MOTOR_ESQUERDO , TRAS , 255);
  } else {
    f_Motor(MOTOR_ESQUERDO , FRENTE , 255);
    f_Motor(MOTOR_DIREITO , TRAS , 255);
  }
   
}



void loop() {
    // Variáveis locais para armazenar as leituras dos sensores
    int sharpEsq = calcDistance(sensor0);
    int sharpDir = calcDistance(sensor1);

    // Constantes de controle
    const int limiteDistancia = 80;
    const int ajuste = 5;

    // Registrar o último lado mais próximo
    if (sharpEsq < sharpDir && sharpEsq <= limiteDistancia) {
        ultimoLado = 'e';
    } else if (sharpDir < sharpEsq && sharpDir <= limiteDistancia) {
        ultimoLado = 'd';
    }

    // Controle de movimentação
    if (sharpEsq <= limiteDistancia || sharpDir <= limiteDistancia) {
        int velocidadeEsq = 255, velocidadeDir = 255;

        if (abs(sharpEsq - sharpDir) > ajuste) {
            if (sharpEsq > sharpDir) velocidadeEsq = max(0, 255 - (sharpEsq - sharpDir));
            else velocidadeDir = max(0, 255 - (sharpDir - sharpEsq));
        }

        f_Motor(MOTOR_ESQUERDO, FRENTE, velocidadeEsq);
        f_Motor(MOTOR_DIREITO, FRENTE, velocidadeDir);
    } else {
        radar();
    }
}
