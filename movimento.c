#include "movimento.h"
#include <allegro5/allegro.h>

void teclas(bool move[], int keycode, bool estado){
  if(keycode == ALLEGRO_KEY_UP) move[tUP] = estado;
  if(keycode == ALLEGRO_KEY_DOWN) move[tDOWN] = estado;
  if(keycode == ALLEGRO_KEY_LEFT) move[tLEFT] = estado;
  if(keycode == ALLEGRO_KEY_RIGHT) move[tRIGHT] = estado;
}

void atualiza(bool move[], int &x, int &y){
  if(move[tUP]) y -= SPEED;
  if(move[tDOWN]) y += SPEED;
  if(move[tLEFT]) x -= SPEED;
  if(move[tRIGHT]) x += SPEED;
}
