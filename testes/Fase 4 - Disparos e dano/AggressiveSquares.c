//Compilação: gcc AggressiveSquares.c Square.c Joystick.c Bullet.c Pistol.c -o AS $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 --libs --cflags)

#include <allegro5/allegro5.h>																																												//Biblioteca base do Allegro
#include <allegro5/allegro_font.h>																																											//Biblioteca de fontes do Allegro
#include <allegro5/allegro_primitives.h>																																									//Biblioteca de figuras básicas

#include "Square.h"																																															//Inclusão da biblioteca de quadrados

#define X_SCREEN 1080																																														//Definição do tamanho da tela em pixels no eixo x
#define Y_SCREEN 620																																														//Definição do tamanho da tela em pixels no eixo y

unsigned char collision_2D(square *element_first, square *element_second){																																	//Implementação da função de verificação de colisão entre dois quadrados

	if ((((element_second->y-element_second->side/2 >= element_first->y-element_first->side/2) && (element_first->y+element_first->side/2 >= element_second->y-element_second->side/2)) || 	//				//Verifica se o primeiro elemento colidiu com o segundo no eixo X 
		((element_first->y-element_first->side/2 >= element_second->y-element_second->side/2) && (element_second->y+element_second->side/2 >= element_first->y-element_first->side/2))) && 	//				//Verifica se o segundo elemento colidiu com o primeiro no eixo X 
		(((element_second->x-element_second->side/2 >= element_first->x-element_first->side/2) && (element_first->x+element_first->side/2 >= element_second->x-element_second->side/2)) || 	//				//Verifica se o primeiro elemento colidiu com o segundo no eixo Y
		((element_first->x-element_first->side/2 >= element_second->x-element_second->side/2) && (element_second->x+element_second->side/2 >= element_first->x-element_first->side/2)))) return 1;			//Verifica se o segundo elemento colidiu com o primeiro no eixo Y
	else return 0;																																															//Se as condições não forem satisfeita, não houve colisão
}

unsigned char check_kill(square *killer, square *victim){																																					//Implementação da função que verifica se um projétil acertou um jogador (!)

	for (bullet *index = killer->gun->shots; index != NULL; index = (bullet*) index->next){																													//Para todos os projéteis do atirador (!)
		if ((index->x >= victim->x - victim->side/2) && (index->x <= victim->x + victim->side/2) && //																										//Verique se houve colisão com a vítima no eixo X (!)
		   (index->y >= victim->y - victim->side/2) && (index->y <= victim->y + victim->side/2)){																											//Verifique se houve colisão com a vítima no eixo Y (!)
			return 1;																																														//Se sim, retorne que o quadrado vítima foi atingido
		}
	}
	return 0;																																																//Se não houver nenhum projétil que acertou a vítima, retorne falso (!)
}

void update_bullets(square *player){																																										//Implementação da função que atualiza o posicionamento de projéteis conforme o movimento dos mesmos (!)
	
	bullet *previous = NULL;																																												//Variável auxiliar para salvar a posição imediatamente anterior na fila (!)
	for (bullet *index = player->gun->shots; index != NULL;){																																				//Para cada projétil presente na lista de projéteis disparados (!)
		if (!index->trajectory) index->x -= BULLET_MOVE;																																					//Se a trajetória for para a esquerda, atualiza a posição para a esquerda (!)
		else if (index->trajectory == 1) index->x += BULLET_MOVE;																																			//Se a trajetória for para a direita, atualiza a posição para a esquerda (!)
		
		if ((index->x < 0) || (index->x > X_SCREEN)){																																						//Verifica se o projétil saiu das bordas da janela (!)
			if (previous){																																													//Verifica se não é o primeiro elemento da lista de projéteis (!)
				previous->next = index->next;																																								//Se não for, salva o próximo projétil (!)
				bullet_destroy(index);																																										//Chama o destrutor para o projétil atual (!)
				index = (bullet*) previous->next;																																							//Atualiza para o próximo projétil (!)
			}
			else {																																															//Se for o primeiro projétil da lista (!)
				player->gun->shots = (bullet*) index->next;																																					//Atualiza o projétil no início da lista (!)
				bullet_destroy(index);																																										//Chama o destrutor para o projétil atual (!)
				index = player->gun->shots;																																									//Atualiza para o próximo projétil (!)
			}
		}
		else{																																																//Se não saiu da tela (!)
			previous = index;																																												//Atualiza o projétil anterior (para a próxima iteração) (!)
			index = (bullet*) index->next;																																									//Atualiza para o próximo projétil (!)
		}
	}
}

