// Definições de pinos para os motores
#define pinMotorDireitoBack 9    // Pino para mover o motor direito para trás
#define pinMotorDireitoFront 10  // Pino para mover o motor direito para frente
#define pinMotorEsquerdoBack 5   // Pino para mover o motor esquerdo para trás
#define pinMotorEsquerdoFront 6  // Pino para mover o motor esquerdo para frente

// Índices para identificar os motores
#define MOTOR_DIREITO 0
#define MOTOR_ESQUERDO 1

// listas contendo os pinos de a cada motor
int pinsMotorDir[2] = {pinMotorDireitoBack, pinMotorDireitoFront}; // Pinos do motor direito
int pinsMotorEsq[2] = {pinMotorEsquerdoBack, pinMotorEsquerdoFront}; // Pinos do motor esquerdo


void f_Motor(int ladoMotor, int direcao, int velocidade) {
    // ladoMotor = qual motor será controlado: 0 (direito) ou 1 (esquerdo).
    // direcao qual direcao o motor deve girar: 0 (trás) ou 1 (frente).
    // velocidade do motor (0 à 255)

    // Seleciona o conjunto correto de pinos baseado no lado do motor
    int* pins = (ladoMotor == MOTOR_DIREITO) ? pinsMotorDir : pinsMotorEsq;

    // Ativa o pino correspondente à direção e desativa o oposto
    analogWrite(pins[direcao], velocidade);      // Define a velocidade no pino de direção
    analogWrite(pins[1 - direcao], 0);           // Garante que o pino oposto esteja desativado
}

void setup() {
    // define os pinos do motor direito como saída
    pinMode(pinMotorDireitoBack, OUTPUT);
    pinMode(pinMotorDireitoFront, OUTPUT);

    // define os pinos do motor esquerdo como saída
    pinMode(pinMotorEsquerdoBack, OUTPUT);
    pinMode(pinMotorEsquerdoFront, OUTPUT);
}

void loop() {
    f_Motor(MOTOR_DIREITO, 1, 128); // Motor direito, frente, metade da velocidade
    f_Motor(MOTOR_ESQUERDO, 0, 128); // Motor esquerdo, trás, metade da velocidade
    // ou seja, o robo gira em seu proprio eixo

    delay(1000); // Aguarda 1 segundo

    f_Motor(MOTOR_DIREITO, 0, 255); // Motor direito, trás, velocidade máxima
    f_Motor(MOTOR_ESQUERDO, 1, 128); // Motor esquerdo, frente, metade da velocidade
    delay(1000); // Aguarda 1 segundo

    // Para ambos os motores
    f_Motor(MOTOR_DIREITO, 0, 0); // Para o motor direito
    f_Motor(MOTOR_ESQUERDO, 0, 0); // Para o motor esquerdo
    delay(1000); // Aguarda 1 segundo
}
