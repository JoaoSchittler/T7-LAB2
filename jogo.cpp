#include"jogo.hpp"

using namespace jogo;
void Jogo::joga()
{
    
    inicia();
    while (verifica_fim() == false)
    {
        atualiza();
    }
    finaliza();
}
void Jogo::inicia(void)
{
    //Inicia a tela do jogo
    t.inicia                (800, 600, "Pacman");
    //Carrega o mapa padrao do jogo
    mapa.cria               ("mapa.txt");
    //Inicia imagens sons
    carrega_imagens         ();
    carrega_sons            ();
    inicia_mapa();
    estado = Estado::menu;

}
//Função que roda a cada frame do jogo
void Jogo::atualiza(void)
{
    detecta_tecla();
    t.limpa();
    if(estado == Estado::menu)
    {
        desenha_menu_principal();
    }    
    if(estado == Estado::jogando)
    {
        desenha_jogo();
        move_personagens();
        if(powerup_timer>0) powerup_timer--;
        game_timer++;
    }
    if(estado == Estado::perdeu || estado == Estado::ganhou)
    {
        desenha_menu_final();    
    }
    if(estado == Estado::customize)
    {
        mouse_pos    = t.rato();    
        mapa.desenha(t);
        altera_mapa();
        
    }    
    t.mostra();
    t.espera(1000/FPS);
}   
void Jogo::inicia_jogo()
{
    //Cria e preenche o grafo correspondente ao mapa 
    grafo.cria_grafo        (mapa.map,mapa.lin,mapa.col);
    grafo.cria_lista_adj    ();

    //Inicia variáveis 
    inicia_variaveis        ();

     //Som do movimento dos fantasmas
    t.play_instance         (instance);
}
void Jogo::inicia_variaveis()
{
    estado                   = Estado::jogando;
    score                    = 0;
    game_timer               = 0;
    powerup_timer            = 0;
    inicia_pacman();
    inicia_fantasmas();
}

void Jogo::inicia_pacman()
{
    pacman.current_dir       = 4;//Inicia movendo para a direita
    pacman.next_dir          = 4;
    pacman.anim_counter      = 0;
    pacman.anim_sprite_frame = 0;
    pacman.pos               = grafo.grafo[328].ponto;
    pacman.map_pos           = grafo.grafo[328].ponto;
}

void Jogo::inicia_fantasmas()
{
    for(int  i = 0 ; i<quant_fantasma; i++) 
    {
        Ponto p;
        int dir ;
        switch(i){
            case 0: p = grafo.grafo[0].ponto;
                    dir = 4;
                break;
            case 1: p = grafo.grafo[31].ponto;
                    dir = 3;
                break;
            case 2: p = grafo.grafo[grafo.grafo.size()-1].ponto;
                    dir = 3;
                break;
            case 3: p = grafo.grafo[grafo.grafo.size()-33].ponto;
                    dir = 4;
        }
        ghost[i].pos               = p;
        ghost[i].map_pos           = p;
        ghost[i].current_dir       = dir;//Inicia fantasma olhando para a direita
        ghost[i].anim_counter      = 0;
        ghost[i].anim_sprite_frame = 0;
        ghost[i].eaten             = 0;
    }
}
void Jogo::inicia_mapa()
{
    //Cria o mapa original, usado para retornar o mapa à versão default caso o usuário deseja
    mapa_ori =  new char* [mapa.lin];
    for (unsigned int i = 0; i < mapa.lin;i++)
    {
          mapa_ori[i] =  new char [mapa.col];
    }
    //Copia o mapa para o mapa auxiliar
    for(unsigned int i = 0; i < mapa.lin;i++)
    {
        for(unsigned int j = 0; j < mapa.col; j++)
        {
            mapa_ori[i][j] = mapa.map[i][j];
        }    
    }    

}
void Jogo::carrega_imagens()
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

