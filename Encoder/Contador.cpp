//#include "Arduino.h"
#include "Contador.h"

Contador::Contador(int *p)
{
    pinos = p;
    for (int i = 0; i < 8; i++) {
      pinMode(pinos[i], INPUT);
  }
}

void Contador::atualiza()
{
    calculaContagem();
    calculaPosicao();
    calculaVelocidade();
}

double Contador::getPosicao()
{
    return posicao;
}

double Contador::getVelocidade()
{
    return velocidade;
}

void Contador::calculaContagem()
{
  int recebe;
  int somaParcial = 0;

  for (int pos = 0; pos < 8; pos++)
  {
    // Variável que recebe o valor lido no pino
    recebe = digitalRead(pinos[pos]);

    /* Calcula o shift do valor, assim evita de fazer
     * potências e fica mais eficiente, por conta dos
     * valores lidos serem só 0 ou 1
     */
    recebe = recebe << pos;

    // Coloca em uma soma parcial;
    somaParcial += recebe;
  }

  contagem = somaParcial;
}

void Contador::calculaPosicao()
{
  posicaoAnterior = posicao;
  
  int deltaContagem = 0;                                //Diferença entre a última contagem e a contagem atual
  
  if (contagem < contagemAnterior)                           //O contador completou um ciclo de contagem
  {
    deltaContagem = ((256 - contagemAnterior) + contagem);   //Soma do que faltava para completar uma volta com o que passou depois da volta completa
  } else
  {
    deltaContagem = contagem - contagemAnterior;             //Diferença entre a última contagem e a contagem atual
  }

  contagemAnterior = contagem;

  /*
   *  Verifica o sentido do giro do encoder 
   */
  deltaContagem *= sentido;

 /*
 * Medindo no braço do robô, foi constatado que as 400 contagens do encoder equivalem a uma variação
 * vertical de (4,0  0,5)mm, ou seja, cada contagem equivale a 0,01mm de movimento vertical
 */
  posicao = posicaoAnterior + (double)deltaContagem*0.01;

 /*
  * Aqui, a posição que o braço do robô vai estar já foi definida
  */

}

void Contador::calculaVelocidade ()
{
  /*
   * A velocidade do motor é definida pela diferença de posição no tempo entre chamadas de interrupção
   * Uma velocidade negativa indica que o motor está descendo
   * Unidade: mm/s
   */
  velocidade = (posicao - posicaoAnterior)*1000000/intervalo;

}