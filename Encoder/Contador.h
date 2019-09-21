#ifndef Contador_h_
#define Contador_h_

class Contador
{
    public:
        Contador(int *p, int interv, int sent);
        void atualiza();
        int getSentido();
        double getPosicao();
        double getVelocidade();

    private:
        int contagem;
        int contagemAnterior;
        int *pinos;         //Ponteiro para um elemento no vetor de pinos
        int intervalo;      //Valor do intervalo da interrupção, usado no calculo da velocidade
        int sentidoPin;     //Pino da ponte H que quando HIGH -> Horário e LOW -> Anti-horário
        int sentido;
        double posicao;
        double posicaoAnterior;
        double velocidade;

        void calculaContagem();
        void calculaPosicao();
        void calculaVelocidade();
};

#endif
