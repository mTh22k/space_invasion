#ifndef ENEMY_H
#define ENEMY_H

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
#define MAX_ENEMIES 4
#define MAX_SHOOTING_ENEMIES 1
#define FIRE_INTERVAL 0.2
#define INVULNERABILITY_TIME 1.5
#define BOSS_SHOT_INTERVAL 0.4
#define TIME_TO_BOSS 12
#define SCROLL_SPEED 60
#define EXPLOSION_FRAME_COUNT 5

void init_enemies(Enemy enemies[], int count);
void move_enemies(Enemy enemies[], int count, int game_phase);

void init_shooting_enemies(ShootingEnemy *enemy, int game_phase);
void move_shooting_enemy(ShootingEnemy *enemy);
void shoot_enemy_bullet(ShootingEnemy *enemy, float player_x, float player_y, int game_phase);
void move_enemy_bullets(EnemyBullet bullets[], int count);
void init_enemy_bullets(Bullet enemy_bullets[], int count);

// Gera inimigos básicos
void generate_enemy(Enemy enemies[], int count, int player_width, int player_height);

// Gera inimigos que atiram
void generate_shooting_enemy(ShootingEnemy enemies[], int count, int game_phase);

#endif // ENEMY_H
