#define tUP 0
#define tDOWN 1
#define tLEFT 2
#define tRIGHT 3
#define SPEED 4
#define DIV 40

const int PEDRA = 2;
const int PAREDE = 1;
const int LARGURA_TELA = 840;
const int ALTURA_TELA = 680;
const int ALTURA_PLAYER = 35;
const int LARGURA_PLAYER = 35;
const int LINHA = 17;
const int COLUNA = 21;

typedef struct{
  int x, y;
  bool move[4];
}player_t;
//Define se está ou não movento a tecla
void teclas(bool move[], int keycode, bool estado);

//Move o personagem
void atualiza(player_t &p, char paredes[LINHA][COLUNA]);

//Testa colisao
bool colisao(int x, int y, char paredes[LINHA][COLUNA], int movimento);
