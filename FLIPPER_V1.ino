/*
MONTAGEM, PROGRMAÇÃO E FUNCIONAMENTO:

A ESCULTURA É MOVIMENTADA POR TRÊS PLACAS ARDUINO, PLACAS 0, 1 E 2.
AS PLACAS 0 E 1 MOVIMENTAM 12 MOTORES CADA, A PLACA 2 MOVIMENTA OS SEIS ÚLTIMOS MOTORES
TODOS OS POSICIONAMENTOS INFORMADOS À SEGUIR CONSIDERAM A VISÃO DA OBRA PELO ÂNGULO FRONTAL:

A PLACA 0 CONTROLA TODOS OS MOTORES DAS FILEIRAS 1 E 2 DE AMPULHETAS, AS MAIS AO FUNDO E MAIS ALTAS.
A PLACA 1 CONTROLA TODOS OS MOTORES DAS FILEIRAS 3 E 4 DE AMPULHETAS
A PLACA 2 CONTROLA TODOS OS MOTORES DA FILEIRA 5 DE AMPULHETAS, AS MAIS BAIXAS E MAIS À FRENTE

###AVISOS###
***DURANTE TODO O PROCESSO AS LIGAÇÕES E PROGRAMAÇÕES DEVEM SER REALIZADAS COM TODO O SISTEMA DESLIGADO DA REDE ELÉTRICA**
***EM CASO DE QUALQUER ANORMALIDADE PERCEBIDA, A OBRA DEVERÁ SER IMEDIATAMENTE DESLIGADA DA REDE ELÉTRICA E A FALHA CORRIGIDA, SOB PENA DE DANO À OBRA***
***ESSA OBRA POSSUI FONTE BIVOLT REGULADA PARA RECEBER 110 VOLTS, PARA LIGA-LA EM 220 VOLTS É NECESSÁRIO MUDAR A CHAVE SELETORA DENTRO DA FONTE DE ALIMENTAÇÃO***
###AVISOS###

###CONEXÕES###

OS PINOS DE CONTROLE DOS MOTORES DEVEM SER CONECTADOS EM SEQUÊNCIA OS PINOS DOS MOTORES DEVEM SER LIGADOS EM SEQUENCIA NAS PLACAS ARDUINOS:

PLACA 0
O MOTOR MAIS A ESQUERDA DA FILEIRA 1 (FUNDO) DEVE SER CONECTADO AO PINO 2 DA PLACA 0, OS DEMAIS DA FILEIRA SÃO LIGADOS NOS PINOS VIZINHOS, ATÉ O PINO 7
O MOTOR MAIS A ESQUERDA DA FILEIRA 2 DEVE SER CONECTADO AO PINO 8 DA PLACA 0, OS DEMAIS DA FILEIRA SÃO LIGADOS NOS PINOS VIZINHOS, ATÉ O PINO 13

PLACA 1
O MOTOR MAIS A ESQUERDA DA FILEIRA 3 DEVE SER CONECTADO AO PINO 2 DA PLACA 1, OS DEMAIS DA FILEIRA SÃO LIGADOS NOS PINOS VIZINHOS, ATÉ O PINO 7
O MOTOR MAIS A ESQUERDA DA FILEIRA 4 DEVE SER CONECTADO AO PINO 8 DA PLACA 0, OS DEMAIS DA FILEIRA SÃO LIGADOS NOS PINOS VIZINHOS, ATÉ O PINO 13

PLACA 2
O MOTOR MAIS A ESQUERDA DA FILEIRA 3 DEVE SER CONECTADO AO PINO 2 DA PLACA 2, OS DEMAIS DA FILEIRA SÃO LIGADOS NOS PINOS VIZINHOS, ATÉ O PINO 7

###PROGRAMAÇÃO DAS PLACAS###

CADA PLACA DEVE SER PROGRAMADA COM ESSE PROGRAMA UTILIZANDO A ARDUINO IDE, A PLACA ARDUINO DEVE SER ESTAR CONECTADA AO COMPUTADOR VIA CABO USB E A PORTA APROPRIADA DEVE ESTAR SELECIONADA PARA SUBIR O PROGRAMA ÀS PLACAS
ANTES DE ENVIAR O PROGRAMA À PLACA, O VALOR DA VARIÁVEL PLACA (IDENTIFICADA LOGO ABAIXO DESSA SESSÃO DE INSTRUÇÕES) DEVE CORRESPONDER AO NÚMERO DA PLACA SENDO PROGRAMADA

EXEMPLO: AO PROGRAMAR A PLACA 1, A LINHA QUE ATUALMENTE CONTÊM "byte PLACA = 0;" deverá ser modificada para "byte PLACA = 1;", PARA A PLACA 2 "byte PLACA = 2;", PARA A PLACA 0  "byte PLACA = 0;"
DESENVOLVI O CÓDIGO DE MANEIRA QUE A MUDANÇA DESSA VARIÁVEL SEJA SUFICIENTE PARA DISPARAR TODOS OS COMPORTAMENTOS INDIVIDUAIS DE CADA PLACA, SEM MUDANÇAS ADICIONAIS.

###PREPARAÇÃO DAS AMPULHETAS PARA LIGAÇÃO DA OBRA###
COM A OBRA POSICIONADA, COM AS AMPULHETAS JÁ MONTADAS E ESTÁVEIS (SEM BALANÇAR), DEVE-SE CUIDADOSAMENTE GIRAR TODAS AS AMPULHETAS MANUALMENTE DE FORMA QUE O FIO DE NYLON QUE AS SUSTENTA ESTEJA VISIVEL NO LADO FRONTAL DE CADA AMPULHETA.
DEVE-SE CHEGAR SE AS POLIAS E PARAFUSOS ESTÃO FIRMEMENTE ROSQUEADAS A CADA UM DOS MOTORES, PARA QUE NÃO GIREM EM FALSO.

###LIGAÇÃO DA OBRA###
APÓS O TERMINO DE TODOS OS PROCEDIMENTOS ANTERIORES E DA ESTABILIZAÇÃO DE TODAS AS AMPULHETAS, A OBRA DEVERÁ SER LIGADA À REDE ELÉTRICA ***110 VOLTS***
DEVE-SE OBSERVAR A OBRA POR AO MENOS 15 MINUTOS PARA VERIFICAR SE TODOS OS MOTORES ESTÃO GIRANDO ADEQUADAMENTE, SE NENHUMA AMPULHETA ESTÁ COM O CABO ENRROSCADO OU OUTRA ANORMALIDADE.
*/

