
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
#define  FPS 30       //Frams por segundos
#define  PPF  5       //Pixeis por frames
using namespace tela;
using namespace geom;
using namespace mapa;
using namespace grafo;
/* estados para o jogo */
enum Estado { nada, menu, jogando, ganhou, perdeu };

// Estrutura para controlar todos os objetos e estados do Jogo Centipede
struct Jogo
{
    struct Character
    {
        Ponto    pos;
        Ponto map_pos; 
        int      next_dir;
        int      current_dir;
        int      anim_counter;
        int      anim_sprite_frame;
    };
    Tela            t;
    Estado          estado;
    Character       pacman;
    Character       r_ghost;
    Mapa            mapa;
    ALLEGRO_BITMAP* imagens[16];
    Grafo           grafo;
    int             tecla;
    int             frame_counter;
    int             score;
    

    void inicia(void)
    {
      t.inicia                (800, 600, "Pacman");
      mapa.cria               ("mapa.txt");
      grafo.cria_grafo        (mapa.map,mapa.lin,mapa.col);
      grafo.cria_lista_adj    ();
      carrega_imagens         ();
      estado                   = Estado::nada;
      score                    = 0;
      pacman.current_dir       = 4;//Inicia movendo para a direita
      pacman.next_dir          = 4;
      pacman.anim_counter      = 0;
      pacman.anim_sprite_frame = 0;
      pacman.pos               = grafo.grafo[0].ponto;
      pacman.map_pos           = grafo.grafo[0].ponto;
    }
    void carrega_imagens()
    {
         //Imagens do pacman
         imagens[0] = t.carrega_imagem("Sprites/PAC_U1.png"); 
         imagens[1] = t.carrega_imagem("Sprites/PAC_U2.png");

         imagens[2] = t.carrega_imagem("Sprites/PAC_D1.png");
         imagens[3] = t.carrega_imagem("Sprites/PAC_D2.png");


         imagens[4] = t.carrega_imagem("Sprites/PAC_L1.png");
         imagens[5] = t.carrega_imagem("Sprites/PAC_L2.png");

         imagens[6] = t.carrega_imagem("Sprites/PAC_R1.png");
         imagens[7] = t.carrega_imagem("Sprites/PAC_R2.png");


         //Imagens do fantasma vermelho 

         std::cout << "Imagens carregadas \n"; 
    }
    //Função que roda a cada frame do jogo
    void atualiza(void)
    {
      detecta_tecla();
      desenha_jogo();
      t.mostra();
      move_personagens();
      t.espera(1000/FPS);
    }
    void detecta_tecla()
    {
        tecla = t.tecla();
       // std::cout << tecla << std::endl;
        if(tecla == ALLEGRO_KEY_Q) estado = Estado::perdeu;
        if(tecla == ALLEGRO_KEY_UP || tecla == ALLEGRO_KEY_W)    pacman.next_dir = 1;
        if(tecla == ALLEGRO_KEY_DOWN || tecla == ALLEGRO_KEY_S)  pacman.next_dir = 2;
        if(tecla == ALLEGRO_KEY_LEFT || tecla == ALLEGRO_KEY_A)  pacman.next_dir = 3;
        if(tecla == ALLEGRO_KEY_RIGHT || tecla == ALLEGRO_KEY_D) pacman.next_dir = 4;
    }
    void desenha_jogo()
    {
        mapa.desenha(t);
        desenha_pacman();
        //desenha_fantasmas();
    }
    void desenha_pacman()
    {
        //Indexação com o vetor imagens: 2*(dir-1) se anim_sprite_frame = 0, 2*(dir-1)+1se anim_sprite_frame = 1
        int index = 2*(pacman.current_dir-1);
        if(pacman.anim_sprite_frame > 1) index++;
        //std::cout << "Index : " << index << std::endl;
        t.desenha_imagem(imagens[index],pacman.pos.x-6,pacman.pos.y-5);
    }
    void move_personagens()
    {
        move_pac();
        //move_ghost();
        //detecte_gameover();
    } 
    void move_pac()
    {
        if(pacman.anim_counter < 20)
        {
           //Continua a animação de movimento  
            pac_move_anim();
        }
        else
        {
           //Começa a próxima animação de movimento
            pacman.map_pos.x = pacman.pos.x;
            pacman.map_pos.y = pacman.pos.y;
            pacman.anim_counter = 0;
            pacman.current_dir = pacman.next_dir;
        }  
    }
    //Animações
    void pac_move_anim()
    {
        //Realiza o movimento
        std::vector <Vertice*> saidas = grafo.busca_vizinhos(pacman.map_pos);
        int i = 0;
        int tam = saidas.size();
        while(i < tam)
        {
            if((pacman.current_dir==1) && (saidas[i]->ponto.y == pacman.map_pos.y-20))
            {
                pacman.pos.y -=PPF;
            }
            if((pacman.current_dir==2) && (saidas[i]->ponto.y == pacman.map_pos.y+20))
            {
                pacman.pos.y +=PPF;
            }
            if((pacman.current_dir==3) && (saidas[i]->ponto.x == pacman.map_pos.x-20))
            {
                pacman.pos.x -=PPF;
            }
            if((pacman.current_dir==4) && (saidas[i]->ponto.x == pacman.map_pos.x+20))
            {
                pacman.pos.x +=PPF;
            }  

            i++;  
        } 
      //Ajusta o frame do sprite a ser desenhado, ajusta o contador de animação
        if(pacman.anim_sprite_frame >= 0)
              pacman.anim_sprite_frame++;
        if(pacman.anim_sprite_frame == 5)
              pacman.anim_sprite_frame = 0;
        pacman.anim_counter+=PPF;
       //std::cout << "X : " << pacman.pos.x  << "  Y : " << pacman.pos.y << std::endl;
       //std::cout << "Current dir : " << pacman.current_dir << "  Next dir :" << pacman.next_dir << std::endl;

    }
    bool verifica_fim(void)
    {
      if (estado == Estado::perdeu || estado == Estado::ganhou)
        return true;
      else
        return false;
    }

    void finaliza(void)
    {
      mapa.libera();
      if(estado == Estado::perdeu)
        printf("\nGAME OVER\n");
      else
        printf("\nYOU WIN \n");
      int i;
      for(i=0;i<8;i++)
      {
          t.destroy_imagem(imagens[i]);
      } 
      t.finaliza();
    }
  
};

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



