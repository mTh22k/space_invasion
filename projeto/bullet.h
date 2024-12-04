#ifndef BULLET_H
#define BULLET_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "collision.h"
#include "boss.h"
#include "structs.h"
#include "defines.h"

// Inicializa as balas do jogador
void init_bullets(Bullet bullets[], int count);

// Dispara uma nova bala a partir da posição (x, y)
void fire_bullet(Bullet bullets[], int count, float x, float y);

// Move as balas do jogador
void move_bullets(Bullet bullets[], int count, Player *player, ALLEGRO_FONT *font, int game_phase);

// Inicializa as balas do boss
void init_boss_bullets(BossBullet boss_bullets[], int count);

// Move as balas disparadas pelo boss
void move_boss_bullets(BossBullet boss_bullets[], int count, Player *player);

// Dispara as balas do inimigo
void shoot_enemy_bullet(ShootingEnemy *enemy, float player_x, float player_y, int game_phase);

// Move as balas disparadas pelos inimigos
void move_enemy_bullets(EnemyBullet bullets[], int count);

// Inicializa as balas disparadas pelos inimigos
void init_enemy_bullets(Bullet enemy_bullets[], int count);



#endif // BULLET_H