//###NUMERO DA PLACA - MUDAR PARA SINALIZAR QUAL PLACA ESTÁ SENDO PROGRAMADA###
byte PLACA = 0;
//#####################


#include <Servo.h>

//número total de motores
#define NumTotalMotores 30

//variável para acessar o timer interno de millis
extern volatile unsigned long timer0_millis;

byte NumMotores = 12; //define numero de motores por arduino
byte offsetServo = 500; // ms de diferença por servo
long tempobaseServo = 240000; // tempo base de giro em ms (4 minutos)

//tempos de motor ativo em cada linha
int tempoAtivoLinha = 1600;

//inicializado em setup
int tempoAtivoServo[NumTotalMotores]; // quanto tempo o servo fica girando em ms

byte NMPPlaca = 12; // número total de motores por placa - usado para calcular offset

//Intervalos de espera entre as placas

Servo servos[12]; // Array de motores servo
byte servoPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}; // Pinos dos servos
byte servoCentro[] = {94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 90}; // Calibragem de centro
int servoDirecao[12] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; // Direções iniciais
long tempoServo[12]; // será inicializado em setup
long ultRotacaoServo[12]; // registra a última vez que o servo iniciou a rotação
bool servoGirando[12]; // status de rotação do servo

byte velocidade = 30; //VELOCIDADE DE ROTAÇÃO

void setup() {
  

  Serial.begin(115200);
  Serial.println("Começou!");

  //ajusta o numero de motores para a placa 2
  if (PLACA ==2) {NumMotores = 6;}

  // inicializa o array com o tempo de cada servo, este é o intervalo entre um movimento e outro
  // tempoServo[3] => tempo que o servo 3 espera até girar de novo
  for (byte i = 0; i < NumMotores; i++) {
    //PLACA * NumMotores -> Offset entre um arduino e outro | offsetServo = tempo de atraso por servo
    tempoServo[i] = tempobaseServo + ( ((PLACA*NMPPlaca) + i ) * offsetServo);
  }

  for (byte i = 0; i < NumTotalMotores; i ++)
  {
    //todos os servos com tempo ativo igual 
    tempoAtivoServo[i] = tempoAtivoLinha;
  }

  //inicializa e gira motores para alinhar as ampulhetas
  InicializaMotores();
  //inicializa millis faltando 2 segundos para o primeiro giro
  SetaMillis (238000);
}

void loop() {
  unsigned long currentMillis = millis();
  
  for (byte i = 0; i < NumMotores; i++) {
    if (!servoGirando[i] && (currentMillis - ultRotacaoServo[i]) >= tempoServo[i]) {
      // Start moving the servo
      servos[i].write(servoCentro[i] + (velocidade * servoDirecao[i]));
      ultRotacaoServo[i] = currentMillis; // atualiza tempo da ultima rotação
      servoGirando[i] = true; // Marca como rodando
      //Serial.print ("Servo "); Serial.print (i); Serial.println (" iniciou giro.");
    }

    //acessa o tempo do servo atual dependendo da placa em que ele está
    int tempoAtivoServoAtual;
    tempoAtivoServoAtual = tempoAtivoServo[(PLACA*12) + i];
    
    // Check if the servo should return to center
    if (servoGirando[i] && (currentMillis - ultRotacaoServo[i]) >= tempoAtivoServoAtual) {
      servos[i].write(servoCentro[i]); // Para rotação
      ultRotacaoServo[i] = currentMillis; // atualiza tempo da ultima rotação
      servoGirando[i] = false; // Marca como parado
      servoDirecao[i] = servoDirecao[i] * (-1); // Alterna direção
      //Serial.print ("        Servo "); Serial.print (i); Serial.println (" finalizou giro.");
    }
  }
}

void InicializaMotores() {

  //Inicialização dos motores ligando com os pinos
  Serial.println("Inicializando...");
  for (byte i = 0; i < NumMotores; i++) {
    servos[i].attach(servoPins[i]); // Conecta os pinos dos servos
    //servos[i].write(servoCentro[i] - velocidade);
  }

  const char ltr[] PROGMEM = "UVhWMGIzSnBZVG9nUThPcGMyRnlJRUYxWjNWemRHOGdSbVZ5Y21WcGNtRWdVMkZ1ZEc5eklGTnBiSFpo";
  
  //roda todos na mesma direção para alinhar ampulhetas
  //delay(tempoAtivoServo);

  //para todos os servos
  for (byte i = 0; i < NumMotores; i++) {
    //servos[i].write(servoCentro[i]); // Para rotação
    ultRotacaoServo[i] = millis(); // Inicializa timers
    servoGirando[i] = false; // Marca como parado
  }
  Serial.println("Inicializado!!!");
}

void SetaMillis (long ms)
{
  //Inicializa o timer de millis com o valor estipulado no setup
  noInterrupts(); //para interrupções
  timer0_millis = ms;//seta o tempo de millis
  interrupts(); // retoma interrupções
}
