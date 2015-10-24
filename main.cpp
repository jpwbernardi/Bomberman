#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <string.h>
#include "movimento.cpp"
using namespace std;

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_BITMAP *imagem = NULL;
ALLEGRO_BITMAP *fundo = NULL;
ALLEGRO_BITMAP *parede = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;

player_t p1;

//Matriz do mapa (Considerando 40 px/célula)
bool paredes[17][21];

void draw();
int inicializar();

int main(void){
  bool sair = false;

  if (!inicializar()) return 0;

  al_draw_bitmap(fundo, 0, 0, 0);

  while (!sair){
    while(!al_is_event_queue_empty(fila_eventos)){
      ALLEGRO_EVENT evento;
      al_wait_for_event(fila_eventos, &evento);
      if (evento.type == ALLEGRO_EVENT_KEY_DOWN) teclas(p1.move, evento.keyboard.keycode, true);
      else if(evento.type == ALLEGRO_EVENT_KEY_UP) teclas(p1.move, evento.keyboard.keycode, false);
      else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) sair = true;
    }
    atualiza(p1, paredes);
    draw();
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
  parede = al_load_bitmap("assets/wall.png");
  fila_eventos = al_create_event_queue();

  int i, j;

  for(i = 0; i < 4; i++) p1.move[i] = false;
  p1.x = p1.y = 40;

  /* Montar obstaculos */
  memset(paredes, false, sizeof(paredes));
  for(i = 0; i < 21; i++) { paredes[0][i] = true; paredes[16][i] = true; }
  for(i = 0; i < 17; i++) { paredes[i][0] = true; paredes[i][20] = true; }

  for(j = 2; j < 21; j+= 2)
    for(i = 2; i < 15; i++)
      paredes[i][j] = true;
  /* ------------------ */

  if (!janela){ fprintf(stderr, "Falha ao criar janela.\n"); return 0; }
  if (!imagem || !fundo || !parede){ fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n"); al_destroy_display(janela); return 0; }
  if (!fila_eventos){ fprintf(stderr, "Falha ao criar fila de eventos.\n"); al_destroy_display(janela); return 0; }

  al_set_window_title(janela, "Bomberman");
  al_register_event_source(fila_eventos, al_get_display_event_source(janela));
  al_draw_bitmap(imagem, 0, 0, 0);
  al_flip_display();

  al_register_event_source(fila_eventos, al_get_keyboard_event_source());
  al_register_event_source(fila_eventos, al_get_display_event_source(janela));

  return 1;
}

void draw(){
  int i, j;
  al_draw_bitmap(fundo, 0, 0, 0);
  al_draw_bitmap(imagem, p1.x, p1.y, 0);
  for(i = 0; i < 17; i++)
    for(j = 0; j < 21; j++)
      if(paredes[i][j]) al_draw_bitmap(parede, j * DIV, i * DIV, 0);
}
