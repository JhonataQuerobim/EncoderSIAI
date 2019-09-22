/*
    Contador.h - Classe que interpreta o contador que conta os pulsos do encoder do robô da disciplina.
    Autores: Carlos Gomes, Gustavo de Souza, Jhonata Querobim e Vinicíus Peixoto
    Domínio público
*/

#ifndef Contador_h_
#define Contador_h_

class Contador
{
    public:
        Contador(int *p, int interv, int sent);     //Construtor
        Contador(const Contador &copia);            //Construtor de cópia
        ~Contador();                                //Destrutor
        void atualiza();                            //Atualiza os atributos do objeto
        int getSentido();
        double getPosicao();
        double getVelocidade();
        void setIntervalo(int interv);

    private:
        int contagem;       //Contagem mais atual do encoder, convertida do binário
        int contagemAnterior;
        int *pinos;         //Ponteiro para um elemento no vetor de pinos
        int intervalo;      //Valor do intervalo da interrupção, usado no calculo da velocidade
        int sentidoPin;     //Pino da ponte H que quando HIGH -> Horário e LOW -> Anti-horário
        int sentido;        //1 -> Horário, -1 -> Anti-horário
        double posicao;     //Posição em mm
        double posicaoAnterior;
        double velocidade;  //Velocidade em mm/s

        void calculaContagem();
        void calculaPosicao();
        void calculaVelocidade();
};

#endif
