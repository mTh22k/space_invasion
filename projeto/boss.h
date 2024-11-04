#ifndef BOSS_H
#define BOSS_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include <time.h>

#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "collision.h"
#include "boss.h"
#include "structs.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define PLAYER_SPEED 6
#define BULLET_SPEED 6
#define ENEMY_SPEED 5
#define ENEMY_SPEED_SHOOTING 3
#define MAX_BULLETS 8
#define MAX_BULLET_COUNT 7
#define MAX_BOSS_BULLETS 8
#define MAX_ENEMIES 6
#define MAX_SHOOTING_ENEMIES 2
#define FIRE_INTERVAL 0.2
#define INVULNERABILITY_TIME 1.5
#define BOSS_SHOT_INTERVAL 0.4
#define TIME_TO_BOSS 5
#define SCROLL_SPEED 60
#define EXPLOSION_FRAME_COUNT 6

void init_boss(Boss *boss);

void move_boss(Boss *boss);

// Inicializa as balas do chefe
void init_boss_bullets(BossBullet boss_bullets[], int count);

// Dispara uma bala do chefe
void shoot_boss_bullet(Boss *boss, BossBullet boss_bullets[], int *boss_bullet_count);

// Move as balas do chefe
void move_boss_bullets(BossBullet boss_bullets[], int count);

// Verifica colisões entre as balas do chefe e o jogador
void check_boss_bullet_collisions(Player *player, BossBullet boss_bullets[], int *game_over);

#endif // BOSS_H
