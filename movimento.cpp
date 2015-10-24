#include "movimento.h"
#include <allegro5/allegro.h>

void teclas(bool move[], int keycode, bool estado){
  if(keycode == ALLEGRO_KEY_UP) move[tUP] = estado;
  if(keycode == ALLEGRO_KEY_DOWN) move[tDOWN] = estado;
  if(keycode == ALLEGRO_KEY_LEFT) move[tLEFT] = estado;
  if(keycode == ALLEGRO_KEY_RIGHT) move[tRIGHT] = estado;
}

void atualiza(player_t &p, bool paredes[LINHA][COLUNA]){
  if(p.move[tUP] && p.y >= SPEED && !colisao(p.x, p.y - SPEED, paredes, tUP)/*paredes[(p.y - SPEED) / DIV][p.x / DIV]*/) p.y -= SPEED;
  if(p.move[tDOWN] && p.y + ALTURA_PLAYER <= ALTURA_TELA && !colisao(p.x, p.y + SPEED, paredes, tDOWN)/*paredes[(p.y + ALTURA_PLAYER) / DIV][p.x / DIV]*/) p.y += SPEED;
  if(p.move[tLEFT] && p.x >= SPEED && !colisao(p.x - SPEED, p.y, paredes, tLEFT)/*paredes[p.y / DIV][(p.x - SPEED) / DIV]*/) p.x -= SPEED;
  if(p.move[tRIGHT] && p.x + LARGURA_PLAYER <= LARGURA_TELA && !colisao(p.x + SPEED, p.y, paredes, tRIGHT)/* paredes[p.y / DIV][(p.x + LARGURA_PLAYER) / DIV]*/) p.x += SPEED;
}

bool colisao(int x, int y, bool paredes[LINHA][COLUNA], int movimento){
  switch (movimento) {
    case tUP:
      if(paredes[y / DIV][x / DIV] || paredes[y / DIV][(x + LARGURA_PLAYER) / DIV]) return true;
      break;
    case tDOWN:
      if(paredes[(y + ALTURA_PLAYER) / DIV][x / DIV] || paredes[(y + ALTURA_PLAYER) / DIV][(x + LARGURA_PLAYER) / DIV]) return true;
      break;
    case tLEFT:
      if(paredes[y / DIV][x / DIV] || paredes[(y + ALTURA_PLAYER) / DIV][x / DIV]) return true;
      break;
    case tRIGHT:
      if(paredes[y / DIV][(x + LARGURA_PLAYER) / DIV] || paredes[(y + ALTURA_PLAYER) / DIV][(x + LARGURA_PLAYER) / DIV]) return true;
  }
  return false;
}
