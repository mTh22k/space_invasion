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

// inicia o boss
void init_boss(Boss *boss);

// movimentação do boss
void move_boss(Boss *boss, int game_phase);


// Dispara uma bala normal do chefe. A bala é disparada com um intervalo específico
void shoot_boss_bullet(Boss *boss, BossBullet boss_bullets[], int *boss_bullet_count, int game_phase);

// Dispara o primeiro tipo de ataque especial do chefe
void shoot_boss_special_attack(Boss *boss, BossBullet boss_bullets[], int *boss_bullet_count);

// Dispara o segundo tipo de ataque especial do chefe
void shoot_boss_special_attack_2(Boss *boss, BossBullet boss_bullets[], int *boss_bullet_count);

// Verifica e ativa o chefe após o tempo de espera. 
void check_and_activate_boss(Boss *boss, double *boss_start_time, double current_time, 
                             double *boss_shoot_start_time, bool *boss_waiting, int remaining_time, 
                             int screen_width);

#endif // BOSS_H
