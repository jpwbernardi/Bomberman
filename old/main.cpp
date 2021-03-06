//g++ -lallegro -lallegro_font -lallegro_ttf -lallegro_image main.cpp -Wall -O2
#include <time.h>
#include <stdio.h>
#include "acao.cpp"
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
using namespace std;

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_BITMAP *imagem = NULL;
ALLEGRO_BITMAP *imagem1 = NULL;
ALLEGRO_BITMAP *fundo = NULL;
ALLEGRO_BITMAP *parede = NULL;
ALLEGRO_BITMAP *pedra = NULL;
ALLEGRO_BITMAP *bomba = NULL;
ALLEGRO_BITMAP *explosao = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_FONT *fonte = NULL;

player_t p1, p2;
double tempoInicial;

//Matriz do mapa (Considerando 40 px/célula), contem pedras(quebraveis) e paredes fixas;
char paredes[LINHA][COLUNA];
explosao_t pilhaExp[200];

void draw();
void criamapa();
int inicializar();
double getTime();

int main(void){
  bool sair = false;

  if (!inicializar()) return 0;

  al_draw_bitmap(fundo, 0, 0, 0);

  while (!sair){
    tempoInicial = al_get_time();

    while(!al_is_event_queue_empty(fila_eventos)){
      ALLEGRO_EVENT evento;
      al_wait_for_event(fila_eventos, &evento);
      if(evento.type == ALLEGRO_EVENT_KEY_DOWN && evento.keyboard.keycode == ALLEGRO_KEY_SPACE) novaBomba(p2);
      else if(evento.type == ALLEGRO_EVENT_KEY_DOWN && evento.keyboard.keycode == ALLEGRO_KEY_RCTRL) novaBomba(p1);
      else if(evento.type == ALLEGRO_EVENT_KEY_DOWN && evento.keyboard.keycode == ALLEGRO_KEY_F1) criamapa();
      else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) teclas(p1.move, p2.move, evento.keyboard.keycode, true);
      else if(evento.type == ALLEGRO_EVENT_KEY_UP) teclas(p1.move, p2.move, evento.keyboard.keycode, false);
      else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) sair = true;
    }

    if(!p1.vivo) { printf("Player 1 morto."); }
    if(!p2.vivo) { printf("Player 2 morto."); }

    atualiza(p1, paredes);
    atualiza(p2, paredes);
    draw();

    al_flip_display();

    if(getTime() < 1.0 / FPS) al_rest((1.0 / FPS) - getTime());
  }

  al_destroy_display(janela);
  al_destroy_event_queue(fila_eventos);

  return 0;
}

