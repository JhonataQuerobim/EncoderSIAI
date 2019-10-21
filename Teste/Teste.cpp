# include <iostream>
# include <fstream>
# include <string>
# include <cmath>

  int pinos[8] = {0, 1, 0, 0, 1, 0, 0, 0};
  int contagem;       //Contagem mais atual do encoder, convertida do binário
  int contagemAnterior;
  int sentido;        //1 -> Horário, -1 -> Anti-horário
  double posicao;     //Posição em mm
  double posicaoAnterior;
  double velocidade;  //Velocidade em mm/s
  double tempoAtual;
  double tempoAnterior;
  double posCorreta;
  double velCorreta;

  void pegaDados(std::ifstream& entrada);
  void calculaPosicao(std::ifstream& entrada, std::ofstream& saida);
  void calculaVelocidade (std::ofstream& saida);
  int calculaContagem();



    

int main ()
{
  std::ifstream entrada;
  std::ofstream saida;

  int exemplos = 5;

  entrada.open("testes.txt");
  saida.open("resultados.txt");

  for (int i = 0; i < exemplos; i++)
  {
      calculaPosicao(entrada, saida);
      calculaVelocidade(saida);
  }

  saida.close();
  entrada.close();
  
  std::cout << calculaContagem() << std::endl;
  
  return 0;
}

void pegaDados(std::ifstream& entrada)
{
  std::string posicaoArq, contagemAnteriorArq, tempoAnteriorArq, contagemArq, tempoAtualArq, sentidoArq, posCorretaArq, velCorretaArq;

  if(entrada.is_open()){
      entrada >> posicaoArq >> contagemAnteriorArq >> tempoAnteriorArq >> contagemArq >> tempoAtualArq >> sentidoArq >> posCorretaArq >> velCorretaArq;
 }

  posicao = std::stod(posicaoArq);
  contagemAnterior = std::stoi(contagemAnteriorArq);
  tempoAnterior = std::stod(tempoAnteriorArq);
  contagem = std::stoi(contagemArq);  
  tempoAtual = std::stod(tempoAtualArq);
  sentido = std::stoi(sentidoArq);
  posCorreta = std::stod(posCorretaArq);
  velCorreta = std::stod(velCorretaArq);
}

/*
  Calcula a posição, tratando casos como o estouro de contagem do encoder e o sentido de giro
 */
void calculaPosicao(std::ifstream& entrada, std::ofstream& saida)
{
  std::string avaliacao;
  pegaDados(entrada);

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
  avaliacao = (abs(posicao - posCorreta) < 0.001) ? "|Correto| " : "|Errado| ";
  saida << "Posicao : " << std::to_string(posicao) << " mm " << avaliacao;
  

}
/*
  Calcula a velocidade a partir das diferenças nas posições e no intervalo entre chamadas da função
 */
void calculaVelocidade (std::ofstream& saida)
{
  std::string avaliacao;
  velocidade = (posicao - posicaoAnterior)*1000/(tempoAtual - tempoAnterior);
  avaliacao = (abs(velocidade - velCorreta) < 0.001) ? "|Correto|" : "|Errado|";

  saida << "Velocidade : " << std::to_string(velocidade) << " mm/s " << avaliacao << std::endl;
}

int calculaContagem()
{
  int recebe;
  int somaParcial = 0;

  for (int pos = 0; pos < 8; pos++)
  {
    // Variável "recebe" recebe o valor lido no pino
    recebe = pinos[pos];

    /* Calcula o shift do valor, assim evita de fazer
     * potências e fica mais eficiente, por conta dos
     * valores lidos serem só 0 ou 1
     */
    recebe = recebe << pos;

    // Coloca em uma soma parcial;
    somaParcial += recebe;
  }
  return somaParcial;
}