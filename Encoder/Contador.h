#ifndef Contador_h_
#define Contador_h_

//#include "Arduino.h"

class Contador
{
    public:
        Contador(int *p);
        void atualiza();
        double getPosicao();
        double getVelocidade();

    private:
        int contagem;
        int contagemAnterior;
        int *pinos;
        int intervalo;
        double posicao;
        double posicaoAnterior;
        double velocidade;
        double sentido;

        void calculaContagem();
        void calculaPosicao();
        void calculaVelocidade();
};

#endif