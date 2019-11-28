#define tam 8  //Tamanho do barramento do contador
#define intrv 1000000 //Intervalo da interrupção no sistema, em microssegundos
#define estrategiaControle 0
#define PINO_INTERRUPCAO 0

/*
 * Biblioteca que contém interrupção por tempo
 */
// #include <wiringPi>
#include "CalculaPosVel.h"

/*
 * Valor que vem do contador de 8 bits
 * Intervalo de 0 a 255
 */
int pinos[] = 
{
  46, 48, 50, 52, 47, 49, 51, 53
};
double PosicaoDesejada;

 CalculaPosVel contador = CalculaPosVel(pinos, 22);

void selecionaEstrategia(int estratControle, double &PWM, int &sentido, double erro)
{
  if(estratControle == 1)
  {
    // pid.calcula(erro, PWM, sentido); // Descomente apos incluir o PID
  }
  else if (estratControle == 2)
  {
    // pidAdaptativo.calcula(erro, PWM, sentido); // Descomente apos incluir o PID e o PID adaptativo
  }
  else if (estratControle == 3)
  {
    // fuzzy.calcula(erro, PWM, sentido); // Descomente apos incluir o fuzzy
  }
}

void envia(double PWM, int sentido)
{
  if(sentido == 1)
  {
    analogWrite(0, PWM);
    analogWrite(1, PWM);

  }
  else if (sentido == -1)
  {
    analogWrite(2, PWM);
    analogWrite(3, PWM);
  }
}

/*
 * Função da interrupção
 */
void subtracao(void)
{
  // Sistema checa a posição real
  double erro = PosicaoDesejada - contador.getPosicao();
  // A posição real é a desejada?
  if (erro != 0)
  {
    int sentido = 0;
    double PWM = 0;
    selecionaEstrategia(estrategiaControle, PWM, sentido, erro);
    envia(PWM, sentido);
  }
}

int main(void)
{ 
  // Inicialização da wiringPi
  wiringPiSetup();

  // Configuraão da interrupção
  pinMode(PINO_INTERRUPCAO, INPUT);
  wiringPiISR (PINO_INTERRUPCAO, INT_EDGE_FALLING, &subtracao);

  // Loop principal do programa
  while(true)
  {
    // Um monte de nada
  }
  return 0;
}
