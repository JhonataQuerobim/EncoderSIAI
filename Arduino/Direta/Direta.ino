/*
    Autores: Carlos Gomes, Gustavo de Souza, Jhonata Querobim e Vinicíus Peixoto
*/

#define tam 8  //Tamanho do barramento do contador
#define pino 22 // Pino da ponte h que, quando HIGH -> Horario, e LOW -> Anti-horário
#define intervalo 1000000 //Intervalo da interrupção no sistema, em microssegundos

#include <TimerOne.h> //Biblioteca que contém interrupção por tempo


/*
 * Valor que vem do contador de 8 bits
 * Intervalo de 0 a 255
 */
int oldContagem = 0;
int contagem = 0;

int pinos[] =
{
  46, 48, 50, 52, 47, 49, 51, 53
};

/*
 * Valores das variáveis do robô
 */
double oldPosicao = 0;
double newPosicao = 0;
double velocidade = 0;
 
 /*
 * Variáveis de sentido
 */
int sentido = 0;

 /*
  *  Variáveis de medição de tempo (frequência em torno de 7.7k )
  */
unsigned long tempoInicio = 0;
unsigned long tempoFinal = 0;
  
void setup() 
{

  for (int i = 46; i < 54; i++) {
      pinMode(pinos[i], INPUT);
  }
  
/*
 * Inicialização do timer
 * O intervalo de interrupção pode ser definido aqui
 * Valor mínimo de 1us, que gera uma frequência máxima de 1MHz
 */
  Timer1.initialize(intervalo);

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
 * Função de interrupção, calcula também o tempo de execução das rotinas de cálculo da posição, velocidade e contagem
 */
void leEncoder()
{
  tempoInicio = micros();
  getContagem();
  getPosicao();
  getVelocidade();
  tempoFinal = micros();
  Serial.println(tempoFinal-tempoInicio);
}

/*
 * Converte os valores que vêm do contador em um valor inteiro
 */
void getContagem()
{
  int recebe;
  int somaParcial = 0;

  for (int pos = 0; pos < tam; pos++)
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
 * Obtem a posição do robô a partir da contagem do contador
 */
void getPosicao ()
{
  sentido = (digitalRead(pino) == HIGH) ? 1 : -1; 

  oldPosicao = newPosicao;
  
  int deltaContagem = 0;                                //Diferença entre a última contagem e a contagem atual
  
  if (contagem < oldContagem)                           //O contador completou um ciclo de contagem
  {
    deltaContagem = ((256 - oldContagem) + contagem);   //Soma do que faltava para completar uma volta com o que passou depois da volta completa
  } else
  {
    deltaContagem = contagem - oldContagem;             //Diferença entre a última contagem e a contagem atual
  }

  oldContagem = contagem;

  /*
   *  Verifica o sentido do giro do encoder 
   */
  deltaContagem *= sentido;
/*
 * Necessário as especificações do encoder e do roscamento em que o robô irá se deslocar
 * Isso irá definir sua posição vertical através do tempo
 */
/*
  * Medindo no braço do robô, foi constatado que as 400 contagens do encoder equivalem a uma variação
  * vertical de (4,0  0,5)mm, ou seja, cada contagem equivale a 0,01mm de movimento vertical
  */
  newPosicao = oldPosicao + (double)deltaContagem*0.01;

}

/*
 * Obtem a velocidade a partir da nova posição do robô
 */
void getVelocidade ()
{
  /*
   * A velocidade do motor é definida pela diferença de posição no tempo entre chamadas de interrupção
   * Uma velocidade negativa indica que o motor está descendo
   * Unidade: mm/s
   */
  velocidade = (newPosicao - oldPosicao)*1000000/intervalo;

}

void loop()
{
  Serial.println(newPosicao);
  Serial.println(velocidade);
  Serial.println(sentido);
}
