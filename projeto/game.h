#ifndef GAME_H
#define GMAE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "collision.h"
#include "boss.h"
#include "structs.h"

void init_allegro();

void cleanup(ALLEGRO_BITMAP *background, ALLEGRO_BITMAP *player_sprite, ALLEGRO_BITMAP *bullet_sprite,
             ALLEGRO_BITMAP *enemy_sprite, ALLEGRO_BITMAP *shooting_enemy_sprite, ALLEGRO_BITMAP *enemy_bullet_sprite,
             ALLEGRO_BITMAP *boss_sprite, ALLEGRO_BITMAP *boss_bullet_sprite, ALLEGRO_EVENT_QUEUE *event_queue,
             ALLEGRO_TIMER *timer, ALLEGRO_DISPLAY *display, ALLEGRO_FONT *font, ALLEGRO_BITMAP *explosion_bitmaps[]);

#endif // ENEMY_H