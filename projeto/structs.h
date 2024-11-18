#ifndef STRUCTS_H
#define STRUCTS_H

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

typedef struct
{
    unsigned char right, left, up, down, fire;
} Joystick;

typedef struct
{
    float x, y;
    int width, height;
    int lives;
    int invulnerable;
    double invulnerable_time;
    Joystick joystick;
    int paused;
} Player;

typedef struct
{
    float x, y;
    int width, height;
    int active;
} Bullet;

typedef struct
{
    float x, y;
    int width, height;
    int active;
    int health;
    float vertical_speed;
    float original_y;
    int moving_up;
} Enemy;

typedef struct
{
    float x, y;
    int width, height;
    int active;
    float speed;
} EnemyBullet;

typedef struct
{
    float x, y;
    int width, height;
    int active;
    float speed;
} BossBullet;

typedef struct
{
    float x, y;
    int width, height;
    int active;
    int health;
    float vertical_speed;
    EnemyBullet bullets[10];
    double last_shot_time;
    int moving_up;
} ShootingEnemy;

typedef struct
{
    float x, y;
    int width, height;
    int active;
    float speed;
    int health;
    int horizontal_speed;
    double last_shot_time;
} Boss;

#endif // STRUCTS_H
