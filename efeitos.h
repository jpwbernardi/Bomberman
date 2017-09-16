#define INV_RESPAWN 0 //Invencibilidade ao respawnar
#define INV_RESPAWN_T 3 //Tempo de duração do efeito;

struct efeito {
  int id;
  double tmpinicial, duracao;
  efeito() {};
  efeito(int _id, int _d) : id(_id), duracao(_d) {
    tmpinicial = al_get_time();
  };
  int ativo() {
    return al_get_time() - tmpinicial < duracao;
  };
};
