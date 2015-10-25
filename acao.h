#define tUP 0
#define tDOWN 1
#define tLEFT 2
#define tRIGHT 3
#define SPEED 4
#define DIV 40
#define MAX_B 5

const int PEDRA = 2;
const int PAREDE = 1;
const int LARGURA_TELA = 840;
const int ALTURA_TELA = 680;
const int ALTURA_PLAYER = 35;
const int LARGURA_PLAYER = 35;
const int ALTURA_BOMBA = 30;
const int LARGURA_BOMBA = 30;
const int LINHA = 17;
const int COLUNA = 21;

typedef struct{
  int x, y;
  long tictac;
  bool viva;
}bomba_t;

typedef struct{
  int x, y;
  bool move[4];
  bomba_t bombas[MAX_B];
}player_t;

typedef struct{
  int x, y;
  long tempo;
  bool tipo, viva;
}explosao_t;


//Define se está ou não movento a tecla
void teclas(bool move[], int keycode, bool estado);

//Move o personagem (só funciona para um player) =/
void atualiza(player_t &p, char paredes[LINHA][COLUNA]);

//Testa colisao (WIP)
bool colisao(int x, int y, char paredes[LINHA][COLUNA], int movimento);

//Player coloca uma bomba no mapa
void novaBomba(player_t &p);

//Faz a explosão da bomba
void explode(bomba_t &b);
