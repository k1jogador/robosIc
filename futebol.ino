#include <SoftwareSerial.h>

// Configuração do Bluetooth
SoftwareSerial serial1(10, 11); // RX no pino 10, TX no pino 11

// Definições de pinos
#define pinMotorDireitoBack 9    // Pino para mover o motor direito para trás
#define pinMotorDireitoFront 10  // Pino para mover o motor direito para frente
#define pinMotorEsquerdoBack 5   // Pino para mover o motor esquerdo para trás
#define pinMotorEsquerdoFront 6  // Pino para mover o motor esquerdo para frente

// Índices para identificar os motores
#define MOTOR_DIREITO 0
#define MOTOR_ESQUERDO 1

// Listas contendo os pinos de cada motor
int pinsMotorDir[2] = {pinMotorDireitoBack, pinMotorDireitoFront}; // Pinos do motor direito
int pinsMotorEsq[2] = {pinMotorEsquerdoBack, pinMotorEsquerdoFront}; // Pinos do motor esquerdo

// Função para controlar os motores
void f_Motor(int ladoMotor, int direcao, int velocidade) {
    int* pins = (ladoMotor == MOTOR_DIREITO) ? pinsMotorDir : pinsMotorEsq;

    analogWrite(pins[direcao], velocidade);      // Define a velocidade no pino da direção
    analogWrite(pins[1 - direcao], 0);           // Garante que o pino oposto esteja desativado
}

void setup() {
    // Configuração dos pinos dos motores como saída
    pinMode(pinMotorDireitoBack, OUTPUT);
    pinMode(pinMotorDireitoFront, OUTPUT);
    pinMode(pinMotorEsquerdoBack, OUTPUT);
    pinMode(pinMotorEsquerdoFront, OUTPUT);

    // Inicialização da comunicação serial com o módulo Bluetooth
    serial1.begin(9600);
}

void loop() {
    if (serial1.available() > 0) {
        String comando = ""; // String para armazenar o comando composto
        delay(20); // Aguarda possíveis comandos adicionais serem recebidos

        while (serial1.available() > 0) {
            comando += (char)serial1.read(); // Lê os caracteres recebidos via Bluetooth
        }

        // Interpretação dos comandos
        bool frente = comando.indexOf('F') != -1;
        bool tras = comando.indexOf('B') != -1;
        bool esquerda = comando.indexOf('L') != -1;
        bool direita = comando.indexOf('R') != -1;

        // Controle do robô baseado nos comandos recebidos
        if (frente) {
            if (esquerda) { // Frente e esquerda
                f_Motor(MOTOR_DIREITO, 1, 255);
                f_Motor(MOTOR_ESQUERDO, 1, 128);
            } else if (direita) { // Frente e direita
                f_Motor(MOTOR_DIREITO, 1, 128);
                f_Motor(MOTOR_ESQUERDO, 1, 255);
            } else { // Apenas frente
                f_Motor(MOTOR_DIREITO, 1, 255);
                f_Motor(MOTOR_ESQUERDO, 1, 255);
            }
        } else if (tras) {
            if (esquerda) { // Trás e esquerda
                f_Motor(MOTOR_DIREITO, 0, 255);
                f_Motor(MOTOR_ESQUERDO, 0, 128);
            } else if (direita) { // Trás e direita
                f_Motor(MOTOR_DIREITO, 0, 128);
                f_Motor(MOTOR_ESQUERDO, 0, 255);
            } else { // Apenas trás
                f_Motor(MOTOR_DIREITO, 0, 255);
                f_Motor(MOTOR_ESQUERDO, 0, 255);
            }
        } else if (esquerda) { // Apenas esquerda (giro no próprio eixo)
            f_Motor(MOTOR_DIREITO, 1, 255);
            f_Motor(MOTOR_ESQUERDO, 0, 255);
        } else if (direita) { // Apenas direita (giro no próprio eixo)
            f_Motor(MOTOR_DIREITO, 0, 255);
            f_Motor(MOTOR_ESQUERDO, 1, 255);
        } else { // Parar caso não tenha comando válido
            f_Motor(MOTOR_DIREITO, 0, 0);
            f_Motor(MOTOR_ESQUERDO, 0, 0);
        }
    }
}
