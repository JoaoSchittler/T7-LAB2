
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
#define  PPF 5       //Pixeis por frames
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
        ALLEGRO_BITMAP* sprite[12];
    };
    Tela            t;
    Estado          estado;
    Character       pacman;
    Character       ghost[4];
    Mapa            mapa;
    Grafo           grafo;
    int             tecla;
    int             frame_counter;
    int             score;
    int             powerup_timer;
    int             game_timer;
    const  int      quant_fantasma = 4;

    void inicia(void)
    {
      t.inicia                (800, 600, "Pacman");
      mapa.cria               ("mapa.txt");
      grafo.cria_grafo        (mapa.map,mapa.lin,mapa.col);
      grafo.cria_lista_adj    ();
      carrega_imagens         ();
      inicia_variaveis        ();
    }
    void inicia_variaveis()
    {
        estado                   = Estado::nada;
        score                    = 0;
        game_timer               = 0; 
        powerup_timer            = 0; 
        inicia_pacman();
        inicia_fantasmas();
    }
    void inicia_pacman()
    {
        pacman.current_dir       = 4;//Inicia movendo para a direita
        pacman.next_dir          = 4;
        pacman.anim_counter      = 0;
        pacman.anim_sprite_frame = 0;
        pacman.pos               = grafo.grafo[0].ponto;
        pacman.map_pos           = grafo.grafo[0].ponto;
    }
    void inicia_fantasmas()
    {
        for(int  i =0 ; i<quant_fantasma;i++)
        {
            ghost[i].pos               = grafo.grafo[i+2].ponto;
            ghost[i].map_pos           = grafo.grafo[i+2].ponto;
            ghost[i].current_dir       = 4;//Inicia fantasma olhando para a direita
            ghost[i].anim_counter      = 0;
            ghost[i].anim_sprite_frame = 0;
        }    
    }
    void carrega_imagens()
    {
         //Imagens do pacman
         pacman.sprite[0]   = t.carrega_imagem("Sprites/PAC_U1.png");
         pacman.sprite[1]   = t.carrega_imagem("Sprites/PAC_U2.png");
         pacman.sprite[2]   = t.carrega_imagem("Sprites/PAC_D1.png");
         pacman.sprite[3]   = t.carrega_imagem("Sprites/PAC_D2.png");
         pacman.sprite[4]   = t.carrega_imagem("Sprites/PAC_L1.png");
         pacman.sprite[5]   = t.carrega_imagem("Sprites/PAC_L2.png");
         pacman.sprite[6]   = t.carrega_imagem("Sprites/PAC_R1.png");
         pacman.sprite[7]   = t.carrega_imagem("Sprites/PAC_R2.png");
         pacman.sprite[8]   = t.carrega_imagem("Sprites/PAC_3.png");
         //Imagens do fantasma vermelho 
         ghost[0].sprite[0] = t.carrega_imagem("Sprites/RG_U1.png");
         ghost[0].sprite[1] = t.carrega_imagem("Sprites/RG_U2.png");
         ghost[0].sprite[2] = t.carrega_imagem("Sprites/RG_D1.png");
         ghost[0].sprite[3] = t.carrega_imagem("Sprites/RG_D2.png");
         ghost[0].sprite[4] = t.carrega_imagem("Sprites/RG_L1.png");
         ghost[0].sprite[5] = t.carrega_imagem("Sprites/RG_L2.png");
         ghost[0].sprite[6] = t.carrega_imagem("Sprites/RG_R1.png");
         ghost[0].sprite[7] = t.carrega_imagem("Sprites/RG_R2.png");
         ghost[0].sprite[8]  = t.carrega_imagem("Sprites/SG_1.png");
         ghost[0].sprite[9]  = t.carrega_imagem("Sprites/SG_2.png");
         ghost[0].sprite[10] = t.carrega_imagem("Sprites/SG_3.png");
         ghost[0].sprite[11] = t.carrega_imagem("Sprites/SG_4.png");
         //Imagens do fantasma Azul
         ghost[1].sprite[0] = t.carrega_imagem("Sprites/BG_U1.png");
         ghost[1].sprite[1] = t.carrega_imagem("Sprites/BG_U2.png");
         ghost[1].sprite[2] = t.carrega_imagem("Sprites/BG_D1.png");
         ghost[1].sprite[3] = t.carrega_imagem("Sprites/BG_D2.png");
         ghost[1].sprite[4] = t.carrega_imagem("Sprites/BG_L1.png");
         ghost[1].sprite[5] = t.carrega_imagem("Sprites/BG_L2.png");
         ghost[1].sprite[6] = t.carrega_imagem("Sprites/BG_R1.png");
         ghost[1].sprite[7] = t.carrega_imagem("Sprites/BG_R2.png");
         ghost[1].sprite[8]  = t.carrega_imagem("Sprites/SG_1.png");
         ghost[1].sprite[9]  = t.carrega_imagem("Sprites/SG_2.png");
         ghost[1].sprite[10] = t.carrega_imagem("Sprites/SG_3.png");
         ghost[1].sprite[11] = t.carrega_imagem("Sprites/SG_4.png");
         //Imagens do fantasma rosa
         ghost[2].sprite[0] = t.carrega_imagem("Sprites/PG_U1.png");
         ghost[2].sprite[1] = t.carrega_imagem("Sprites/PG_U2.png");
         ghost[2].sprite[2] = t.carrega_imagem("Sprites/PG_D1.png");
         ghost[2].sprite[3] = t.carrega_imagem("Sprites/PG_D2.png");
         ghost[2].sprite[4] = t.carrega_imagem("Sprites/PG_L1.png");
         ghost[2].sprite[5] = t.carrega_imagem("Sprites/PG_L2.png");
         ghost[2].sprite[6] = t.carrega_imagem("Sprites/PG_R1.png");
         ghost[2].sprite[7] = t.carrega_imagem("Sprites/PG_R2.png");
         ghost[2].sprite[8]  = t.carrega_imagem("Sprites/SG_1.png");
         ghost[2].sprite[9]  = t.carrega_imagem("Sprites/SG_2.png");
         ghost[2].sprite[10] = t.carrega_imagem("Sprites/SG_3.png");
         ghost[2].sprite[11] = t.carrega_imagem("Sprites/SG_4.png");
         //Imagens do fantasma amarelo
         ghost[3].sprite[0] = t.carrega_imagem("Sprites/YG_U1.png");
         ghost[3].sprite[1] = t.carrega_imagem("Sprites/YG_U2.png");
         ghost[3].sprite[2] = t.carrega_imagem("Sprites/YG_D1.png");
         ghost[3].sprite[3] = t.carrega_imagem("Sprites/YG_D2.png");
         ghost[3].sprite[4] = t.carrega_imagem("Sprites/YG_L1.png");
         ghost[3].sprite[5] = t.carrega_imagem("Sprites/YG_L2.png");
         ghost[3].sprite[6] = t.carrega_imagem("Sprites/YG_R1.png");
         ghost[3].sprite[7] = t.carrega_imagem("Sprites/YG_R2.png");
         ghost[3].sprite[8]  = t.carrega_imagem("Sprites/SG_1.png");
         ghost[3].sprite[9]  = t.carrega_imagem("Sprites/SG_2.png");
         ghost[3].sprite[10] = t.carrega_imagem("Sprites/SG_3.png");
         ghost[3].sprite[11] = t.carrega_imagem("Sprites/SG_4.png");
         std::cout << "Imagens carregadas \n"; 
    }
    //Função que roda a cada frame do jogo
    void atualiza(void)
    {
      detecta_tecla();
      desenha_jogo();
      t.mostra();
      move_personagens();
      if(powerup_timer>0) powerup_timer--;
      game_timer++;
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
        desenha_fantasmas();
        desenha_info();
    }
    void desenha_pacman()
    {
          //Indexação com o vetor imagens: 2*(dir-1) se anim_sprite_frame = 0, 2*(dir-1)+1se anim_sprite_frame = 1
         int index = 2*(pacman.current_dir-1);
         if(pacman.anim_sprite_frame > 1) index++;
         if(pacman.anim_sprite_frame == 4) index = 8;

         //std::cout << "Index : " << index << std::endl;
         t.desenha_imagem(pacman.sprite[index],pacman.pos.x-6,pacman.pos.y-5);
    }
    void desenha_info()
    {
          char* msg_timer = "TIME : ";
          char* msg_score = "SCORE: ";
          Ponto p1 = {100,10};
          Ponto p2 = {100,30};
          t.texto(p1,msg_score);
          t.texto(p2,msg_timer);

          std::string s = std::to_string(score);
          char const *Score = s.c_str();
          s = std::to_string(game_timer/FPS);
          char const *Time  = s.c_str();
          p1.x += 80;
          p2.x += 80;
          t.texto(p1,Score);
          t.texto(p2,Time);


    }

    void desenha_fantasmas()
    {
        for(int i = 0 ;i < quant_fantasma;i++)
        {
            desenha_fantasma(i);
        }
    }
    void desenha_fantasma(int i)
    {
        if(powerup_timer==0)
        {
              //UP = 1,DOWN = 2, LEFT =3 RIGHT = 4
              int index = 2*(ghost[i].current_dir-1);
              if(ghost[i].anim_sprite_frame > 1) index++;
              t.desenha_imagem(ghost[i].sprite[index],ghost[i].pos.x,ghost[i].pos.y);
    
        }
        else
        {
            //Powerup está ativo
        }  
    } 
    void move_personagens()
    {
        move_pac();
        move_ghosts();
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
            //Pacman chegou no proximo vertice do grafo
            unsigned int colindex =  (pacman.pos.x-mapa.xini)/20;
            unsigned int linindex =  (pacman.pos.y-mapa.yini)/20;
            if(mapa.map[linindex][colindex]=='C')
            {
                score++;
                mapa.map[linindex][colindex]= 'V';
                //std::cout << "Score: " << score << std::endl;
            } 
            if(mapa.map[linindex][colindex]=='E')
            {
                score++;
                mapa.map[linindex][colindex]= 'V';
                powerup_timer = 20*FPS;//20 segundos de powerup  
            } 
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
        bool moveu = false;
        int tam = saidas.size();
        while(i < tam)
        {
            if((pacman.current_dir==1) && (saidas[i]->ponto.y == pacman.map_pos.y-20))
            {
                pacman.pos.y -=PPF;
                moveu = true;
            }
            if((pacman.current_dir==2) && (saidas[i]->ponto.y == pacman.map_pos.y+20))
            {
                pacman.pos.y +=PPF;
                moveu = true;
            }
            if((pacman.current_dir==3) && (saidas[i]->ponto.x == pacman.map_pos.x-20))
            {
                pacman.pos.x -=PPF;
                moveu = true;
            }
            if((pacman.current_dir==4) && (saidas[i]->ponto.x == pacman.map_pos.x+20))
            {
                pacman.pos.x +=PPF;
                moveu = true;
            }  

            i++;  
        } 
      //Ajusta o frame do sprite a ser desenhado, ajusta o contador de animação
        if(moveu==true)
        {
          pacman.anim_sprite_frame++;
          if(pacman.anim_sprite_frame == 5)
              pacman.anim_sprite_frame = 0;
        } 
        else
          pacman.anim_sprite_frame = 3; 
        pacman.anim_counter+=PPF; 
       //std::cout << "X : " << pacman.pos.x  << "  Y : " << pacman.pos.y << std::endl;
       //std::cout << "Current dir : " << pacman.current_dir << "  Next dir :" << pacman.next_dir << std::endl;

    }

    void move_ghosts()
    {
        for(int i = 0;i < quant_fantasma; i++)
        {
              move_ghost(retorna_caminho(i,pacman.map_pos),i);
        }  
    }
    void move_ghost(Ponto proximo ,int i)
    {
          if(ghost[i].anim_counter < 20)
          {
              ghost_move_anim(proximo,i);
          }
          else//Atualiza posição do fantasma no grafo
          {
              ghost[i].map_pos.x    = ghost[i].pos.x;
              ghost[i].map_pos.y    = ghost[i].pos.y;
              ghost[i].anim_counter = 0;
              Ponto next = retorna_caminho(i,pacman.map_pos);
              if(next.x ==  ghost[i].map_pos.x && next.y == ghost[i].map_pos.y)
              {
                  estado = Estado::perdeu;
                  return;
              }  
              //std::cout <<" I :  " << i <<  "  X: " <<next.x << "  Y:" << next.y << std::endl;
              ghost[i].current_dir =  ajusta_direcao(next,ghost[i].map_pos); 
          }
         // std::cout << "I:" << i << "  Dir:  " << ghost[i].current_dir << std::endl;
          //std::cout  << "Ghost " << i << "  mapX : " << ghost[i].map_pos.x << " mapY :" << ghost[i].map_pos.y << std::endl;
    }
    void ghost_move_anim(Ponto proximo ,int i)
    {
            switch(ghost[i].current_dir)
            {
                case 1:   ghost[i].pos.y -=PPF;
                          break;
                case 2:   ghost[i].pos.y +=PPF;
                          break;
                case 3:   ghost[i].pos.x -=PPF;
                          break;
                case 4:   ghost[i].pos.x +=PPF;
            }

            ghost[i].anim_sprite_frame++;
            if(ghost[i].anim_sprite_frame==4) ghost[i].anim_sprite_frame = 0;
            ghost[i].anim_counter +=PPF;
       

    }
    
    int ajusta_direcao(Ponto destino,Ponto gp)
    {

        if(gp.y>destino.y)
        {
            return 1; // Sobe

        }
        if(gp.y<destino.y)
        {
            return 2; // Desce

        }
        if(gp.x>destino.x)
        {
            return 3; // Esquerda
        }
        if(gp.x<destino.x)
        {
            return 4; // Direita

        }


    }
        //Retorna o caminho do fantasma i ate o ponto q
    Ponto retorna_caminho(int i, Ponto q){
          Ponto p = ghost[i].map_pos;
          Vertice * a = grafo.busca_vertice(p);
          Vertice * b = grafo.busca_vertice(q);
          grafo.busca_largura_lab(a,b);
          std::vector<Vertice*> caminho = grafo.caminho_curto(p, q);
          return caminho[1]->ponto; // Retorna o próximo vertice que o fantasma i precisa chegar

    }

    bool verifica_fim(void)
    {
      if (estado == Estado::perdeu || estado == Estado::ganhou)
        return true;
      else
        return false;
    }
    void destroi_imagens()
    {
        int i,j;
        for(i=0;i<9;i++)
        {
            t.destroy_imagem(pacman.sprite[i]);
        }
        for(i=0;i<quant_fantasma;i++) 
        {
           for(j=0;j<12;j++)
           {
              t.destroy_imagem(ghost[i].sprite[j]);
           } 
        
        }
    }
    void finaliza(void)
    {
      mapa.libera();
      if(estado == Estado::perdeu)
        printf("\nGAME OVER\n");
      else
        printf("\nYOU WIN \n");
      destroi_imagens(); 
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
