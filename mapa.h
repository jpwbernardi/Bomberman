#define AR 0
#define PEDRA 1
#define TOTAL 2
#define PAREDE 2
#define PLAYER 3

#define ESPACOLIVRE 3

struct mapa {
  int info;
  mapa() { info = AR; };
  void aleatorio() { info = rand() % TOTAL; };
};

mapa** novomapa(int alt, int lar);

