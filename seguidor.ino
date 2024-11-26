// Definições de pinos para os motores
#define pinMotorDireitoBack 9    // Pino para mover o motor direito para trás
#define pinMotorDireitoFront 10  // Pino para mover o motor direito para frente
#define pinMotorEsquerdoBack 5   // Pino para mover o motor esquerdo para trás
#define pinMotorEsquerdoFront 6  // Pino para mover o motor esquerdo para frente

// Definições de pinos para os sensores
#define sensorEsquerdoExtremo A0   // Sensor extremo esquerdo
#define sensorEsquerdo A1          // Sensor esquerdo
#define sensorCentral A2           // Sensor central
#define sensorDireito A3           // Sensor direito
#define sensorDireitoExtremo A4    // Sensor extremo direito

// Índices para identificar os motores
#define MOTOR_DIREITO 0
#define MOTOR_ESQUERDO 1

// Listas contendo os pinos de cada motor
int pinsMotorDir[2] = {pinMotorDireitoBack, pinMotorDireitoFront}; // Pinos do motor direito
int pinsMotorEsq[2] = {pinMotorEsquerdoBack, pinMotorEsquerdoFront}; // Pinos do motor esquerdo

void f_Motor(int ladoMotor, int direcao, int velocidade) {
    // ladoMotor = qual motor será controlado: 0 (direito) ou 1 (esquerdo).
    // direcao = qual direcao o motor deve girar: 0 (trás) ou 1 (frente).
    // velocidade do motor (0 à 255)

    // Seleciona o conjunto correto de pinos baseado no lado do motor
    int* pins = (ladoMotor == MOTOR_DIREITO) ? pinsMotorDir : pinsMotorEsq;

    // Ativa o pino correspondente à direção e desativa o oposto
    analogWrite(pins[direcao], velocidade);      // Define a velocidade no pino de direção
    analogWrite(pins[1 - direcao], 0);           // Garante que o pino oposto esteja desativado
}

void setup() {
    // Define os pinos dos motores como saída
    pinMode(pinMotorDireitoBack, OUTPUT);
    pinMode(pinMotorDireitoFront, OUTPUT);
    pinMode(pinMotorEsquerdoBack, OUTPUT);
    pinMode(pinMotorEsquerdoFront, OUTPUT);

    // Define os pinos dos sensores como entrada
    pinMode(sensorEsquerdoExtremo, INPUT);
    pinMode(sensorEsquerdo, INPUT);
    pinMode(sensorCentral, INPUT);
    pinMode(sensorDireito, INPUT);
    
    pinMode(sensorDireitoExtremo, INPUT);
}

void loop() {
    // Valor lido pelos sensores
    int valorEsquerdoExtremo = analogRead(sensorEsquerdoExtremo);
    int valorEsquerdo = analogRead(sensorEsquerdo);
    int valorCentral = analogRead(sensorCentral);
    int valorDireito = analogRead(sensorDireito);
    int valorDireitoExtremo = analogRead(sensorDireitoExtremo);

    int limiar = 500; // Limiar para diferenciar branco de preto, pode ter que ser alterado dependendo do sensor!!!!

    // Vê se os sensores captaram menos luz que 500
    bool esqExt = (valorEsquerdoExtremo < limiar);
    bool esq = (valorEsquerdo < limiar);
    bool centro = (valorCentral < limiar);
    bool dir = (valorDireito < limiar);
    bool dirExt = (valorDireitoExtremo < limiar);

    // Movimento baseado nos sensores
    if (centro) {
        // Segue em frente se o sensor central está na linha
        f_Motor(MOTOR_DIREITO, 1, 128);
        f_Motor(MOTOR_ESQUERDO, 1, 128);
    } else if (esqExt && dirExt) {
        // Se ambos sensores extremos detectam linha, o robô pode estar em um cruzamento ou zigue-zague
        f_Motor(MOTOR_DIREITO, 1, 200);  // Aumenta a velocidade do motor direito
        f_Motor(MOTOR_ESQUERDO, 1, 200); // Aumenta a velocidade do motor esquerdo
    } else if (esqExt) {
        // Desvio grande para a esquerda: motor direito para frente, motor esquerdo para trás (sensor extrema esqueda detecta linha)
        f_Motor(MOTOR_DIREITO, 1, 200);
        f_Motor(MOTOR_ESQUERDO, 0, 100);
    } else if (dirExt) {
        // Desvio grande para a direita: motor esquerdo para frente, motor direito para trás (sensor extrema direita detecta a linha)
        f_Motor(MOTOR_DIREITO, 0, 100);
        f_Motor(MOTOR_ESQUERDO, 1, 200);
    } else if (esq) {
        // Ajusta para a esquerda (sensor esquerdo detecta linha)
        f_Motor(MOTOR_DIREITO, 1, 160);
        f_Motor(MOTOR_ESQUERDO, 1, 100);
    } else if (dir) {
        // Ajusta para a direita (sensor direito detecta linha)
        f_Motor(MOTOR_DIREITO, 1, 100);
        f_Motor(MOTOR_ESQUERDO, 1, 160);
    } else {
        // Se nenhum sensor detecta linha, para os motores
        f_Motor(MOTOR_DIREITO, 1, 0);
        f_Motor(MOTOR_ESQUERDO, 1, 0);
    }
}
