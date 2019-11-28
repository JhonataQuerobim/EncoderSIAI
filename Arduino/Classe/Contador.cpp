#include "Arduino.h"
#include "Contador.h"
/*
  Construtor da classe Contador
    *p Ponteiro para a posição inicial de um vetor contendo os pinos do encoder
    sent Pino da ponte H que indicará o sentido de giro do motor
 */
Contador::Contador(int *p, int sent)
{
    pinos = p;
    for (int i = 0; i < 8; i++) {
      pinMode(pinos[i], INPUT);
  }

  contagem = 0;
  contagemAnterior = 0;
  posicao = 0;
  posicaoAnterior = 0;
  velocidade = 0;
  sentidoPin = sent;
  tempoAtual = millis();
  tempoAnterior = 0;
}
/*
  Construtor de cópia
 */
Contador::Contador(const Contador &c)
{
  pinos = c.pinos;
  contagem = c.contagem;
  posicao = c.posicao;
  posicaoAnterior = c.posicaoAnterior;
  velocidade = c.velocidade;
  sentidoPin = c.sentidoPin;
  tempoAtual = c.tempoAtual;
  tempoAnterior = c.tempoAnterior;
}
/*
  Destrutor
 */
Contador::~Contador()
{

}
/*
  Retrona o sentido
 */
int Contador::getSentido()
{
  sentido = (digitalRead(sentidoPin) == HIGH) ? 1 : -1;
  return sentido;
}
/*
  Retorna a posição
 */
double Contador::getPosicao()
{
  calculaPosicao();
  return posicao;
}
/*
  Retorna a velocidade
 */
double Contador::getVelocidade()
{
  calculaPosicao();
  calculaVelocidade();
  return velocidade;
}
/*
  Calcula a contagem lendo os pinos do contador e convertendo eles de binário para inteiro
 */
void Contador::calculaContagem()
{
  int recebe;
  int somaParcial = 0;

  for (int pos = 0; pos < 8; pos++)
  {
    // Variável "recebe" recebe o valor lido no pino
    recebe = digitalRead(pinos[pos]);

    /* Calcula o shift do valor, assim evita de fazer
     * potências e fica mais eficiente, por conta dos
     * valores lidos serem só 0 ou 1
     */
    recebe = recebe << pos;

    // Coloca em uma soma parcial;
    somaParcial += recebe;
  }
  tempoAnterior = tempoAtual;
  tempoAtual = millis();

  // Atualiza a contagem com a soma parcial
  contagem = somaParcial;
  sentido = (digitalRead(sentidoPin) == HIGH) ? 1 : -1;
}
/*
  Calcula a posição, tratando casos como o estouro de contagem do encoder e o sentido de giro
 */
void Contador::calculaPosicao()
{
  calculaContagem();

  posicaoAnterior = posicao;
  
  int deltaContagem = 0;                                //Calcual a diferença entre a última contagem e a contagem atual
  
  if (contagem < contagemAnterior)                           //Ajuste, caso o contador complete um ciclo de contagem
  {
    deltaContagem = ((256 - contagemAnterior) + contagem);   //Soma do que faltava para completar uma volta com o que passou depois da volta completa
  } else
  {
    deltaContagem = contagem - contagemAnterior;             //Diferença entre a última contagem e a contagem atual
  }

  contagemAnterior = contagem;

  deltaContagem *= sentido;     //Verifica o sentido do giro do encoder 

 /*
 * Soma ou subtrai a diferença, e converte para mm.
 * Medindo no braço do robô, foi constatado que as 400 contagens do encoder equivalem a uma variação
 * vertical de (4,0  0,5)mm, ou seja, cada contagem equivale a 0,01mm de movimento vertical
 */

  posicao = posicaoAnterior + (double)deltaContagem*0.01;

}
/*
  Calcula a velocidade em mm/s, a partir das diferenças nas posições e no intervalo de tempo
 */
void Contador::calculaVelocidade ()
{
  velocidade = (posicao - posicaoAnterior)*1000/(tempoAtual - tempoAnterior);
}
