#include "acao.h"
#include <time.h>
#include <allegro5/allegro.h>

void teclas(bool p1[], bool p2[], int keycode, bool estado){
  if(keycode == ALLEGRO_KEY_UP) p1[tUP] = estado;
  if(keycode == ALLEGRO_KEY_DOWN) p1[tDOWN] = estado;
  if(keycode == ALLEGRO_KEY_LEFT) p1[tLEFT] = estado;
  if(keycode == ALLEGRO_KEY_RIGHT) p1[tRIGHT] = estado;
  if(keycode == ALLEGRO_KEY_W) p2[tUP] = estado;
  if(keycode == ALLEGRO_KEY_S) p2[tDOWN] = estado;
  if(keycode == ALLEGRO_KEY_A) p2[tLEFT] = estado;
  if(keycode == ALLEGRO_KEY_D) p2[tRIGHT] = estado;
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
    }}

//Pelamor de deus, melhora esse código :P
void explode(bomba_t &b, explosao_t e[200], player_t &p1, player_t &p2, char paredes[LINHA][COLUNA]){
  int i, j, k = -1, flag, flag2;
  int raio = 3;
  b.viva = false; /*printf("Tempo passado: %lf\n", double(tmp - b.tictac) / CLOCKS_PER_SEC);*/
  for(k = j = b.y, flag2 = flag = i = 0; i < raio; i++){
    if(!flag){
      for(; j < b.y + i * ALTURA_EXPL + ALTURA_EXPL && !(flag |= colisaoExplosao(b.x, j, paredes, VERTICAL, p1, p2)); j++);
      novaExpl(b.x, j, e);
    }
    if(!flag2){
      for(; k > b.y - i * ALTURA_EXPL - ALTURA_EXPL && !(flag2 |= colisaoExplosao(b.x, k, paredes, VERTICAL, p1, p2)); k--);
      novaExpl(b.x, k, e);
    }}
  for(k = j = b.x, flag2 = flag = i = 0; i < raio; i++){
    if(!flag){
      for(; j < b.x + i * LARGURA_EXPL + LARGURA_EXPL && !(flag |= colisaoExplosao(j, b.y, paredes, HORIZONTAL, p1, p2)); j++);
      novaExpl(j, b.y, e);
    }
    if(!flag2){
      for(; k > b.x - i * LARGURA_EXPL - LARGURA_EXPL && !(flag2 |= colisaoExplosao(k, b.y, paredes, HORIZONTAL, p1, p2)); k--);
      novaExpl(k, b.y, e);
    }}}

bool colisaoExplosao(int x, int y, char paredes[LINHA][COLUNA], char direcao, player_t &p1, player_t &p2){
  int X = x + LARGURA_EXPL / 2, Y = y + ALTURA_EXPL / 2, flag = 0;
  flag |= testaCE(X, Y, paredes, p1, p2);
  if(direcao == HORIZONTAL){
    flag |= testaCE(X, y, paredes, p1, p2);
    flag |= testaCE(X, y + ALTURA_EXPL, paredes, p1, p2);
  } else {
    flag |= testaCE(x, Y, paredes, p1, p2);
    flag |= testaCE(x + LARGURA_EXPL, Y, paredes, p1, p2);
  }
  return flag;
}

bool testaCE(int x, int y, char paredes[LINHA][COLUNA], player_t &p1, player_t &p2){
  if(y / DIV == (p1.y + ALTURA_PLAYER) / DIV && x / DIV == (p1.x + LARGURA_PLAYER) / DIV){
    p1.vivo = false;
  }
  if(y / DIV == (p2.y + ALTURA_PLAYER) / DIV && x / DIV == (p2.x + LARGURA_PLAYER) / DIV){
    p2.vivo = false;
  }
  if(paredes[y / DIV][x / DIV]){
    if(paredes[y / DIV][x / DIV] == PEDRA) paredes[y / DIV][x / DIV] = 0;
    return true;
  }
  return false;
}

void novaExpl(int x, int y, explosao_t e[200]){
  for(int k = 0; k < 200; k++)
    if(!e[k].viva){
      e[k].tempo = clock();
      e[k].viva = true;
      e[k].x = x;
      e[k].y = y;
      return;
    }
}