void update_position(square *player_1, square *player_2){																																					//Função de atualização das posições dos quadrados conforme os comandos do controle

	if (player_1->control->left){																																											//Se o botão de movimentação para esquerda do controle do primeiro jogador está ativado...
		square_move(player_1, 1, 0, X_SCREEN, Y_SCREEN);																																					//Move o quadrado do primeiro jogador para a esquerda
		if (collision_2D(player_1, player_2)) square_move(player_1, -1, 0, X_SCREEN, Y_SCREEN);																												//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo
	}
	if (player_1->control->right){																																											//Se o botão de movimentação para direita do controle do primeir ojogador está ativado...
		square_move(player_1, 1, 1, X_SCREEN, Y_SCREEN);																																					//Move o quadrado do primeiro jogador para a direta
		if (collision_2D(player_1, player_2)) square_move(player_1, -1, 1, X_SCREEN, Y_SCREEN);																												//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo
	}
	if (player_1->control->up) {																																											//Se o botão de movimentação para cima do controle do primeiro jogador está ativado...
		square_move(player_1, 1, 2, X_SCREEN, Y_SCREEN);																																					//Move o quadrado do primeiro jogador para cima
		if (collision_2D(player_1, player_2)) square_move(player_1, -1, 2, X_SCREEN, Y_SCREEN);																												//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo
	}
	if (player_1->control->down){																																											//Se o botão de movimentação para baixo do controle do primeiro jogador está ativado...
		square_move(player_1, 1, 3, X_SCREEN, Y_SCREEN);																																					//Move o quadrado do primeiro jogador para a baixo
		if (collision_2D(player_1, player_2)) square_move(player_1, -1, 3, X_SCREEN, Y_SCREEN);																												//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo
	}

	if (player_2->control->left){																																											//Se o botão de movimentação para esquerda do controle do segundo jogador está ativado...
		square_move(player_2, 1, 0, X_SCREEN, Y_SCREEN);																																					//Move o quadrado do segundo jogador para a esquerda
		if (collision_2D(player_2, player_1)) square_move(player_2, -1, 0, X_SCREEN, Y_SCREEN);																												//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo
	}
	
	if (player_2->control->right){ 																																											//Se o botão de movimentação para direita do controle do segundo jogador está ativado...
		square_move(player_2, 1, 1, X_SCREEN, Y_SCREEN);																																					//Move o quadrado do segundo jogador para a direita
		if (collision_2D(player_2, player_1)) square_move(player_2, -1, 1, X_SCREEN, Y_SCREEN);																												//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo
	}
	
	if (player_2->control->up){																																												//Se o botão de movimentação para cima do controle do segundo jogador está ativado...
		square_move(player_2, 1, 2, X_SCREEN, Y_SCREEN);																																					//Move o quadrado do segundo jogador para a cima
		if (collision_2D(player_2, player_1)) square_move(player_2, -1, 2, X_SCREEN, Y_SCREEN);																												//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo			
	}
	if (player_2->control->down){																																											//Se o botão de movimentação para baixo do controle do segundo jogador está ativado...
		square_move(player_2, 1, 3, X_SCREEN, Y_SCREEN);																																					//Move o quadrado do segundo jogador para a baixo
		if (collision_2D(player_2, player_1)) square_move(player_2, -1, 3, X_SCREEN, Y_SCREEN);																												//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo
	}
	if (player_1->control->fire){																																											//Verifica se o primeiro jogador está atirando (!)
		if (!player_1->gun->timer){																																											//Verifica se a arma do primeiro jogador não está em cooldown (!)
			square_shot(player_1); 																																											//Se não estiver, faz um disparo (!)
			player_1->gun->timer = PISTOL_COOLDOWN;																																							//Inicia o cooldown da arma (!)
		} 
	}
	if (player_2->control->fire){																																											//Verifica se o segundo jogador está atirando (!)
		if (!player_2->gun->timer){																																											//Verifica se a arma do segundo jogador não está em cooldown (!)
			square_shot(player_2);																																											//Se não estiver, faz um disparo (!)
			player_2->gun->timer = PISTOL_COOLDOWN;																																							//Inicia o cooldown da arma (!)
		}
	}
	update_bullets(player_1);																																												//Atualiza os disparos do primeiro jogador (!)
	update_bullets(player_2);																																												//Atualiza os disparos do segundo jogador (!)
}

