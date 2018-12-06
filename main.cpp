
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

//??
int const tam = 20;
int const quant_fantasma = 4;

using namespace tela;
using namespace geom;
using namespace mapa;
using namespace grafo;
/* estados para o jogo */
enum Estado { nada, menu, jogando, ganhou, perdeu };

//??
enum Direcao {sobe, desce, direita , esquerda};
//??
struct Fantasmas{
    Ponto p;
    ALLEGRO_BITMAP* sprite[8];
    Direcao direcao;
};

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
    //??
    Fantasmas fantasma[quant_fantasma];
    int             tecla;
    int             frame_counter;
    int             score;
    

    void inicia(void)
    {
      t.inicia                (800, 600, "Pacman");
      mapa.cria               ("mapa.txt");
      grafo.cria_grafo        (mapa.map,mapa.lin,mapa.col);
      grafo.cria_lista_adj    ();
      //??
      inicializa_fantasmas();
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
    
    
    //??
    void inicializa_fantasmas(){
        for(int  i =0 ; i<quant_fantasma;i++){
            fantasma[i].p = g.grafo[i*25].ponto;
            fantasma[i].direcao = direita;
            switch(i){
                case 0: inicializa_sprites_ver(i);
                    break;
                case 1: inicializa_sprites_azul(i);
                    break;
                case 2: inicializa_sprites_roxo(i);
                    break;
                case 3: inicializa_sprites_amarelo(i);
                    break;
            }
        }
    }
    //??
    void inicializa_sprites_ver(int i){

        fantasma[i].sprite[0] = t.carrega_imagem("Sprites/RG_U1.png");

        fantasma[i].sprite[1] = t.carrega_imagem("Sprites/RG_D1.png");

        fantasma[i].sprite[2] = t.carrega_imagem("Sprites/RG_R1.png");

        fantasma[i].sprite[3] = t.carrega_imagem("Sprites/RG_L1.png");

        fantasma[i].sprite[4] = t.carrega_imagem("Sprites/RG_U2.png");

        fantasma[i].sprite[5] = t.carrega_imagem("Sprites/RG_D2.png");

        fantasma[i].sprite[6] = t.carrega_imagem("Sprites/RG_R2.png");

        fantasma[i].sprite[7] = t.carrega_imagem("Sprites/RG_L2.png");

    }
    //??
     void inicializa_sprites_azul(int i){

        fantasma[i].sprite[0] = t.carrega_imagem("Sprites/BG_U1.png");

        fantasma[i].sprite[1] = t.carrega_imagem("Sprites/BG_D1.png");

        fantasma[i].sprite[2] = t.carrega_imagem("Sprites/BG_R1.png");

        fantasma[i].sprite[3] = t.carrega_imagem("Sprites/BG_L1.png");

        fantasma[i].sprite[4] = t.carrega_imagem("Sprites/BG_U2.png");

        fantasma[i].sprite[5] = t.carrega_imagem("Sprites/BG_D2.png");

        fantasma[i].sprite[6] = t.carrega_imagem("Sprites/BG_R2.png");

        fantasma[i].sprite[7] = t.carrega_imagem("Sprites/BG_L2.png");
    }
    //??
    void inicializa_sprites_roxo(int i){
        fantasma[i].sprite[0] = t.carrega_imagem("Sprites/PG_U1.png");

        fantasma[i].sprite[1] = t.carrega_imagem("Sprites/PG_D1.png");

        fantasma[i].sprite[2] = t.carrega_imagem("Sprites/PG_R1.png");

        fantasma[i].sprite[3] = t.carrega_imagem("Sprites/PG_L1.png");

        fantasma[i].sprite[4] = t.carrega_imagem("Sprites/PG_U2.png");

        fantasma[i].sprite[5] = t.carrega_imagem("Sprites/PG_D2.png");

        fantasma[i].sprite[6] = t.carrega_imagem("Sprites/PG_R2.png");

        fantasma[i].sprite[7] = t.carrega_imagem("Sprites/PG_L2.png");
    }
    //??
    void inicializa_sprites_amarelo(int i){
        fantasma[i].sprite[0] = t.carrega_imagem("Sprites/YG_U1.png");

        fantasma[i].sprite[1] = t.carrega_imagem("Sprites/YG_D1.png");

        fantasma[i].sprite[2] = t.carrega_imagem("Sprites/YG_R1.png");

        fantasma[i].sprite[3] = t.carrega_imagem("Sprites/YG_L1.png");

        fantasma[i].sprite[4] = t.carrega_imagem("Sprites/YG_U2.png");

        fantasma[i].sprite[5] = t.carrega_imagem("Sprites/YG_D2.png");

        fantasma[i].sprite[6] = t.carrega_imagem("Sprites/YG_R2.png");

        fantasma[i].sprite[7] = t.carrega_imagem("Sprites/YG_L2.png");
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
        desenha_fantasmas();
    }
    //??
    void desenha_fantasmas(){
        for(int i = 0 ;i < quant_fantasma;i++){
            desenha_fantasma_(i);
        }
    }
    //??
    void desenha_fantasma_(int i){
        ALLEGRO_BITMAP* aux;
        if((int)al_get_time()%2==0){
            switch(fantasma[i].direcao){
                case(sobe):      aux = fantasma[i].sprite[0];
                    break;
                case(desce):     aux = fantasma[i].sprite[1];
                    break;
                case(direita):   aux = fantasma[i].sprite[2];
                    break;
                case(esquerda):  aux = fantasma[i].sprite[3];
                    break;

            }
            t.desenha_imagem(aux, {fantasma[i].p.x-tam/2, fantasma[i].p.y-tam/2});
        }else{
            switch(fantasma[i].direcao){
                case(sobe):     aux = fantasma[i].sprite[4];
                    break;
                case(desce):    aux = fantasma[i].sprite[5];
                    break;
                case(direita):  aux = fantasma[i].sprite[6];
                    break;
                case(esquerda): aux = fantasma[i].sprite[7];
                    break;

            }
            t.desenha_imagem(aux, {fantasma[i].p.x-tam/2, fantasma[i].p.y-tam/2});
        }


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
        movimento_fantasmas();
        //move_ghost();
        //detecte_gameover();
    } 
    //??
    //Retorna quanto o fantasma move no x e no y apartir do ponto 
    //e altera a direcao
    std::vector<int> retorna_direcao(Ponto destino, int i){
        int x = 0;
        int y = 0;
        std::vector<int> x_e_y;
        if(fantasma[i].p.y<destino.y){
            fantasma[i].direcao = desce;
            y = 5;
        }else if(fantasma[i].p.y>destino.y){
            fantasma[i].direcao = sobe;
            y = -5;
        }else if(fantasma[i].p.x<destino.x){
            fantasma[i].direcao = direita;
            x = 5;
        }else if(fantasma[i].p.x>destino.x){
            fantasma[i].direcao = esquerda;
            x= -5;
        }
        x_e_y.push_back(x);
        x_e_y.push_back(y);

        return x_e_y;


    }
    //Retorna o caminho do fantasma i ate o ponto q
    std::vector<Vertice*> retorna_caminho(int i, Ponto q){
        Ponto p = fantasma[i].p;
        g.busca_largura_lab(g.busca_vertice(p), g.busca_vertice(q));
        std::vector<Vertice*> caminho = g.caminho_curto(p, q);
        return caminho;

    }


    ///Realiza o movimento dso fantasmas
    void movimento_fantasmas(){
        for(int i = 0 ; i<quant_fantasma;i++){
                movimento_fantasmas_(retorna_caminho(i,g.grafo[g.grafo.size()-4].ponto), i);
        }
    }
    
    //Recebe o caminho e o fantasma i
    //Enquanto o fantasma nao for o mesmo ponto que o ultimo ponto do caminho segue o loop
    //No loop ele procura a posicao do fantasma e se for do grafo ele procura as direcoes que o fantasma
    // deve andar ate o proximo ponto do caminho e ele fica andando esse  e y até que o lugar do fantasma
    // pertenca ao caminho denovo
    void movimento_fantasmas_(std::vector<Vertice*> caminho, int i){
            int antiga_x= 0 ;
            int antiga_y = 0;
            std::vector<Vertice*>::iterator it = caminho.begin();
            while(g.busca_vertice(fantasma[i].p)!=caminho[caminho.size()-1]){
                if(g.busca_vertice(fantasma[i].p)!=nullptr ){
                    it++;
                    std::vector<int> direcoes = retorna_direcao((*it)->ponto, i);
                    antiga_x = direcoes[0];
                    antiga_y = direcoes[1];
                }
                fantasma[i].p.x+=antiga_x;
                fantasma[i].p.y+=antiga_y;
                tecla = t.tecla();
                if(tecla == ALLEGRO_KEY_SPACE)
                        estado = Estado::fim;
                if (estado == Estado::jogo) {
                    t.limpa();
                    desenha();
                    t.mostra();
                    t.espera(16.66);
                }

            }
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



