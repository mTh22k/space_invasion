//Compilação: gcc Aggressiverectangles.c rectangle.c -o AS $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 --libs --cflags)

#include <allegro5/allegro5.h>																																											//Biblioteca base do Allegro
#include <allegro5/allegro_font.h>																																										//Biblioteca de fontes do Allegro
#include <allegro5/allegro_primitives.h>																																								//Biblioteca de figuras básicas
#include <allegro5/allegro_image.h>

#include "Square.h"

#include <stdio.h>																																														//Inclusão da biblioteca de quadrados

#define X_SCREEN 1080																																													//Definição do tamanho da tela em pixels no eixo x
#define Y_SCREEN 620																																													//Definição do tamanho da tela em pixels no eixo y

unsigned char collision_2D(rectangle *element_first, rectangle *element_second)
{
	if ((((element_first->y + element_first->height / 2 >= element_second->y - element_second->height / 2) &&
		  (element_second->y - element_second->height / 2 >= element_first->y - element_first->height / 2)) ||
		 ((element_second->y + element_second->height / 2 >= element_first->y - element_first->height / 2) &&
		  (element_first->y - element_first->height / 2 >= element_second->y - element_second->height / 2))) &&
		(((element_first->x + element_first->width / 2 >= element_second->x - element_second->width / 2) &&
		  (element_second->x - element_second->width / 2 >= element_first->x - element_first->width / 2)) ||
		 ((element_second->x + element_second->width / 2 >= element_first->x - element_first->width / 2) &&
		  (element_first->x - element_first->width / 2 >= element_second->x - element_second->width / 2))))
		return 1;
	else
		return 0;
}

void update_position(rectangle *player_1, rectangle *player_2)
{
	if (player_1->control->left)
	{
		rectangle_move(player_1, 1, 0, X_SCREEN, Y_SCREEN);
		if (collision_2D(player_1, player_2))
			rectangle_move(player_1, -1, 0, X_SCREEN, Y_SCREEN);
	}
	if (player_1->control->right)
	{
		rectangle_move(player_1, 1, 1, X_SCREEN, Y_SCREEN);
		if (collision_2D(player_1, player_2))
			rectangle_move(player_1, -1, 1, X_SCREEN, Y_SCREEN);
	}
	if (player_1->control->up)
	{
		rectangle_move(player_1, 1, 2, X_SCREEN, Y_SCREEN);
		if (collision_2D(player_1, player_2))
			rectangle_move(player_1, -1, 2, X_SCREEN, Y_SCREEN);
	}
	if (player_1->control->down)
	{
		rectangle_move(player_1, 1, 3, X_SCREEN, Y_SCREEN);
		if (collision_2D(player_1, player_2))
			rectangle_move(player_1, -1, 3, X_SCREEN, Y_SCREEN);
	}

	if (player_2->control->left)
	{
		rectangle_move(player_2, 1, 0, X_SCREEN, Y_SCREEN);
		if (collision_2D(player_2, player_1))
			rectangle_move(player_2, -1, 0, X_SCREEN, Y_SCREEN);
	}
	if (player_2->control->right)
	{
		rectangle_move(player_2, 1, 1, X_SCREEN, Y_SCREEN);
		if (collision_2D(player_2, player_1))
			rectangle_move(player_2, -1, 1, X_SCREEN, Y_SCREEN);
	}
	if (player_2->control->up)
	{
		rectangle_move(player_2, 1, 2, X_SCREEN, Y_SCREEN);
		if (collision_2D(player_2, player_1))
			rectangle_move(player_2, -1, 2, X_SCREEN, Y_SCREEN);
	}
	if (player_2->control->down)
	{
		rectangle_move(player_2, 1, 3, X_SCREEN, Y_SCREEN);
		if (collision_2D(player_2, player_1))
			rectangle_move(player_2, -1, 3, X_SCREEN, Y_SCREEN);
	}
}

