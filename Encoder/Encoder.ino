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
int sentido;
double posicao;
double velocidade;

 /*
  *  Variáveis de medição de tempo (frequência em torno de 7.7k )
  */
  unsigned long tempoInicio = 0;
  unsigned long tempoFinal = 0;
  Contador contador = Contador(pinos, intrv, 22);

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
  Timer1.attachInterrupt(leEncoder);

  Serial.begin(9600);
}

/*
 * Função de interrupção
 */
void leEncoder()
{
  tempoInicio = micros();
  contador.atualiza();
  tempoFinal = micros();
  Serial.println(tempoFinal-tempoInicio);
}

void loop()
{
  sentido = contador.getSentido();
  posicao = contador.getPosicao();
  velocidade = contador.getVelocidade();

  Serial.println(sentido);
  Serial.println(posicao);
  Serial.println(velocidade);
  
}
