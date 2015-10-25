#include "acao.h"
#include <time.h>
#include <allegro5/allegro.h>

void teclas(bool move[], int keycode, bool estado){
  if(keycode == ALLEGRO_KEY_UP) move[tUP] = estado;
  if(keycode == ALLEGRO_KEY_DOWN) move[tDOWN] = estado;
  if(keycode == ALLEGRO_KEY_LEFT) move[tLEFT] = estado;
  if(keycode == ALLEGRO_KEY_RIGHT) move[tRIGHT] = estado;
}

void atualiza(player_t &p, char paredes[LINHA][COLUNA]){
  if(p.move[tUP] && p.y >= SPEED && !colisao(p.x, p.y - SPEED, paredes, tUP)/*paredes[(p.y - SPEED) / DIV][p.x / DIV]*/) p.y -= SPEED;
  if(p.move[tDOWN] && p.y + ALTURA_PLAYER <= ALTURA_TELA && !colisao(p.x, p.y + SPEED, paredes, tDOWN)/*paredes[(p.y + ALTURA_PLAYER) / DIV][p.x / DIV]*/) p.y += SPEED;
  if(p.move[tLEFT] && p.x >= SPEED && !colisao(p.x - SPEED, p.y, paredes, tLEFT)/*paredes[p.y / DIV][(p.x - SPEED) / DIV]*/) p.x -= SPEED;
  if(p.move[tRIGHT] && p.x + LARGURA_PLAYER <= LARGURA_TELA && !colisao(p.x + SPEED, p.y, paredes, tRIGHT)/* paredes[p.y / DIV][(p.x + LARGURA_PLAYER) / DIV]*/) p.x += SPEED;
}

bool colisao(int x, int y, char paredes[LINHA][COLUNA], int movimento){
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

void novaBomba(player_t &p){
  int i;
  for(i = 0; i < MAX_B; i++)
    if(!p.bombas[i].viva){
      p.bombas[i].viva = true;
      p.bombas[i].tictac = clock();
      p.bombas[i].x = p.x; p.bombas[i].y = p.y;
      break;
    }
}

void explode(bomba_t &b, explosao_t e[200]){
  int i, j, k = -1, dx[] = {0, 0, -1, 1}, dy[] = {-1, 1, 0, 0};
  b.viva = false;
  for(j = 1; j <= 3; j++)
    for(i = 0; i < 4; i++)
      for(k++; k < 200; k++)
        if(!e[k].viva){
          e[k].tempo = clock();
          e[k].viva = true;
          e[i].x = b.x + dx[i] * j * 10;
          e[i].y = b.y + dy[i] * j * 10;
          break;
        }
}
