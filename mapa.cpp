#include <stdio.h>
#include "mapa.h"

mapa** novomapa(int alt, int lar) {
  mapa **m;
  m = (mapa **) malloc(alt * sizeof(mapa *));
  for (int i = 0; i < alt; i++)
    m[i] = (mapa *)malloc(lar * sizeof(mapa));
  for (int i = 0; i < alt; i++)
    for (int j = 0; j < lar; j++)
      if (i == 0 || j == 0 || j == lar - 1 || i == alt - 1 ||
          (i % 2 == 0 && j % 2 == 0)) m[i][j].info = PAREDE;
      else if ((i + 1 > ESPACOLIVRE || j + 1 > ESPACOLIVRE) && (i + ESPACOLIVRE < alt || j + ESPACOLIVRE < lar))
        m[i][j].aleatorio();
  return m;
}
