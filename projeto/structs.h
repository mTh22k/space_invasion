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
#include "defines.h"

// Estrutura que armazena o estado dos botões do joystick
typedef struct
{
    unsigned char right, left, up, down, fire; // Direções e botão de tiro
} Joystick;

// Estrutura que define as opções do jogo, como a nave selecionada e grupos de opções
typedef struct
{
    int sprite_option;    // 0, 1 ou 2 (tipos de nave ou opções relacionadas)
    int group_1_selected; // 1 para marcar o grupo 1, 0 para desmarcar

    int new_option_1;     // 0, 1 ou 2 (opção 1 do novo grupo)
    int group_2_selected; // 1 para marcar o grupo 2, 0 para desmarcar

    int new_option_2;     // Opção 2 do novo grupo
    int group_3_selected; // 1 para marcar o grupo 3, 0 para desmarcar

    int new_option_3;     // Opção 3 do novo grupo
    int group_4_selected; // 1 para marcar o grupo 4, 0 para desmarcar
} GameOptions;

// Estrutura que representa o jogador, com informações como posição, vidas e joystick
typedef struct
{
    int x, y;                    // Posição do jogador
    int width, height;           // Tamanho do jogador
    int lives;                   // Quantidade de vidas
    int invulnerable;            // Flag de invulnerabilidade
    double invulnerable_time;    // Tempo em que começou a invulnerabilidade
    Joystick joystick;           // Joystick do jogador
    int paused;                  // Flag indicando se o jogo está pausado
    bool special_attack_active;  // Flag de ataque especial ativo
    float special_attack_start_time; // Tempo de início do ataque especial
    float speed_multiplier;      // Multiplicador de velocidade
    double slow_effect_end_time; // Tempo de término do efeito de lentidão
} Player;

// Estrutura que representa uma bala, com sua posição, tamanho e estado ativo
typedef struct
{
    float x, y;   // Posição da bala
    int width, height; // Tamanho da bala
    int active;   // Flag indicando se a bala está ativa
} Bullet;

// Estrutura que representa um inimigo, com informações sobre sua posição, saúde e movimento
typedef struct
{
    float x, y;               // Posição do inimigo
    int width, height;        // Tamanho do inimigo
    int active;               // Flag indicando se o inimigo está ativo
    int health;               // Saúde do inimigo
    float vertical_speed;     // Velocidade de movimento vertical
    float original_y;         // Posição Y original para movimentação
    int moving_up;            // Flag indicando se o inimigo está subindo
    int damaged;              // Flag indicando se o inimigo está piscando por dano
    double damaged_time;      // Tempo em que o inimigo começou a piscar
    double explosion_time;    // Tempo para explosão do inimigo
    int exploding;            // Flag de explosão
} Enemy;

// Estrutura que representa uma bala disparada por um inimigo
typedef struct
{
    float x, y;   // Posição da bala
    int width, height; // Tamanho da bala
    int active;   // Flag indicando se a bala está ativa
    float speed;  // Velocidade da bala
} EnemyBullet;

// Estrutura que representa uma bala disparada por um boss, com suas propriedades
typedef struct
{
    float x, y;               // Posição da bala
    int width, height;        // Tamanho da bala
    int active;               // Flag indicando se a bala está ativa
    float speed;              // Velocidade da bala
    double vx, vy;            // Velocidade nas direções X e Y
    double spawn_time;        // Tempo de spawn da bala
} BossBullet;

// Estrutura que representa um inimigo que atira, com suas balas e propriedades
typedef struct
{
    float x, y;               // Posição do inimigo
    int width, height;        // Tamanho do inimigo
    int active;               // Flag indicando se o inimigo está ativo
    int health;               // Saúde do inimigo
    float vertical_speed;     // Velocidade de movimento vertical
    EnemyBullet bullets[10];  // Balas disparadas pelo inimigo
    double last_shot_time;    // Tempo do último disparo
    int moving_up;            // Flag indicando se o inimigo está subindo
    int damaged;              // Flag indicando se o inimigo está piscando por dano
    double damaged_time;      // Tempo em que o inimigo começou a piscar
    double explosion_time;    // Tempo de explosão
    int exploding;            // Flag de explosão
    bool ready_to_shoot;      // Flag indicando se o inimigo está pronto para disparar
} ShootingEnemy;

// Estrutura que representa um boss, com suas balas, ataques especiais e propriedades
typedef struct
{
    float x, y;                       // Posição do boss
    int width, height;                 // Tamanho do boss
    int active;                        // Flag indicando se o boss está ativo
    float speed;                       // Velocidade de movimento do boss
    int health;                        // Saúde do boss
    int horizontal_speed;              // Velocidade de movimento horizontal
    double last_shot_time;             // Tempo do último disparo do boss
    int damaged;                       // Flag indicando se o boss está piscando por dano
    double damaged_time;               // Tempo em que o boss começou a piscar
    double last_special_attack_time;   // Tempo do último ataque especial
    int special_attack_active;         // Flag indicando se o ataque especial está ativo
    double explosion_time;             // Tempo de explosão do boss
    int exploding;                     // Flag de explosão do boss
} Boss;

// Estrutura que representa um item do jogo, com suas propriedades
typedef struct
{
    int x, y;                 // Posição do item
    bool active;              // Flag indicando se o item está ativo
    ALLEGRO_BITMAP *sprite;   // Ponteiro para o sprite do item
    int width, height;        // Tamanho do item
} Item;

#endif // STRUCTS_H
