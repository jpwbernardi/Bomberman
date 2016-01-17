//Movimentos do personagem
#define tUP 0
#define tDOWN 1
#define tLEFT 2
#define tRIGHT 3
//Velocidade do personagem
#define SPEED 4
//Divisões na janela para criar a matriz
#define DIV 40
//Numero maximo de bombas
#define MAX_B 5
//Direções para explosão
#define VERTICAL 1
#define HORIZONTAL 2
#define FPS 60

//Código para o mapa
const int PEDRA = 2;
const int PAREDE = 1;
//Altura e largura da janela
const int LARGURA_TELA = 840;
const int ALTURA_TELA = 680;
//Altura e largura do jogador
const int ALTURA_PLAYER = 35;
const int LARGURA_PLAYER = 35;
//Altura e largura da bomba
const int ALTURA_BOMBA = 30;
const int LARGURA_BOMBA = 30;
//Altura e largura da explosao
const int ALTURA_EXPL = 35;
const int LARGURA_EXPL = 35;
//Quantidade de linhas e colunas da matriz (Deve ser LARGURA_TELA / DIV e ALTURA_TELA / DIV)
const int LINHA = 17;
const int COLUNA = 21;

typedef struct{
  int x, y;
  long tictac;
  bool viva;
}bomba_t;

typedef struct{
  int x, y;
  bool move[4], vivo;
  bomba_t bombas[MAX_B];
}player_t;

typedef struct{
  int x, y;
  long tempo;
  bool tipo, viva;
}explosao_t;


//Define se está ou não movento a tecla
void teclas(bool p1[], bool p2[], int keycode, bool estado);

//Move o personagem (só funciona para um player) =/
void atualiza(player_t &p, char paredes[LINHA][COLUNA]);

//Testa colisao do jogador(WIP)
bool colisao(int x, int y, char paredes[LINHA][COLUNA], int movimento);

//Player coloca uma bomba no mapa
void novaBomba(player_t &p);

//Faz a explosão da bomba
void explode(bomba_t &b, explosao_t e[200], player_t &p1, player_t &p2, char paredes[LINHA][COLUNA]);

//Testa os limites para a explosão
bool colisaoExplosao(int x, int y, char paredes[LINHA][COLUNA], char direcao, player_t &p1, player_t &p2);

//Testa as paredes de forma genérica (chamada pela colisaoExplosao)
bool testaCE(int x, int y, char paredes[LINHA][COLUNA], player_t &p1, player_t &p2);

//Cria uma nova animação de explosão
void novaExpl(int x, int y, explosao_t e[200]);
