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

void init_enemies(Enemy enemies[], int count);
void move_enemies(Enemy enemies[], int count, int game_phase);

void init_shooting_enemies(ShootingEnemy *enemy, int game_phase);
void move_shooting_enemy(ShootingEnemy *enemy);

// Gera inimigos básicos
void generate_enemy(Enemy enemies[], int count, int player_width, int player_height);

// Gera inimigos que atiram
void generate_shooting_enemy(ShootingEnemy enemies[], int count, int game_phase);

#endif // ENEMY_H
