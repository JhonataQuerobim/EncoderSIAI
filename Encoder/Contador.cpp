#include "Arduino.h"
#include "Contador.h"
/*
  Construtor da classe Contador
    *p Ponteiro para a posição inicial de um vetor contendo os pinos do encoder
    interv Intervalo entre chama consultas ao encoder (ms)
    sent Pino da ponte H que indicará o sentido de giro do motor
 */
Contador::Contador(int *p, int interv, int sent)
{
    pinos = p;
    for (int i = 0; i < 8; i++) {
      pinMode(pinos[i], INPUT);
  }

  contagem = 0;
  contagemAnterior = 0;
  intervalo = interv;
  posicao = 0;
  posicaoAnterior = 0;
  velocidade = 0;
  sentidoPin = sent;
}
/*
  Construtor de cópia
 */
Contador::Contador(const Contador &c)
{
  pinos = c.pinos;
  contagem = c.contagem;
  intervalo = c.intervalo;
  posicao = c.posicao;
  posicaoAnterior = c.posicaoAnterior;
  velocidade = c.velocidade;
  sentidoPin = c.sentidoPin;
}
/*
  Destrutor
 */
Contador::~Contador()
{

}
/*
  Atualiza os valores de contagem, posição e velocidade
 */
void Contador::atualiza()
{ 
  sentido = (digitalRead(sentidoPin) == HIGH) ? 1 : -1;
  calculaContagem();
  calculaPosicao();
  calculaVelocidade();
}
/*
  Retrona o sentido
 */
int Contador::getSentido()
{
    return sentido;
}
/*
  Retorna a posição
 */
double Contador::getPosicao()
{
    return posicao;
}
/*
  Retorna a velocidade
 */
double Contador::getVelocidade()
{
    return velocidade;
}
/*
  Altera o intervalo
 */
void Contador::setIntervalo(int interv)
{
  intervalo = interv;
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
/*
  Calcula a posição, tratando casos como o estouro de contagem do encoder e o sentido de giro
 */
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

  deltaContagem *= sentido;     //Verifica o sentido do giro do encoder 

 /*
 * Medindo no braço do robô, foi constatado que as 400 contagens do encoder equivalem a uma variação
 * vertical de (4,0  0,5)mm, ou seja, cada contagem equivale a 0,01mm de movimento vertical
 */

  posicao = posicaoAnterior + (double)deltaContagem*0.01;

}
/*
  Calcula a velocidade a partir das diferenças nas posições e no intervalo entre chamadas da função
 */
void Contador::calculaVelocidade ()
{
  velocidade = (posicao - posicaoAnterior)*1000000/intervalo;
}