int inicializar(){
  srand(time(NULL)); //Para gerar aleatórios mais aleatorios(??)
  if (!al_init()){ fprintf(stderr, "Falha ao inicializar a Allegro.\n");  return 0; }
  if (!al_init_image_addon()){ fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n"); return 0; }
  if (!al_install_keyboard()){ fprintf(stderr, "Falha ao inicializar o teclado.\n"); return false; }
  al_init_font_addon();
  if (!al_init_ttf_addon()){ fprintf(stderr, "Falha ao inicializar allegro_ttf.\n"); return false; }

  janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
  fundo = al_load_bitmap("assets/bb.jpg");
  parede = al_load_bitmap("assets/wall.png");
  pedra = al_load_bitmap("assets/rock.png");
  imagem = al_load_bitmap("assets/bman2.png");
  imagem1 = al_load_bitmap("assets/bman22.png");
  bomba = al_load_bitmap("assets/bomb.png");
  explosao = al_load_bitmap("assets/expl2.png");
  fila_eventos = al_create_event_queue();
  fonte = al_load_font("assets/animated.ttf", 32, 0);

  int i;
  for(i = 0; i < 200; i++) pilhaExp[i].viva = false;
  for(i = 0; i < MAX_B; i++){ p1.bombas[i].viva = false; p2.bombas[i].viva = false; }
  for(i = 0; i < 4; i++){ p1.move[i] = false; p2.move[i] = false; }
  p2.x = p2.y = 40; p1.x = LARGURA_TELA - 78; p1.y = ALTURA_TELA - 78;
  p2.vivo = p1.vivo = true;
  criamapa();


  if (!janela){ fprintf(stderr, "Falha ao criar janela.\n"); return 0; }
  if (!imagem || !fundo || !parede || !pedra || !bomba || !explosao){ fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n"); al_destroy_display(janela); return 0; }
  if (!fila_eventos){ fprintf(stderr, "Falha ao criar fila de eventos.\n"); al_destroy_display(janela); return 0; }

  al_set_display_icon(janela, imagem);
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
  long tmp;
  al_draw_bitmap(fundo, 0, 0, 0);
  al_draw_bitmap(imagem, p1.x, p1.y, 0);
  al_draw_bitmap(imagem1, p2.x, p2.y, 0);
  for(i = 0; i < LINHA; i++)
    for(j = 0; j < COLUNA; j++)
      if(paredes[i][j] == PAREDE) al_draw_bitmap(parede, j * DIV, i * DIV, 0);
      else if(paredes[i][j] == PEDRA) al_draw_bitmap(pedra, j * DIV, i * DIV, 0);
  for(i = 0; i < MAX_B; i++){
    tmp = clock();
    if(p1.bombas[i].viva && (tmp - p1.bombas[i].tictac) < 100000) //Mais ou menos 3 segundos... eu acho...
      al_draw_bitmap(bomba, p1.bombas[i].x + (LARGURA_PLAYER - LARGURA_BOMBA) / 2.0, p1.bombas[i].y + (ALTURA_PLAYER - ALTURA_BOMBA) / 2.0, 0);
    else if(p1.bombas[i].viva) explode(p1.bombas[i], pilhaExp, p1, p2, paredes);
    tmp = clock();
    if(p2.bombas[i].viva && (tmp - p2.bombas[i].tictac) < 100000) //Mais ou menos 3 segundos... eu acho...
      al_draw_bitmap(bomba, p2.bombas[i].x + (LARGURA_PLAYER - LARGURA_BOMBA) / 2.0, p2.bombas[i].y + (ALTURA_PLAYER - ALTURA_BOMBA) / 2.0, 0);
    else if(p2.bombas[i].viva) explode(p2.bombas[i], pilhaExp, p1, p2, paredes);
  }
  for(i = 0; i < 200; i++){
    tmp = clock();
    if(pilhaExp[i].viva && (tmp - pilhaExp[i].tempo) < 10000 * 3) al_draw_bitmap(explosao, pilhaExp[i].x, pilhaExp[i].y, 0);
    else if(pilhaExp[i].viva) pilhaExp[i].viva = false;
  }
}

void criamapa(){
  int i, j;
  /* Montar obstaculos */
  memset(paredes, 0, sizeof(paredes));
  for(i = 0; i < COLUNA; i++) { paredes[0][i] = PAREDE; paredes[16][i] = PAREDE; }
  for(i = 0; i < LINHA; i++) { paredes[i][0] = PAREDE; paredes[i][20] = PAREDE; }

  //paredes[10][10] = true; paredes[10][8] = true;
  for(j = 2; j < COLUNA - 2; j++)
    for(i = 2; i < 15; i++)
      paredes[i][j] = (rand() % 4) % 3;
  for(i = 3; i < COLUNA - 3; i++){
    int x = rand() % 3;
    if(x == 1) paredes[1][i] = PEDRA;
    x = rand() % 3;
    if(x == 1) paredes[LINHA - 2][i] = PEDRA;
  }
  for(i = 3; i < LINHA - 3; i++){
    int x = rand() % 3;
    if(x == 1) paredes[i][1] = PEDRA;
    x = rand() % 3;
    if(x == 1) paredes[i][COLUNA - 2] = PEDRA;
  }
  /* ------------------ */
}

double getTime(){
  return al_get_time() - tempoInicial;
}