int main(){
	al_init();																																															//Faz a preparação de requisitos da biblioteca Allegro
	al_init_primitives_addon();																																											//Faz a inicialização dos addons das imagens básicas
	al_init_image_addon();
	al_install_keyboard();																																												//Habilita a entrada via teclado (eventos de teclado), no programa

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);																																					//Cria o relógio do jogo; isso indica quantas atualizações serão realizadas por segundo (30, neste caso)
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();																																				//Cria a fila de eventos; todos os eventos (programação orientada a eventos) 
	ALLEGRO_FONT* font = al_create_builtin_font();																																						//Carrega uma fonte padrão para escrever na tela (é bitmap, mas também suporta adicionar fontes ttf)
	ALLEGRO_DISPLAY* disp = al_create_display(X_SCREEN, Y_SCREEN);																																		//Cria uma janela para o programa, define a largura (x) e a altura (y) da tela em píxeis (320x320, neste caso)

	al_register_event_source(queue, al_get_keyboard_event_source());																																	//Indica que eventos de teclado serão inseridos na nossa fila de eventos
	al_register_event_source(queue, al_get_display_event_source(disp));																																	//Indica que eventos de tela serão inseridos na nossa fila de eventos
	al_register_event_source(queue, al_get_timer_event_source(timer));																																	//Indica que eventos de relógio serão inseridos na nossa fila de eventos

	// Carrega sprites para os jogadores
	ALLEGRO_BITMAP *sprite_player_1 = al_load_bitmap("./pakkun(2).jpg");	  // Coloque o caminho da imagem do primeiro jogador
	ALLEGRO_BITMAP *sprite_player_2 = al_load_bitmap("./pakkun(2).jpg");	  // Coloque o caminho da imagem do segundo jogador

	// Verifica se os sprites foram carregados corretamente
	if (!sprite_player_1 || !sprite_player_2)
	{
		fprintf(stderr, "Erro ao carregar sprites!\n");
		return -1;
	}

	rectangle *player_1 = rectangle_create(100, 50, 100, 100, X_SCREEN, Y_SCREEN);
	if (!player_1)
	{
		fprintf(stderr, "Erro ao criar player_1: retângulo fora dos limites ou falha de alocação\n");
		return 1;
	}

	rectangle *player_2 = rectangle_create(100, 50, X_SCREEN - 100, Y_SCREEN - 100, X_SCREEN, Y_SCREEN);
	if (!player_2)
	{
		fprintf(stderr, "Erro ao criar player_2: retângulo fora dos limites ou falha de alocação\n");
		return 2;
	} // Verificação de erro na criação do quadrado do segundo jogador

	ALLEGRO_EVENT event;																																												//Variável que guarda um evento capturado, sua estrutura é definida em: https:		//www.allegro.cc/manual/5/ALLEGRO_EVENT
	al_start_timer(timer);																																												//Função que inicializa o relógio do programa
	while (1)
	{
		al_wait_for_event(queue, &event);

		if (event.type == ALLEGRO_EVENT_TIMER)
		{
			update_position(player_1, player_2);
			al_clear_to_color(al_map_rgb(0, 0, 0));

			float margin = 20.0;
			float player_1_x = player_1->x - al_get_bitmap_width(sprite_player_1) / 2;
			float player_1_y = player_1->y - al_get_bitmap_height(sprite_player_1) / 2;
			float player_2_x = player_2->x - al_get_bitmap_width(sprite_player_2) / 2;
			float player_2_y = player_2->y - al_get_bitmap_height(sprite_player_2) / 2;

			// Desenha os sprites dos jogadores com a margem
			al_draw_scaled_bitmap(
				sprite_player_1, 0, 0,
				al_get_bitmap_width(sprite_player_1), al_get_bitmap_height(sprite_player_1),
				player_1_x + margin, player_1_y + margin,
				al_get_bitmap_width(sprite_player_1) - 2 * margin,
				al_get_bitmap_height(sprite_player_1) - 2 * margin,
				0);

			al_draw_scaled_bitmap(
				sprite_player_2, 0, 0,
				al_get_bitmap_width(sprite_player_2), al_get_bitmap_height(sprite_player_2),
				player_2_x + margin, player_2_y + margin,
				al_get_bitmap_width(sprite_player_2) - 2 * margin,
				al_get_bitmap_height(sprite_player_2) - 2 * margin,
				0);

			al_flip_display();
		}
		else if (event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP)
		{
			// Verifica qual tecla foi pressionada/solta
			if (event.keyboard.keycode == ALLEGRO_KEY_A)
				joystick_left(player_1->control);
			else if (event.keyboard.keycode == ALLEGRO_KEY_D)
				joystick_right(player_1->control);
			else if (event.keyboard.keycode == ALLEGRO_KEY_W)
				joystick_up(player_1->control);
			else if (event.keyboard.keycode == ALLEGRO_KEY_S)
				joystick_down(player_1->control);
			else if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
				joystick_left(player_2->control);
			else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
				joystick_right(player_2->control);
			else if (event.keyboard.keycode == ALLEGRO_KEY_UP)
				joystick_up(player_2->control);
			else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
				joystick_down(player_2->control);
		}
		else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			break;
	}
	// Evento de clique no "X" de fechamento da tela. Encerra o programa graciosamente.

	al_destroy_font(font);																																												//Destrutor da fonte padrão
	al_destroy_display(disp);																																											//Destrutor da tela
	al_destroy_timer(timer);																																											//Destrutor do relógio
	al_destroy_event_queue(queue);																																										//Destrutor da fila

	return 0;
}