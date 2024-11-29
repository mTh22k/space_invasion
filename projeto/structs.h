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
#define TIME_TO_BOSS 12
#define SCROLL_SPEED 60
#define EXPLOSION_FRAME_COUNT 5

typedef struct
{
    unsigned char right, left, up, down, fire;
} Joystick;

typedef struct
{
    int sprite_option;    // 0, 1 ou 2 (tipos de nave ou opção relacionada)
    int group_1_selected; // 1 para marcar o grupo 1, 0 para desmarcar

        int new_option_1;     // 0, 1 ou 2 (opção 1 do novo grupo)
    int group_2_selected; // 1 para marcar o grupo 2, 0 para desmarcar

    int new_option_2;
    int group_3_selected;

    int new_option_3;
    int group_4_selected;
} GameOptions;

typedef struct
{
    float x, y;
    int width, height;
    int lives;
    int invulnerable;
    double invulnerable_time;
    Joystick joystick;
    int paused;
    bool special_attack_active;
    float special_attack_start_time;
    float speed_multiplier; // Multiplicador de velocidade
    double slow_effect_end_time;
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
    int damaged;         // Flag indicando se está piscando
    double damaged_time; // Tempo em que começou o piscamento
    double explosion_time;
    int exploding;
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
    double vx, vy;
    double spawn_time;
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
    int damaged;         // Flag indicando se está piscando
    double damaged_time; // Tempo em que começou o piscamento
    double explosion_time;
    int exploding;
    bool ready_to_shoot;
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
    int damaged;                     // Flag indicando se está piscando
    double damaged_time;             // Tempo em que começou o piscamento
    double last_special_attack_time; // Tempo do último ataque especial
    int special_attack_active;       // Indica se o ataque especial está ativo
    double explosion_time;
    int exploding;
} Boss;

typedef struct
{
    int x, y;
    bool active;
    ALLEGRO_BITMAP *sprite; // Ponteiro para o sprite
} Item;

#endif // STRUCTS_H
