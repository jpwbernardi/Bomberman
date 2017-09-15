#include "bomba.h"

void novabomba(bomba* b, int x, int y) {
  int i;
  for (i = 0; i < QTDBOMBAS; i++)
    if (b[i].ativa == FALSE) break;
  if (i == QTDBOMBAS) return;
  b[i] = bomba(x, y);
}

void novaexpl(explosao* p, int x, int y) {
  int i;
  for (i = 0; i < QTDEXPL; i++)
    if (p[i].ativa == FALSE) break;
  if (i == QTDEXPL) return;
  p[i] = explosao(x, y);
}
