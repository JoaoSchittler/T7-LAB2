
#include <iostream>
#include <cstdlib>

#include "Tela.hpp"
#include "geom.hpp"

namespace tela {

constexpr float XU2X(float x) {
    return x;
}
constexpr float YU2X(float y) {
    return y;
}
constexpr float XX2U(float x) {
    return x;
}
constexpr float YX2U(float y) {
    return y;
}

/* Ajustar de acordo com o servidor X, para acertar as cores */
/* se o servidor X estiver em 16 bits, comentar a linha abaixo */
#define VINTE_E_QUATRO
#ifdef VINTE_E_QUATRO
#define B_R 8
#define B_G 8
#define B_B 8
#else
#define B_R 5
#define B_G 6
#define B_B 5
#endif

void Tela::inicia(int larg, int alt, const char *nome) {
    /* inicializa dados sobre o mouse */
    _rato.x = 0;
    _rato.y = 0;
    _botao = false;
    _tecla = 0;
    _keyhold = 0;

    /* inicializa o allegro */
    if (!al_init()) {
        std::cerr << "falha na inicializacao do allegro" << std::endl;
        std::abort();
    }

    /* conecta com tela X */
    display = al_create_display(larg, alt);
    if (display == NULL) {
        std::cerr << "falha ao criar display do allegro" << std::endl;
        std::abort();
    }

    janela.pos.x = 0;
    janela.pos.y = 0;
    tam.larg = larg;
    tam.alt = alt;
    janela.tam.larg = XX2U(tam.larg);
    janela.tam.alt = YX2U(tam.alt);
    tam = tam;

    /* Titulo da tela */
    al_set_window_title(display, nome);

    /* cria contextos grafico */
    ac_fundo = al_map_rgb(50, 50, 50);
    ac_cor = al_map_rgb(0, 0, 0);

    /* instala o driver de mouse e teclado */
    al_install_mouse();
    al_install_keyboard();
    al_install_audio();
    al_init_primitives_addon();
    

    /* configura fonte */
    al_init_font_addon();
    al_init_image_addon();
    al_init_acodec_addon();
    // fonte = al_load_bitmap_font("data/a4_font.tga");
    fonte = al_load_font("data/fixed_font.tga", 0, 0);
    if (!fonte) {
        std::cerr << "falha ao carregar fonte do allegro" << std::endl;
        std::abort();
    }

    /* inicia o timer */
    /* timer = al_create_timer(1.000 / FPS); */

    /* fila para eventos */
    queue = al_create_event_queue();
    if (queue == NULL) {
        std::cerr << "falha ao buscar eventos do allegro" << std::endl;
        std::abort();
    }

    /* registra para receber eventos de tela/teclado/mouse */
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_mouse_event_source());
    // al_register_event_source(queue,
    // al_get_timer_event_source(timer));
}

void Tela::limpa() {
    /* preenche um retangulo do tamanho da tela com a cor de fundo */
    al_clear_to_color(ac_fundo);
}

void Tela::mostra() {
    /* Troca os buffers de video, passando o que foi desenhado para tela */
    al_flip_display();
}

/* tempo de espera em microsegundos */
void Tela::espera(double ms) {
    al_rest(ms / 1e3);
}

void Tela::finaliza() {
    /* o programa vai morrer, o fim da conexao com o servidor X fecha tudo */
    al_destroy_display(display);
    al_destroy_event_queue(queue);
}

int Tela::tecla() {
    /* retorna a ultima _tecla pressionada */
    int tecla;
    processa_eventos();
    tecla = _tecla;
    _tecla = 0;
    return tecla;
}
int Tela::tecla_hold(){
   return _keyhold;
}

void Tela::processa_eventos() {
    /* processa eventos do servidor X, atualizando a posicao do mouse
     * e ultima _tecla pressionada na variavel da tela. */
    ALLEGRO_EVENT event;
    // al_wait_for_event(queue, &event);
    while (al_get_next_event(queue, &event)) {
        switch (event.type) {
        /* _tecla foi pressionada */
        case ALLEGRO_EVENT_KEY_DOWN: {
            /* const char* keyname = al_keycode_to_name(event.keyboard.keycode);
            printf("code %d char '%c' name '%s' (q=%d)\n",
            event.keyboard.keycode,
              (char)event.keyboard.keycode, keyname, 'Q');*/
            _tecla = event.keyboard.keycode;
            _keyhold = 1;
            break;
        }
        case ALLEGRO_EVENT_KEY_UP:  {
            _keyhold = 0;
            break;
        } 
        case ALLEGRO_EVENT_MOUSE_AXES: {
            _rato.x = XX2U(event.mouse.x);
            _rato.y = YX2U(event.mouse.y);
            break;
        }
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: {
            if (event.mouse.button == 1)
            {
                _botao = true;
               
            }
            break;
        }
        case ALLEGRO_EVENT_MOUSE_BUTTON_UP: {
            if (event.mouse.button == 1)
            {
                _botao = false;
            }
            break;
        }
        case ALLEGRO_EVENT_TIMER: {
            break;
        }
#if 0
      /* _botao de fechar janela pressionado */
      case ALLEGRO_EVENT_DISPLAY_CLOSE:
          {
              return;
              break;
          }
#endif
        default:
            break;
        }
        
    }
}

