#define TRUE 1
#define FALSE 0
#define TMPBOMBA 2
#define TMPEXPL 1
#define QTDBOMBAS 10
#define QTDEXPL 100
#define TAMEXPLNORM 2

struct explosao {
  int x, y;
  double tmpinicial;
  char ativa;
  explosao() { ativa = FALSE; };
  explosao(int _x, int _y) : x(_x), y(_y) {
    ativa = TRUE;
    tmpinicial = al_get_time();
  };
  void some(mapa** m) {
    if (!ativa || al_get_time() - tmpinicial < TMPEXPL) return;
    ativa = FALSE;
    m[x][y].info = AR; //Destroi tudo
  };
  void colide(player &p1, player &p2) {
    if (!ativa) return;
    if (p1.x == x && p1.y == y) p1.dano();
    if (p2.x == x && p2.y == y) p2.dano();
  };
};

void novaexpl(explosao* p, int x, int y);

struct bomba {
  int x, y, tam;
  double tmpinicial;
  char ativa;
  bomba() { ativa = FALSE; tam = TAMEXPLNORM; };
  bomba(int _x, int _y) : x(_x), y(_y) {
    ativa = TRUE; tam = TAMEXPLNORM;
    tmpinicial = al_get_time();
  };
  void explodir(mapa** m, explosao* p) {
    if (!ativa || al_get_time() - tmpinicial < TMPBOMBA) return;
    int i;
    novaexpl(p, x, y);
    for (i = 1; i <= tam; i++) {
      if (m[x + i][y].info == PAREDE) break;
      if (m[x + i][y].info == AR) novaexpl(p, x + i, y);
      if (m[x + i][y].info == PEDRA) { novaexpl(p, x + i, y); break; }
    }
    for (i = 1; i <= tam; i++) {
      if (m[x - i][y].info == PAREDE) break;
      if (m[x - i][y].info == AR) novaexpl(p, x - i, y);
      if (m[x - i][y].info == PEDRA) { novaexpl(p, x - i, y); break; }
    }
    for (i = 1; i <= tam; i++) {
      if (m[x][y + i].info == PAREDE) break;
      if (m[x][y + i].info == AR) novaexpl(p, x, y + i);
      if (m[x][y + i].info == PEDRA) { novaexpl(p, x, y + i); break; }
    }
    for (i = 1; i <= tam; i++) {
      if (m[x][y - i].info == PAREDE) break;
      if (m[x][y - i].info == AR) novaexpl(p, x, y - i);
      if (m[x][y - i].info == PEDRA) { novaexpl(p, x, y - i); break; }
    }
    ativa = FALSE;
  }
};

void novabomba(bomba* b, int x, int y);
