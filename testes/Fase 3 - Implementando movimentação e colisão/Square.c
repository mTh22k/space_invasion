#include <stdlib.h>
#include "Square.h"

rectangle *rectangle_create(unsigned short width, unsigned short height, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y)
{
	// Calcula os limites do retângulo
	unsigned short half_width = width / 2;
	unsigned short half_height = height / 2;

	// Verifica se a posição inicial é válida
	if ((x < half_width) || (x + half_width > max_x) ||
		(y < half_height) || (y + half_height > max_y))
	{
		return NULL;
	}

	rectangle *new_rectangle = (rectangle *)malloc(sizeof(rectangle));
	if (new_rectangle == NULL)
	{
		// Tratar erro de alocação
		return NULL;
	}

	new_rectangle->width = width;
	new_rectangle->height = height;
	new_rectangle->x = x;
	new_rectangle->y = y;
	new_rectangle->control = joystick_create();
	return new_rectangle;
}

void rectangle_move(rectangle *element, char steps, unsigned char trajectory, unsigned short max_x, unsigned short max_y)
{
	if (!trajectory)
	{
		if ((element->x - steps * SQUARE_STEP) - element->width / 2 >= 0)
			element->x -= steps * SQUARE_STEP;
	}
	else if (trajectory == 1)
	{
		if ((element->x + steps * SQUARE_STEP) + element->width / 2 <= max_x)
			element->x += steps * SQUARE_STEP;
	}
	else if (trajectory == 2)
	{
		if ((element->y - steps * SQUARE_STEP) - element->height / 2 >= 0)
			element->y -= steps * SQUARE_STEP;
	}
	else if (trajectory == 3)
	{
		if ((element->y + steps * SQUARE_STEP) + element->height / 2 <= max_y)
			element->y += steps * SQUARE_STEP;
	}
}

void rectangle_destroy(rectangle *element)
{
	joystick_destroy(element->control); // Destrói controle do retângulo
	free(element);						// Libera a memória do retângulo na heap
}