void Tela::retangulo(Retangulo r,Cor cor) {
    /* preenche o retangulo r com a cor padrao */
    al_draw_filled_rectangle(
        /* canto superior esquerdo */
        XU2X(r.pos.x), YU2X(r.pos.y),
        /* canto inferior direito */
        XU2X(r.pos.x + r.tam.larg), YU2X(r.pos.y + r.tam.alt), al_map_rgb(cor.r,cor.g,cor.b));
}

void Tela::circulo(Circulo c,Cor cor) {
    /* preenche o circulo r na tela com a cor padrao */
    al_draw_filled_circle(XU2X(c.centro.x), YU2X(c.centro.y), XU2X(c.raio),
                          al_map_rgb(cor.r,cor.g,cor.b));
}

void Tela::linha(Ponto p1, Ponto p2) {
    /* une os dois pontos com uma linha na cor padrao */
    al_draw_line(XU2X(p1.x), YU2X(p1.y), XU2X(p2.x), YU2X(p2.y), ac_cor, 0.0);
}

#define AJEITA(x) (x < 0 ? 0 : (x > 1 ? 1 : x))
void Tela::cor(Cor c) {
    /* altera a cor padrao */
    int R, G, B;
    R = AJEITA(c.r) * ((1 << B_R) - 1);
    G = AJEITA(c.g) * ((1 << B_G) - 1);
    B = AJEITA(c.b) * ((1 << B_B) - 1);
    ac_cor = al_map_rgb(R, G, B);
}

int Tela::strlen(const char *s) const {
    return al_get_text_width(fonte, s);
}

void Tela::texto(Ponto p, const char *s,Cor c) {
    /* escreve o texto s na posicao p da tela */
    al_draw_text(fonte, 
        al_map_rgb(c.r,c.g,c.b), XU2X(p.x), YU2X(p.y), ALLEGRO_ALIGN_LEFT, s);
}

Ponto Tela::rato() {
    /* retorna a posicao do mouse */
    processa_eventos();
    return _rato;
}

bool Tela::botao() {
    /* retorna o estado do _botao principal do mouse (true == pressionado) */
    processa_eventos();
    return _botao;
}

Tamanho Tela::tamanho_texto(const char *s) {
    Tamanho tam;
    int bbx, bby, bbw, bbh;
    al_get_text_dimensions(fonte, s, &bbx, &bby, &bbw, &bbh);
    tam.larg = bbw;
    tam.alt = bbh;
    return tam;
}

Tamanho Tela::tamanho() const {
    return tam;
}
ALLEGRO_BITMAP* Tela::carrega_imagem(char *path)
{
    ALLEGRO_BITMAP* imagem = al_load_bitmap(path);
    if(!imagem) std::cout << path << " Erro ao carregar\n";
    return imagem;
}
void Tela::destroy_imagem(ALLEGRO_BITMAP* imagem)
{
    al_destroy_bitmap(imagem);
}
void Tela::desenha_imagem(ALLEGRO_BITMAP* image,float x, float y)
{
    al_draw_bitmap(image,x,y,0); 
}
    
void Tela::play_sample(ALLEGRO_SAMPLE* som, ALLEGRO_PLAYMODE mode)
{
    al_play_sample(som, 1.0, 0.0, 1.0,mode, 0);
}

void Tela::play_instance(ALLEGRO_SAMPLE_INSTANCE* instance)
{
    al_play_sample_instance(instance);
}

void Tela::destroy_sample(ALLEGRO_SAMPLE* som)
{
    al_destroy_sample(som);
}

void Tela::destroy_instance(ALLEGRO_SAMPLE_INSTANCE* instance)
{
    al_destroy_sample_instance(instance);
}


}; // namespace tela
