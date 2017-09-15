#define VIDA 3
#define BOMBAS 5
#define CIMA 0
#define BAIXO 1
#define ESQUERDA 2
#define DIREITA 3
#define ATRASO 0.2

struct player{
  int x, y;
  int vida, bombas;
  double tmpespera;
  char movimento[4];
  player() {
    memset(movimento, 0, sizeof(movimento));
    vida = VIDA; tmpespera = 0;
  };
  player(int _x, int _y) : x(_x), y(_y) {
    memset(movimento, 0, sizeof(movimento));
    vida = VIDA; tmpespera = 0;
  };
  void espera() { tmpespera = al_get_time(); };
  void move(mapa** m) {
    if (al_get_time() - tmpespera < ATRASO) return;
    if (movimento[BAIXO] && m[x + 1][y].info == AR) x++;
    else if (movimento[CIMA] && m[x - 1][y].info == AR) x--;
    else if (movimento[ESQUERDA] && m[x][y - 1].info == AR) y--;
    else if (movimento[DIREITA] && m[x][y + 1].info == AR) y++;
    tmpespera = al_get_time();
  };
};
