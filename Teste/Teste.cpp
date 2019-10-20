# include <iostream>
# include <fstream>
# include <string>

    int contagem;       //Contagem mais atual do encoder, convertida do binário
    int contagemAnterior;
    int sentido;        //1 -> Horário, -1 -> Anti-horário
    double posicao;     //Posição em mm
    double posicaoAnterior;
    double velocidade;  //Velocidade em mm/s
    double tempoAtual;
    double tempoAnterior;

    void pegaDados(std::ifstream& entrada);
    void calculaPosicao(std::ifstream& entrada, std::ofstream& saida);
    void calculaVelocidade (std::ofstream& saida);


    

int main ()
{
    std::ifstream entrada;
    std::ofstream saida;

    int exemplos = 2;

    entrada.open("testes.txt");
    saida.open("resultados.txt");

    for (int i = 0; i < exemplos; i++)
    {
        calculaPosicao(entrada, saida);
        calculaVelocidade(saida);
    }

    saida.close();
    entrada.close();

    
    return 0;
}

void pegaDados(std::ifstream& entrada)
{
  std::string posicaoArq, contagemAnteriorArq, tempoAnteriorArq, contagemArq, tempoAtualArq, sentidoArq;

  if(entrada.is_open()){
      entrada >> posicaoArq >> contagemAnteriorArq >> tempoAnteriorArq >> contagemArq >> tempoAtualArq >> sentidoArq;
 }

  posicao = std::stod(posicaoArq);
  contagemAnterior = std::stoi(contagemAnteriorArq);
  tempoAnterior = std::stod(tempoAnteriorArq);
  contagem = std::stoi(contagemArq);  
  tempoAtual = std::stod(tempoAtualArq);
  sentido = std::stoi(sentidoArq);
}

/*
  Calcula a posição, tratando casos como o estouro de contagem do encoder e o sentido de giro
 */
void calculaPosicao(std::ifstream& entrada, std::ofstream& saida)
{
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
  saida << "Posicao : " << std::to_string(posicao) << " mm ";
  

}
/*
  Calcula a velocidade a partir das diferenças nas posições e no intervalo entre chamadas da função
 */
void calculaVelocidade (std::ofstream& saida)
{
  velocidade = (posicao - posicaoAnterior)*1000/(tempoAtual - tempoAnterior);
  saida << "Velocidade : " << std::to_string(velocidade) << " mm/s" << std::endl;
}