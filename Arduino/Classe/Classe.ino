#define tam 8  //Tamanho do barramento do contador
#define intrv 1000000 //Intervalo da interrupção no sistema, em microssegundos

/*
 * Biblioteca que contém interrupção por tempo
 */
#include <TimerOne.h>
#include "Contador.h"

/*
 * Valor que vem do contador de 8 bits
 * Intervalo de 0 a 255
 */
int pinos[] = 
{
  46, 48, 50, 52, 47, 49, 51, 53
};
double PosicaoDesejada;

 Contador contador = Contador(pinos, 22);

void setup() 
{
/*
 * Inicialização do timer
 * O intervalo de interrupção pode ser definido aqui
 * Valor mínimo de 1us, que gera uma frequência máxima de 1MHz
 */
  Timer1.initialize(intrv);

  /* Vinculação da função de interrupção com o temporizador
   * A função de interrupção é chamada sempre que o temporizador entra em overflow
   * Cuidado na configuração do período do temporizador, pois ele pode travar seu
   *  programa para nunca voltar para a função principal se o período for curto e
   *  a função de interrupção for complexa
   */
  Timer1.attachInterrupt(subtracao);

  Serial.begin(9600);
}

void selecionaEstrategia(int estrategiaControle, double &PWM, int &sentido)
{
  if(estrategiaControle == 1)
  {
    pid.calcula(PWM, sentido);
  }
  else if (estrategiaControle == 2)
  {
    pidAdaptativo.calcula(PWM, sentido);
  }
  else if (estrategiaControle == 3)
  {
    fuzzy.calcula(PWM, sentido);
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
void subtracao()
{
  // Sistema checa a posição real
  double diferenca = PosicaoDesejada - contador.getPosicao();
  // A posição real é a desejada?
  if (diferenca != 0)
  {
    int sentido;
    double PWM;
    selecionaEstrategia(estrategiaControle, &PWM, &sentido);
    envia(PWM, sentido);

    //controle(diferenca);
  }
}

void loop()
{ 
  // PosicaoDesejada = getPosicaoDesejada();
  // Um monte de nada
  
}
