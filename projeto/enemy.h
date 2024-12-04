#ifndef ENEMY_H
#define ENEMY_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "collision.h"
#include "boss.h"
#include "structs.h"
#include "defines.h"

// Função para inicializar um array de inimigos básicos
void init_enemies(Enemy enemies[], int count);

// Função para atualizar o movimento dos inimigos básicos com base na fase do jogo
void move_enemies(Enemy enemies[], int count, int game_phase);

// Função para inicializar inimigos que podem atirar
void init_shooting_enemies(ShootingEnemy *enemy, int game_phase);

// Função para atualizar o movimento de um inimigo que atira
void move_shooting_enemy(ShootingEnemy *enemy);

// Função para gerar um inimigo básico em uma posição válida na tela
void generate_enemy(Enemy enemies[], int count, int player_width, int player_height);

// Função para gerar inimigos que atiram, configurados de acordo com a fase do jogo
void generate_shooting_enemy(ShootingEnemy enemies[], int count, int game_phase);

#endif // ENEMY_H
