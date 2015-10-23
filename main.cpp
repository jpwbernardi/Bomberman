#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include "movimento.cpp"
using namespace std;

const int LARGURA_TELA = 840;
const int ALTURA_TELA = 640;

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_BITMAP *imagem = NULL;
ALLEGRO_BITMAP *fundo = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;

//Vetor que controla quais direções estão sendo precionadas
bool move[4];
//Matriz do mapa (Considerando 40 px/célula)
bool paredes[ALTURA_TELA / 40][LARGURA_TELA / 40];

int inicializar();

int main(void){
  bool sair = false;
  int x = 0, y = 0;

  if (!inicializar()) return 0;

  al_draw_bitmap(fundo, 0, 0, 0);

  while (!sair){
    while(!al_is_event_queue_empty(fila_eventos)){
      ALLEGRO_EVENT evento;
      al_wait_for_event(fila_eventos, &evento);
      if (evento.type == ALLEGRO_EVENT_KEY_DOWN) teclas(move, evento.keyboard.keycode, true);
      else if(evento.type == ALLEGRO_EVENT_KEY_UP) teclas(move, evento.keyboard.keycode, false);
      else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) sair = true;
    }
    atualiza(move, x, y);
    al_draw_bitmap(fundo, 0, 0, 0);
    al_draw_bitmap(imagem, x, y, 0);
    al_flip_display();
  }

  al_destroy_display(janela);
  al_destroy_event_queue(fila_eventos);

  return 0;
}

int inicializar(){
  if (!al_init()){ fprintf(stderr, "Falha ao inicializar a Allegro.\n");  return 0; }
  if (!al_init_image_addon()){ fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n"); return 0; }
  if (!al_install_keyboard()){ fprintf(stderr, "Falha ao inicializar o teclado.\n"); return false; }

  janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
  imagem = al_load_bitmap("assets/bman.png");
  fundo = al_load_bitmap("assets/bb.jpg");
  fila_eventos = al_create_event_queue();

  for(int i = 0; i < 4; i++) move[i] = false;

  if (!janela){ fprintf(stderr, "Falha ao criar janela.\n"); return 0; }
  if (!imagem || !fundo){ fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n"); al_destroy_display(janela); return 0; }
  if (!fila_eventos){ fprintf(stderr, "Falha ao criar fila de eventos.\n"); al_destroy_display(janela); return 0; }

  al_set_window_title(janela, "Bomberman");
  al_register_event_source(fila_eventos, al_get_display_event_source(janela));
  al_draw_bitmap(imagem, 0, 0, 0);
  al_flip_display();

  al_register_event_source(fila_eventos, al_get_keyboard_event_source());
  al_register_event_source(fila_eventos, al_get_display_event_source(janela));

  return 1;
}
