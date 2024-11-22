#ifndef COLLISION_H
#define COLLISION_H

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
#define TIME_TO_BOSS 15
#define SCROLL_SPEED 60
#define EXPLOSION_FRAME_COUNT 6

void check_boss_collision(Player *player, Bullet bullets[], int bullet_count, Boss *boss, int *score, int *player_won, int *game_over);
void check_shooting_enemy_collision(Player *player, ShootingEnemy *enemy, int *game_over);
void check_enemy_bullet_collisions(Player *player, ShootingEnemy *enemy, int *game_over);
void check_collisions(Player *player, Bullet bullets[], int bullet_count, Enemy enemies[], int enemy_count, ShootingEnemy shooting_enemies[], int shooting_enemy_count, Item *item, int *score, int *game_over);

#endif // COLLISION_H
