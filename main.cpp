
#include"jogo.hpp"

using namespace jogo;

int main(int argc, char **argv)
{
    Jogo jogo;

    jogo.inicia();
    while (jogo.verifica_fim() == false)
    {
        jogo.atualiza();
    }
    jogo.finaliza();
    return 0;
}
