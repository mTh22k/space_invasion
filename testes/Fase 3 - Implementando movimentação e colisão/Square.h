#ifndef __SQUARE__ 																												//Guardas de inclusão
#define __SQUARE__																												//Guardas de inclusão																										//Quantidade de pixels que um quadrado se move por passo

#include "Joystick.h"																											//Estrutura e procedimentos relacionados ao controle do quadrado (!)

#define SQUARE_STEP 50																											//Tamanho, em pixels, de um passo do quadrado

typedef struct
{
	unsigned char width;  // Largura do retângulo
	unsigned char height; // Altura do retângulo
	unsigned short x;	  // Posição X do centro do retângulo
	unsigned short y;	  // Posição Y do centro do retângulo
	joystick *control;	  // Elemento de controle do retângulo no jogo (!)
} rectangle;
// Definição do nome da estrutura

rectangle *rectangle_create(unsigned short width, unsigned short height, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y);
void rectangle_move(rectangle *element, char steps, unsigned char trajectory, unsigned short max_x, unsigned short max_y);
void rectangle_destroy(rectangle *element);
// Protótipo da função de destruição de um quadrado

#endif																															//Guardas de inclusão