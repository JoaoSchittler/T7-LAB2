#pragma once

#define  TAM  20      //Tamanho de um quadrado do mapa em pixeis
#include <chrono>
#include <list>
#include <random>
#include <vector>
#include <iostream>
#include <functional>

#include "Tela.hpp"
#include "geom.hpp"
#include "mapa.hpp"
#include "grafo.hpp"

#define  FPS 30      //Frames por segundos
#define  PPF 4       //Pixeis por frames

using namespace tela;
using namespace geom;
using namespace mapa;
using namespace grafo;

/* estados para o jogo */
enum Estado { nada, menu, jogando, ganhou, perdeu };

namespace jogo{
    struct Jogo
    {

        struct Character
        {

            Ponto    pos;
            Ponto map_pos;
            int      eaten;
            int      next_dir;
            int      current_dir;
            int      anim_counter;
            int      anim_sprite_frame;
            ALLEGRO_BITMAP* sprite[12];
            
        };

        Tela            t;
        Estado          estado;
        Character       pacman;
        const  int      quant_fantasma = 4;
        Character       ghost[4];
        Mapa            mapa;
        Grafo           grafo;
        ALLEGRO_SAMPLE* som[5];
        ALLEGRO_SAMPLE_INSTANCE* instance;
        int             tecla;
        int             frame_counter;
        int             score;
        int             powerup_timer;
        int             game_timer;


        void inicia(void);

        void inicia_variaveis();

        void inicia_pacman();

        void inicia_fantasmas();

        void carrega_imagens();
        
        void carrega_sons();

        //Função que roda a cada frame do jogo

        void atualiza(void);

        void detecta_tecla();

        void desenha_jogo();

        void desenha_pacman();

        void desenha_info();

        void desenha_fantasmas();

        void desenha_fantasma(int i);

        void move_personagens();

        bool existe_pontos();

        void move_pac()
        ;
        //Animações

        void pac_move_anim();

        void move_ghosts();

        void move_ghost(Ponto proximo ,int i);

        void move_ghost2(Ponto proximo ,int i);

        void ghost_move_anim(Ponto proximo ,int i);

        int ajusta_direcao(Ponto destino,Ponto gp);

        ///Retorna um ponto vizinho aleatorio a partir do fantasma i
        Ponto retorna_caminho_aleatorio(int i);

       //Retorna o caminho do fantasma i ate o ponto q
        Ponto retorna_caminho(int i, Ponto q);

        bool verifica_fim(void);

        void destroi_imagens();
        
        void destroi_sons();

        void finaliza(void);


    };
}
