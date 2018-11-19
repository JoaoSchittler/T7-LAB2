

#include <chrono>
#include <list>
#include <random>
#include <vector>
#include <iostream>
#include <functional>
#include "Tela.hpp"
#include "geom.hpp"
#include "mapa.hpp"
#define  FPS 60
using namespace tela;
using namespace geom;

/* estados para o jogo */
enum Estado { nada, menu, jogando, ganhou, perdeu };

// Estrutura para controlar todos os objetos e estados do Jogo Centipede
struct Jogo {
  Tela t;
  Estado estado;
  int tecla;
  void inicia(void) {
    t.inicia(800, 600, "Pacman");
    estado = Estado::nada;
  }

  // atualiza o jogo, como se fosse um passo na execução
  void atualiza(void) {
    tecla = t.tecla();
    t.espera(1000/FPS);
  }

  // verifica se o jogo terminou ou não
  // - retorna TRUE quando  termina
  // - retorna FALSE caso contrário
  bool verifica_fim(void) {
    if (estado == Estado::perdeu || estado == Estado::ganhou)
      return true;
    else
      return false;
  }

  void finaliza(void) {
    if(estado == Estado::perdeu)
     printf("\nGAME OVER\n");
    else
     printf("\nYOU WIN \n");
    t.finaliza();
  }
  
};

int main(int argc, char **argv) {
  Jogo jogo;

  jogo.inicia();
  while (jogo.verifica_fim() == false)
  {
    jogo.atualiza();
  }
  jogo.finaliza();
  return 0;
}
