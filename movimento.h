#define tUP 0
#define tDOWN 1
#define tLEFT 2
#define tRIGHT 3
#define SPEED 10

//Define se está ou não movento a tecla
void teclas(bool move[], int keycode, bool estado);

//Move o personagem
void atualiza(bool move[], int &x, int &y);
