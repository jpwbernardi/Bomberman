#include <stdio.h>
#include "mapa.cpp"
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#define FPS 60
#define ALT_TELA 840
#define LAR_TELA 680
#define DIV 40

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *eventos = NULL; //Fila de eventos
ALLEGRO_BITMAP *parede = NULL;
ALLEGRO_BITMAP *pedra = NULL;

int verificacoes();
void draw();
void executaeventos();
double tempodecorrido(); //Quanto tempo se passou

mapa **m = NULL;
bool fechar = false;
double tempoinicial;

int main(void) {
  int err;
  printf("%d %d\n", ALT_TELA/DIV, LAR_TELA / DIV);
  if ((err = verificacoes())) return err;

  while (fechar == false) {
    executaeventos();
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

  parede = al_load_bitmap("assets/wall.png");
  pedra = al_load_bitmap("assets/rock.png");
  if (!parede || !pedra) { printf("Falha nas imagens"); return -1; }
  
  return 0;
}

void draw() {
  int i, j, col = ALT_TELA / DIV, lin = LAR_TELA / DIV;
  for(i = 0; i < lin; i++)
    for(j = 0; j < col; j++)
      if(m[i][j].info == PAREDE) al_draw_bitmap(parede, j * DIV, i * DIV, 0);
      else if(m[i][j].info == PEDRA) al_draw_bitmap(pedra, j * DIV, i * DIV, 0);
  al_flip_display();
}

void executaeventos() {
  while(!al_is_event_queue_empty(eventos)) {
      ALLEGRO_EVENT e;
      al_wait_for_event(eventos, &e);
      if (e.type == ALLEGRO_EVENT_DISPLAY_CLOSE) fechar = true;
  }
}

double tempodecorrido() {
  return al_get_time() - tempoinicial;
}