void Jogo::carrega_sons(){
    al_reserve_samples(5);

    som[0] = al_load_sample("Audio/pacman_chomp.wav");

    som[1] = al_load_sample("Audio/pacman_death.wav");

    som[2] = al_load_sample("Audio/pacman_eatfruit.wav");

    som[3] = al_load_sample("Audio/pacman_eatghost.wav");

    som[4] = al_load_sample("Audio/pacman_ghostsound.ogg");

    instance = al_create_sample_instance(som[4]);
    al_set_sample_instance_playmode(instance, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(instance, al_get_default_mixer());

}
 
void Jogo::detecta_tecla()
{
    tecla = t.tecla();
    if(estado == Estado::jogando)
    {
        //Movimentação
        if(tecla == ALLEGRO_KEY_UP || tecla == ALLEGRO_KEY_W)    pacman.next_dir = 1;
        if(tecla == ALLEGRO_KEY_DOWN || tecla == ALLEGRO_KEY_S)  pacman.next_dir = 2;
        if(tecla == ALLEGRO_KEY_LEFT || tecla == ALLEGRO_KEY_A)  pacman.next_dir = 3;
        if(tecla == ALLEGRO_KEY_RIGHT || tecla == ALLEGRO_KEY_D) pacman.next_dir = 4;
    }
    if(estado == Estado::customize)
    {
        if(tecla == ALLEGRO_KEY_R)
        {
            reseta_mapa();
        }    
    }    
    //Menus
    if(tecla == ALLEGRO_KEY_Q) 
        estado = Estado::saiu;

    if(tecla == ALLEGRO_KEY_P && estado != Estado::jogando)
    {
        inicia_jogo();
        reinicia_mapa();
        estado = Estado::jogando;
    }  
    if(tecla == ALLEGRO_KEY_C && estado != Estado::customize)
    {
        botao_last = 0;
        reinicia_mapa();
        estado = Estado::customize;
    }  
    if(tecla == ALLEGRO_KEY_M && estado != Estado::menu)
    { 
        estado = Estado::menu;
    }    
}

void Jogo::desenha_menu_principal()
{
    t.limpa();
    char* title =   "PACMAN";
     //Desenha texto
    t.texto({350,50},title,{255,255,255});

}
void Jogo::reinicia_mapa()
{
    for(unsigned int i = 0; i < mapa.lin; i++)
    {
        for(unsigned int j = 0; j < mapa.col; j++)
        {
           if(mapa.map[i][j] == 'V')
           {
              mapa.map[i][j] = 'C';
           }
           if(mapa.map[i][j] == 'Z')
           {
              mapa.map[i][j] = 'E';
           }

        }
    }    
}
void Jogo::reseta_mapa()
{
    for(unsigned int i = 0; i < mapa.lin; i++)
    {
        for(unsigned int j = 0; j < mapa.col; j++)
        {
           mapa.map[i][j] = mapa_ori[i][j];
        }
    }    
}
void Jogo::altera_mapa()
{
    if(t.botao() == 0 && botao_last == 1)
    {
        botao_up = true;
    }
    else
    {
        botao_up = false;
    }   
    botao_last = t.botao(); 
    //Verifica se o mouse está dentro do mapa
    if(mouse_pos.x >= mapa.xini && mouse_pos.x <= (20*mapa.col) 
        && mouse_pos.y >= mapa.yini && mouse_pos.y <= (20*mapa.lin))
    {
        Ponto upperleft,downright;
        unsigned int lin = floor(mouse_pos.y/20);
        unsigned int col = floor(mouse_pos.x/20);
        upperleft.x = col*20;
        upperleft.y = lin*20;
        downright.x = upperleft.x+20;
        downright.y = upperleft.y+20;
        al_draw_rectangle(upperleft.x,upperleft.y,downright.x,downright.y,al_map_rgb(255,0,0),1);
        // P -> C -> E -> P
        // Parede -> Caminho(Com "pastilha") -> pastilha Especial -> Parede
        if(botao_up)
        {
           if( mapa.map[lin-3][col] == 'P')
           { 
                 mapa.map[lin-3][col] = 'C';
                 return;
           }
           if( mapa.map[lin-3][col] == 'C')
           {
                mapa.map[lin-3][col] = 'E'; 
                return;
           }
           if( mapa.map[lin-3][col] == 'E')
           {
                mapa.map[lin-3][col] = 'P'; 
                return; 
           }

        }    
    }   

}

void Jogo::desenha_jogo()
{
    mapa.desenha(t);
    desenha_pacman();
    desenha_fantasmas();
    desenha_info();
}

void Jogo::desenha_pacman()
{
    //Indexação com o vetor imagens: 2*(dir-1) se anim_sprite_frame = 0, 2*(dir-1)+1se anim_sprite_frame = 1
    int index = 2*(pacman.current_dir-1);
    if(pacman.anim_sprite_frame > 1) index++;

    if(pacman.anim_sprite_frame == 4) index = 8;

    //std::cout << "Index : " << index << std::endl;
    t.desenha_imagem(pacman.sprite[index],pacman.pos.x-6,pacman.pos.y-5);
}

void Jogo::desenha_info()
{
    const char* msg_timer = "TIME : ";
    const char* msg_score = "SCORE: ";
    Ponto p1 = {100,10};
    Ponto p2 = {100,30};
    t.texto(p1,msg_score,{255,255,255});
    t.texto(p2,msg_timer,{255,255,255});

    std::string s = std::to_string(score);
    const char  *Score = s.c_str();
    p1.x += 100;
    t.texto(p1, Score,{255,255,255});

    s = std::to_string(game_timer/FPS);
    char const *Time  = s.c_str();
    p2.x += 100;
    t.texto(p2,Time,{255,255,255});
}

void Jogo::desenha_fantasmas()
{
    for(int i = 0 ;i < quant_fantasma;i++)
    {
        desenha_fantasma(i);
    }
}

void Jogo::desenha_fantasma(int i)
{
    int index;
    if(powerup_timer==0)
    {
            //UP = 1,DOWN = 2, LEFT =3 RIGHT = 4
            index = 2*(ghost[i].current_dir-1);
            if(ghost[i].anim_sprite_frame > 1) index++;
    }
    else
    {

        if(ghost[i].eaten == 1)
            index = 10;
        else
            index = 8;
        //Powerup está ativo
        //std::cout<<"Index : "<<index;
        if(ghost[i].anim_sprite_frame > 1) index++;
    }
    t.desenha_imagem(ghost[i].sprite[index],ghost[i].pos.x-6,ghost[i].pos.y-5);
}

void Jogo::move_personagens()
{
    move_pac();
    move_ghosts();
}

bool Jogo::existe_pontos(){
    for(unsigned int i = 0;i<mapa.lin;i++){
        for(unsigned j  = 0; j<mapa.col;j++){
            if(mapa.map[i][j] == 'C')
                return true;
        }
    }

    return false;
}


void Jogo::move_pac()
{
    if(pacman.anim_counter < TAM)
    {
        //Continua a animação de movimento
        pac_move_anim();
    }
    else
    {
        //Pacman chegou no proximo vertice do grafo
        unsigned int colindex =  (pacman.pos.x-mapa.xini)/TAM;
        unsigned int linindex =  (pacman.pos.y-mapa.yini)/TAM;

        if(mapa.map[linindex][colindex]=='C')
        {
            score+=10;
            mapa.map[linindex][colindex]= 'V';
            //Som de comeu ponto
            t.play_sample(som[0],ALLEGRO_PLAYMODE_ONCE);
            //std::cout << "Score: " << score << std::endl;
        }

        if(mapa.map[linindex][colindex]=='E')
        {
            score++;
            mapa.map[linindex][colindex]= 'Z';
            //Som de comeu power up
            t.play_sample(som[2],ALLEGRO_PLAYMODE_ONCE);
            powerup_timer += 20*FPS; //20 segundos de powerup
        }

        if(existe_pontos() == false){
            estado = Estado::ganhou;
        }

        pacman.map_pos.x = pacman.pos.x;
        pacman.map_pos.y = pacman.pos.y;
        pacman.anim_counter = 0;
        pacman.current_dir = pacman.next_dir;
    }

}

    //Animações
void Jogo::pac_move_anim()
{
    //Realiza o movimento
    std::vector <Vertice*> saidas = grafo.busca_vizinhos(pacman.map_pos);
    int i = 0;
    bool moveu = false;
    int tam = saidas.size();

    while(i < tam)
    {
        if((pacman.current_dir==1) && (saidas[i]->ponto.y == pacman.map_pos.y-TAM))
        {
            pacman.pos.y -=PPF;
            moveu = true;
        }

        if((pacman.current_dir==2) && (saidas[i]->ponto.y == pacman.map_pos.y+TAM))
        {
            pacman.pos.y +=PPF;
            moveu = true;
        }

        if((pacman.current_dir==3) && (saidas[i]->ponto.x == pacman.map_pos.x-TAM))
        {
            pacman.pos.x -=PPF;
            moveu = true;
        }

        if((pacman.current_dir==4) && (saidas[i]->ponto.x == pacman.map_pos.x+TAM))
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


void Jogo::move_ghosts()
{
    if(powerup_timer == 0)
    {
        for(int i = 0;i < quant_fantasma; i++)
        {
            if(estado == Estado::jogando)
            {
                ghost[i].eaten = 0;
                move_ghost(retorna_caminho(i,pacman.map_pos),i);
            }
        }
    }
    else
    {
        for(int i = 0;i < quant_fantasma; i++)
        {
            move_ghost2(retorna_caminho_aleatorio(i),i);
        }
    }
}
bool Jogo::colidiu(int i)
{
    float gx = ghost[i].pos.x;
    float gy = ghost[i].pos.y;
    float px = pacman.pos.x;
    float py = pacman.pos.y;
    if( (gx >= (px-5)) && (gx <= (px+5)) && (gy >= (py-5)) && (gy<=(py+5))) 
        return true;
    else 
        return  false;   
} 
void Jogo::move_ghost(Ponto proximo ,int i)
{
    if(colidiu(i) == true)
    {

            estado = Estado::perdeu;
            //Som de quando o pac morre
            t.play_sample(som[1],ALLEGRO_PLAYMODE_ONCE);
            return;

    }
    if(ghost[i].anim_counter < TAM)
    {
        ghost_move_anim(proximo,i);
    }
    else//Atualiza posição do fantasma no grafo
    {
        ghost[i].map_pos.x    = ghost[i].pos.x;
        ghost[i].map_pos.y    = ghost[i].pos.y;
        ghost[i].anim_counter = 0;
        Ponto next = retorna_caminho(i,pacman.map_pos);

        if(ghost[i].map_pos.x !=  next.x || ghost[i].map_pos.y != next.y)
        {
           ghost[i].current_dir =  ajusta_direcao(next,ghost[i].map_pos);
        }
    }

}

void Jogo::move_ghost2(Ponto proximo ,int i)
{
    if(colidiu(i)==true  && ghost[i].eaten == 0)
    {
            t.play_sample(som[3],ALLEGRO_PLAYMODE_ONCE);
            ghost[i].eaten  = 1;
            score+=100;
            return;

    }
    if(ghost[i].anim_counter < TAM)
    {
        ghost_move_anim(proximo,i);
    }
    else//Atualiza posição do fantasma no grafo
    {
        ghost[i].map_pos.x    = ghost[i].pos.x;
        ghost[i].map_pos.y    = ghost[i].pos.y;
        ghost[i].anim_counter = 0;
        Ponto next = retorna_caminho_aleatorio(i);
        if(ghost[i].map_pos.x !=  next.x || ghost[i].map_pos.y != next.y)
        {
           ghost[i].current_dir =  ajusta_direcao(next,ghost[i].map_pos);
        }

    }
}

void Jogo::ghost_move_anim(Ponto proximo ,int i)
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

int Jogo::ajusta_direcao(Ponto destino,Ponto gp)
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

    ///Retorna um ponto vizinho aleatorio a partir do fantasma i
Ponto Jogo::retorna_caminho_aleatorio(int i)
{
    Ponto p = ghost[i].map_pos;
    std::vector<Vertice*> vizinhos = grafo.busca_vizinhos(p);

    //Semente
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rand(seed);
    std::uniform_int_distribution<> intervalo(0,vizinhos.size()-1);
    int index = intervalo(rand);

    return vizinhos[index]->ponto;
}

   //Retorna o caminho do fantasma i ate o ponto q
Ponto Jogo::retorna_caminho(int i, Ponto q)
{
    Ponto p = ghost[i].map_pos;
    Vertice * a = grafo.busca_vertice(p);
    Vertice * b = grafo.busca_vertice(q);

    std::vector<Vertice*> outros_fantasmas;
    for(int j = 0 ;j < quant_fantasma; j++){
        if(j!=i){
            outros_fantasmas.push_back(grafo.busca_vertice(ghost[j].map_pos));
        }
    }

    grafo.busca_largura_lab(a,b, outros_fantasmas);
    std::vector<Vertice*> caminho = grafo.caminho_curto(p, q);
    if(caminho.size()>1)
        return caminho[1]->ponto; // Retorna o próximo vertice que o fantasma i precisa chegar
    else
        return caminho[0]->ponto;

}
void Jogo::desenha_menu_final()
{
    t.limpa();
    char* msg1 = "GAME OVER";
    char* msg2 = "Pontuacao final";
    char* msgw = "Voce venceu!";
    char* msgl = "Voce perdeu";
    char* msg3 = "Aperte Q para sair";
    t.texto({367,100},msg1,{255,255,255});

    if(estado == Estado::ganhou)
        t.texto({360,200},msgw,{255,255,255});
    else
        t.texto({360,200},msgl,{255,255,255});
    t.texto({330,300},msg2,{255,255,255});
    std::string s = std::to_string(score);
    const char  *Score = s.c_str();
    t.texto({385,325}, Score,{255,255,255});
    t.texto({320,400},msg3,{255,255,255});
}
bool Jogo::verifica_fim(void)
{
    if (estado == Estado::saiu)
        return true;
    else
        return false;
}

void Jogo::destroi_imagens()
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

void Jogo::destroi_sons()
{
    for(int i = 0;i<5;i++){
        t.destroy_sample(som[i]);
    }
    t.destroy_instance(instance);
}

void Jogo::finaliza(void)
{
    mapa.libera();
    if(estado == Estado::saiu)
        printf("\nGAME OVER\n");
    destroi_imagens();
    destroi_sons();
    t.finaliza();
}
