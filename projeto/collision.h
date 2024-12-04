#ifndef COLLISION_H
#define COLLISION_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include <time.h>
#include<stdio.h>

#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "collision.h"
#include "boss.h"
#include "structs.h"
#include "defines.h"


// Verifica colisões entre as balas do chefe e o jogador
void check_boss_bullet_collisions(Player *player, Boss *boss, BossBullet boss_bullets[], int *game_over, int game_phase);

// Verifica colisão entre o chefe e as balas do jogador
void check_boss_collision(Player *player, Bullet bullets[], int bullet_count, Boss *boss, int *score, int *player_won, int *game_over, int game_phase);

// Verifica colisões entre as balas do inimigo que atira e o jogador
void check_enemy_bullet_collisions(Player *player, ShootingEnemy *enemy, int *game_over);

// Verifica todas as colisões no jogo
void check_collisions(Player *player, Bullet bullets[], int bullet_count, Enemy enemies[], int enemy_count, ShootingEnemy shooting_enemies[], int shooting_enemy_count, Item *item_phase1, Item *item_phase2, int *score, int *game_over, int *enemy_destroyed_count, int game_phase);

#endif // COLLISION_H

