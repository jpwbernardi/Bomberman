#include "movimento.h"
#include <allegro5/allegro.h>

void teclas(bool move[], int keycode, bool estado){
  if(keycode == ALLEGRO_KEY_UP) move[tUP] = estado;
  if(keycode == ALLEGRO_KEY_DOWN) move[tDOWN] = estado;
  if(keycode == ALLEGRO_KEY_LEFT) move[tLEFT] = estado;
  if(keycode == ALLEGRO_KEY_RIGHT) move[tRIGHT] = estado;
}

void atualiza(player_t &p, bool paredes[17][21]){
  if(p.move[tUP] && p.y >= SPEED && !paredes[(p.y - SPEED) / DIV][p.x / DIV]) p.y -= SPEED;
  if(p.move[tDOWN] && p.y + ALTURA_PLAYER <= ALTURA_TELA && !paredes[(p.y + ALTURA_PLAYER) / DIV][p.x / DIV]) p.y += SPEED;
  if(p.move[tLEFT] && p.x >= SPEED && !paredes[p.y / DIV][(p.x - SPEED) / DIV]) p.x -= SPEED;
  if(p.move[tRIGHT] && p.x + LARGURA_PLAYER <= LARGURA_TELA && !paredes[p.y / DIV][(p.x + LARGURA_PLAYER) / DIV]) p.x += SPEED;
}
