#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "mapa.cpp"
#include "player.h" //Cuidado com a ordem das bibliotecas!!
#include "bomba.cpp"

#define FPS 60
#define LAR_TELA 840
#define ALT_TELA 680
#define ALT_INFO 20
#define DIV 40

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *eventos = NULL; //Fila de eventos
ALLEGRO_BITMAP *iexplosao = NULL;
ALLEGRO_BITMAP *ibomba = NULL;
ALLEGRO_BITMAP *parede = NULL;
ALLEGRO_BITMAP *pedra = NULL;
ALLEGRO_BITMAP *coracao = NULL;
ALLEGRO_BITMAP *ip1 = NULL;
ALLEGRO_BITMAP *ip2 = NULL;
ALLEGRO_FONT *fonte = NULL;

int verificacoes();
void draw();
void executaeventos();
double tempodecorrido(); //Quanto tempo se passou

player p1, p2;
mapa **m = NULL;
bomba b[QTDBOMBAS];
explosao e[QTDEXPL];
bool fechar = false;
double tempoinicial;

int main(void) {
  int err;
  if ((err = verificacoes())) return err;

  while (fechar == false) {
    tempoinicial = al_get_time();
    executaeventos();
    p1.move(m);
    p2.move(m);
    for (int i = 0; i < QTDBOMBAS; i++)
      b[i].explodir(m, e);
    for (int i = 0; i < QTDEXPL; i++)
      { e[i].some(m); e[i].colide(p1, p2); }
    draw();
    if(tempodecorrido() < 1.0 / FPS) al_rest((1.0 / FPS) - tempodecorrido());
  }

  al_destroy_display(janela);
  return 0;
}

int verificacoes() {
  srand (time(NULL));
  m = novomapa(ALT_TELA / DIV, LAR_TELA / DIV);
  p1 = player(1,1); p2 = player(ALT_TELA / DIV - 2, LAR_TELA / DIV - 2);

  if (!al_init()) {
    fprintf(stderr, "Falha ao inicializar a Allegro 5\n"); return -1;
  }

  if (!al_init_image_addon()) {
    fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n"); return 0;
  }

  if (!al_install_keyboard()) {
    fprintf(stderr, "Falha ao inicializar o teclado.\n"); return -1;
  }

  al_init_font_addon();
  if (!al_init_ttf_addon()) {
    fprintf(stderr, "Falha ao inicializar allegro_ttf.\n"); return -1;
  }

  janela = al_create_display(LAR_TELA, ALT_TELA + ALT_INFO);
  if (!janela) { fprintf(stderr, "Falha ao criar a janela\n"); return -1; }

  eventos = al_create_event_queue();
  if (!eventos){ fprintf(stderr, "Falha ao criar fila de eventos.\n"); return -1; }
    al_register_event_source(eventos, al_get_keyboard_event_source());
  al_register_event_source(eventos, al_get_display_event_source(janela));

  fonte = al_load_font("assets/Pixeled.ttf", 12, 0);

  coracao = al_load_bitmap("assets/coracao1.png");
  parede = al_load_bitmap("assets/parede.png");
  pedra = al_load_bitmap("assets/pedra.png");
  ip1 = al_load_bitmap("assets/p1.png");
  ip2 = al_load_bitmap("assets/p2.png");
  ibomba = al_load_bitmap("assets/bomba1.png");
  iexplosao = al_load_bitmap("assets/explosao3.png");
  if (!parede || !pedra || !ip1 || !ip2) { printf("Falha nas imagens"); return -1; }
  return 0;
}

void draw() {
  int i, j, lin = ALT_TELA / DIV, col = LAR_TELA / DIV;
  al_clear_to_color(al_map_rgb(0, 0, 0));
  al_draw_text(fonte, al_map_rgb(255, 255, 255), 10, ALT_TELA - 10, 0, "P1");
  al_draw_text(fonte, al_map_rgb(255, 255, 255), LAR_TELA - 30, ALT_TELA - 10, 0, "P2");

  for (i = 0; i < p1.vida; i++)
    al_draw_bitmap(coracao, 30 + i * 22, ALT_TELA, 0);
  for (i = 0; i < p2.vida; i++)
    al_draw_bitmap(coracao, LAR_TELA - 53 - i * 22, ALT_TELA, 0);

  for (i = 0; i < lin; i++)
    for (j = 0; j < col; j++)
      if (m[i][j].info == PAREDE) al_draw_bitmap(parede, j * DIV, i * DIV, 0);
      else if(m[i][j].info == PEDRA) al_draw_bitmap(pedra, j * DIV, i * DIV, 0);
  for (i = 0; i < QTDBOMBAS; i++)
    if (b[i].ativa)
      al_draw_bitmap(ibomba, b[i].y * DIV, b[i].x * DIV, 0);
  for (i = 0; i < QTDEXPL; i++)
    if (e[i].ativa)
      al_draw_bitmap(iexplosao, e[i].y * DIV, e[i].x * DIV, 0);
  al_draw_bitmap(ip1, p1.y * DIV, p1.x * DIV, 0);
  al_draw_bitmap(ip2, p2.y * DIV, p2.x * DIV, 0);
  al_flip_display();
}

void executaeventos() {
  while(!al_is_event_queue_empty(eventos)) {
    ALLEGRO_EVENT e;
    al_wait_for_event(eventos, &e);
    if (e.type == ALLEGRO_EVENT_DISPLAY_CLOSE) fechar = true;
    else if (e.type == ALLEGRO_EVENT_KEY_DOWN || e.type == ALLEGRO_EVENT_KEY_UP) {
      int kc = e.keyboard.keycode;
      switch (kc) {
      case ALLEGRO_KEY_W:
        p1.movimento[CIMA] = !p1.movimento[CIMA];
        break;
      case ALLEGRO_KEY_A:
        p1.movimento[ESQUERDA] = !p1.movimento[ESQUERDA];
        break;
      case ALLEGRO_KEY_S:
        p1.movimento[BAIXO] = !p1.movimento[BAIXO];
        break;
      case ALLEGRO_KEY_D:
        p1.movimento[DIREITA] = !p1.movimento[DIREITA];
        break;
      case ALLEGRO_KEY_SPACE:
        novabomba(b, p1.x, p1.y);
      case ALLEGRO_KEY_UP:
        p2.movimento[CIMA] = !p2.movimento[CIMA];
        break;
      case ALLEGRO_KEY_LEFT:
        p2.movimento[ESQUERDA] = !p2.movimento[ESQUERDA];
        break;
      case ALLEGRO_KEY_DOWN:
        p2.movimento[BAIXO] = !p2.movimento[BAIXO];
        break;
      case ALLEGRO_KEY_RIGHT:
        p2.movimento[DIREITA] = !p2.movimento[DIREITA];
        break;
      case ALLEGRO_KEY_RSHIFT:
        novabomba(b, p2.x, p2.y);
      }
    }
  }
}

double tempodecorrido() {
  return al_get_time() - tempoinicial;
}