int main(){
	
	al_init();																																																//Faz a preparação de requisitos da biblioteca Allegro
	al_init_primitives_addon();																																												//Faz a inicialização dos addons das imagens básicas
	
	al_install_keyboard();																																													//Habilita a entrada via teclado (eventos de teclado), no programa

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);																																						//Cria o relógio do jogo; isso indica quantas atualizações serão realizadas por segundo (30, neste caso)
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();																																					//Cria a fila de eventos; todos os eventos (programação orientada a eventos) 
	ALLEGRO_FONT* font = al_create_builtin_font();																																							//Carrega uma fonte padrão para escrever na tela (é bitmap, mas também suporta adicionar fontes ttf)
	ALLEGRO_DISPLAY* disp = al_create_display(X_SCREEN, Y_SCREEN);																																			//Cria uma janela para o programa, define a largura (x) e a altura (y) da tela em píxeis (320x320, neste caso)

	al_register_event_source(queue, al_get_keyboard_event_source());																																		//Indica que eventos de teclado serão inseridos na nossa fila de eventos
	al_register_event_source(queue, al_get_display_event_source(disp));																																		//Indica que eventos de tela serão inseridos na nossa fila de eventos
	al_register_event_source(queue, al_get_timer_event_source(timer));																																		//Indica que eventos de relógio serão inseridos na nossa fila de eventos

	square* player_1 = square_create(20, 1, 10, Y_SCREEN/2, X_SCREEN, Y_SCREEN);																															//Cria o quadrado do primeiro jogador
	if (!player_1) return 1;																																												//Verificação de erro na criação do quadrado do primeiro jogador
	square* player_2 = square_create(20, 0, X_SCREEN-10, Y_SCREEN/2, X_SCREEN, Y_SCREEN);																													//Cria o quadrado do segundo jogador
	if (!player_2) return 2;																																												//Verificação de erro na criação do quadrado do segundo jogador

	ALLEGRO_EVENT event;																																													//Variável que guarda um evento capturado, sua estrutura é definida em: https:		//www.allegro.cc/manual/5/ALLEGRO_EVENT
	al_start_timer(timer);																																													//Função que inicializa o relógio do programa
	unsigned char p1k = 0, p2k = 0;																																											//Variáveis de controle de vida dos quadrados (jogadores) (!)
	while(1){																																																//Laço servidor do jogo
		al_wait_for_event(queue, &event);																																									//Função que captura eventos da fila, inserindo os mesmos na variável de eventos

		if (p1k || p2k){																																													//Verifica se algum jogador foi morto 
			al_clear_to_color(al_map_rgb(0, 0, 0));																																							//Limpe a tela atual para um fundo preto
			if (p2k && p1k) al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2 - 40, Y_SCREEN/2-15, 0, "EMPATE!");																					//Se ambos foram mortos, declare um empate
			else if (p2k) al_draw_text(font, al_map_rgb(255, 0, 0), X_SCREEN/2 - 75, Y_SCREEN/2-15, 0, "JOGADOR 1 GANHOU!");																				//Se o segundo jogador morreu, declare o primeiro jogador vencedor
			else if (p1k) al_draw_text(font, al_map_rgb(0, 0, 255), X_SCREEN/2 - 75, Y_SCREEN/2-15, 0, "JOGADOR 2 GANHOU!");																				//Se o primeiro jogador morreu, declare o segundo jogador vencedor
			al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2 - 110, Y_SCREEN/2+5, 0, "PRESSIONE ESPAÇO PARA SAIR");																					//Indique o modo de conclusão do programa
			al_flip_display();																																												//Atualiza a tela

			if ((event.type == 10) && (event.keyboard.keycode == 75)) break;																																//Espera por um evento de teclado, de clique da tecla de espaço
			else if (event.type == 42) break; 																																								//Finaliza o jogo
		}
		else{																																																
			if (event.type == 30){																																											
				update_position(player_1, player_2);																																						
				p1k = check_kill(player_2, player_1);																																						
				p2k = check_kill(player_1, player_2);																																						
				al_clear_to_color(al_map_rgb(0, 0, 0));																																						
				al_draw_filled_rectangle(player_1->x-player_1->side/2, player_1->y-player_1->side/2, player_1->x+player_1->side/2, player_1->y+player_1->side/2, al_map_rgb(255, 0, 0));					
				al_draw_filled_rectangle(player_2->x-player_2->side/2, player_2->y-player_2->side/2, player_2->x+player_2->side/2, player_2->y+player_2->side/2, al_map_rgb(0, 0, 255));					
	    		for (bullet *index = player_1->gun->shots; index != NULL; index = (bullet*) index->next) al_draw_filled_circle(index->x, index->y, 2, al_map_rgb(255, 0, 0));								
	    		for (bullet *index = player_2->gun->shots; index != NULL; index = (bullet*) index->next) al_draw_filled_circle(index->x, index->y, 2, al_map_rgb(0, 0, 255));								
	    		if (player_2->gun->timer) player_2->gun->timer--; 																																			
	    		al_flip_display();																																											
			}
			else if ((event.type == 10) || (event.type == 12)){																																				
				if (event.keyboard.keycode == 1) joystick_left(player_1->control);																															
				else if (event.keyboard.keycode == 4) joystick_right(player_1->control);																													
				else if (event.keyboard.keycode == 23) joystick_up(player_1->control);																														
				else if (event.keyboard.keycode == 19) joystick_down(player_1->control);																													
				else if (event.keyboard.keycode == 82) joystick_left(player_2->control);																													
				else if (event.keyboard.keycode == 83) joystick_right(player_2->control);																													
				else if (event.keyboard.keycode == 84) joystick_up(player_2->control);																														
				else if (event.keyboard.keycode == 85) joystick_down(player_2->control);																													
				else if (event.keyboard.keycode == 3) joystick_fire(player_1->control);																																	
				else if (event.keyboard.keycode == 13) joystick_fire(player_2->control);																													
			}																																			
			else if (event.type == 42) break;																																								
		}
	}

	al_destroy_font(font);																																													
	al_destroy_display(disp);																																												
	al_destroy_timer(timer);																																												
	al_destroy_event_queue(queue);																																											
	square_destroy(player_1);																																												
	square_destroy(player_2);																							

	return 0;
}