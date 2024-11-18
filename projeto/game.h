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

void init_allegro();

void cleanup(ALLEGRO_BITMAP *background, ALLEGRO_BITMAP *player_sprite, ALLEGRO_BITMAP *bullet_sprite,
             ALLEGRO_BITMAP *enemy_sprite, ALLEGRO_BITMAP *shooting_enemy_sprite, ALLEGRO_BITMAP *enemy_bullet_sprite,
             ALLEGRO_BITMAP *boss_sprite, ALLEGRO_BITMAP *boss_bullet_sprite, ALLEGRO_EVENT_QUEUE *event_queue,
             ALLEGRO_TIMER *timer, ALLEGRO_DISPLAY *display, ALLEGRO_FONT *font, ALLEGRO_BITMAP *explosion_bitmaps[]);

void draw_pause_message(ALLEGRO_FONT *font, ALLEGRO_DISPLAY *display);
void draw_player_life(ALLEGRO_FONT *font, Player *player);
void draw_timer(ALLEGRO_FONT *font, double elapsed_time);
void play_explosion(int x, int y, ALLEGRO_BITMAP *explosion_bitmaps[], ALLEGRO_BITMAP *background);

extern ALLEGRO_FONT *menu_font;
extern ALLEGRO_BITMAP *menu_background;

void draw_menu();
void init_menu_resources();
void cleanup_menu_resources();
void show_game_over_menu(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font, int *restart_game, int *exit_game);
void show_transition_menu(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font, int *continue_game, int *exit_game);
void init_second_phase(Player *player, Enemy enemies[], Bullet bullets[], ShootingEnemy shooting_enemies[], Boss *boss, int *victory_state, int *player_won, double *start_time, int game_phase);
void show_victory_message(ALLEGRO_FONT * font);

#endif // ENEMY_H