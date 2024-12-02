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
#include "defines.h"
 // Substitua pelo valor correto da altura da bala

void init_boss(Boss *boss);

void move_boss(Boss *boss, int game_phase);

// Inicializa as balas do chefe

// Dispara uma bala do chefe

void shoot_boss_bullet(Boss *boss, BossBullet boss_bullets[], int *boss_bullet_count, int game_phase);
void shoot_boss_special_attack_2(Boss *boss, BossBullet boss_bullets[], int *boss_bullet_count);
void shoot_boss_special_attack(Boss *boss, BossBullet boss_bullets[], int *boss_bullet_count);

#endif // BOSS_H
