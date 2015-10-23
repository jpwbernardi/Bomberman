#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
using namespace std;

#define tUP 0
#define tDOWN 1
#define tLEFT 2
#define tRIGHT 3
#define SPEED 10

const int LARGURA_TELA = 640;
const int ALTURA_TELA = 480;

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_BITMAP *imagem = NULL;
ALLEGRO_BITMAP *fundo = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;

bool move[4];

int inicializar();

void teclas(int keycode, bool estado){
  if(keycode == ALLEGRO_KEY_UP) move[tUP] = estado;
  if(keycode == ALLEGRO_KEY_DOWN) move[tDOWN] = estado;
  if(keycode == ALLEGRO_KEY_LEFT) move[tLEFT] = estado;
  if(keycode == ALLEGRO_KEY_RIGHT) move[tRIGHT] = estado;
}

void atualiza(int &x, int &y){
  if(move[tUP]) y -= SPEED;
  if(move[tDOWN]) y += SPEED;
  if(move[tLEFT]) x -= SPEED;
  if(move[tRIGHT]) x += SPEED;
}

int main(void){
  bool sair = false;
  int x = 0, y = 0;

  if (!inicializar()) return 0;

  al_draw_bitmap(fundo, 0, 0, 0);

  while (!sair){
    while(!al_is_event_queue_empty(fila_eventos)){
      ALLEGRO_EVENT evento;
      al_wait_for_event(fila_eventos, &evento);
      if (evento.type == ALLEGRO_EVENT_KEY_DOWN) teclas(evento.keyboard.keycode, true);
      else if(evento.type == ALLEGRO_EVENT_KEY_UP) teclas(evento.keyboard.keycode, false);
      else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) sair = true;
    }
    atualiza(x, y);
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
  imagem = al_load_bitmap("bb-1.png");
  fundo = al_load_bitmap("bb.jpg");
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
