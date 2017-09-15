#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "mapa.cpp"
#include "player.h" //Cuidado com a ordem das bibliotecas!!
#include "bomba.cpp"

#define FPS 60
#define ALT_TELA 840
#define LAR_TELA 680
#define DIV 40

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *eventos = NULL; //Fila de eventos
ALLEGRO_BITMAP *iexplosao = NULL;
ALLEGRO_BITMAP *ibomba = NULL;
ALLEGRO_BITMAP *parede = NULL;
ALLEGRO_BITMAP *pedra = NULL;
ALLEGRO_BITMAP *ip1 = NULL;

int verificacoes();
void draw();
void executaeventos();
double tempodecorrido(); //Quanto tempo se passou

player p1(1,1);
mapa **m = NULL;
bomba b[QTDBOMBAS];
explosao e[QTDEXPL];
bool fechar = false;
double tempoinicial;

int main(void) {
  int err;
  printf("%d %d\n", ALT_TELA/DIV, LAR_TELA / DIV);
  if ((err = verificacoes())) return err;

  while (fechar == false) {
    tempoinicial = al_get_time();
    executaeventos();
    p1.move(m);
    for (int i = 0; i < QTDBOMBAS; i++)
      b[i].explodir(m, e);
    for (int i = 0; i < QTDEXPL; i++)
      e[i].some(m);
    draw();
    if(tempodecorrido() < 1.0 / FPS) al_rest((1.0 / FPS) - tempodecorrido());
  }

  al_destroy_display(janela);
  return 0;
}

int verificacoes() {
  srand (time(NULL));
  m = novomapa(LAR_TELA / DIV, ALT_TELA / DIV);

  if (!al_init()) {
    fprintf(stderr, "Falha ao inicializar a Allegro 5\n"); return -1;
  }

  if (!al_init_image_addon()) {
    fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n"); return 0;
  }

  if (!al_install_keyboard()){
    fprintf(stderr, "Falha ao inicializar o teclado.\n"); return -1;
  }

  janela = al_create_display(ALT_TELA, LAR_TELA);
  if (!janela) { fprintf(stderr, "Falha ao criar a janela\n"); return -1; }

  eventos = al_create_event_queue();
  if (!eventos){ fprintf(stderr, "Falha ao criar fila de eventos.\n"); return -1; }
    al_register_event_source(eventos, al_get_keyboard_event_source());
  al_register_event_source(eventos, al_get_display_event_source(janela));

  parede = al_load_bitmap("assets/parede.png");
  pedra = al_load_bitmap("assets/pedra.png");
  ip1 = al_load_bitmap("assets/p1.png");
  ibomba = al_load_bitmap("assets/bomba1.png");
  iexplosao = al_load_bitmap("assets/explosao3.png");
  if (!parede || !pedra || !ip1) { printf("Falha nas imagens"); return -1; }
  return 0;
}

void draw() {
  int i, j, col = ALT_TELA / DIV, lin = LAR_TELA / DIV;
  al_clear_to_color(al_map_rgb(0, 0, 0));
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
      }
    }
  }
}

double tempodecorrido() {
  return al_get_time() - tempoinicial;
}
