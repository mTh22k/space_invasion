#ifndef GAME_H
#define GMAE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

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
#define TIME_TO_BOSS 30
#define SCROLL_SPEED 60
#define EXPLOSION_FRAME_COUNT 5

void init_allegro();

void cleanup_resources(
    ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer,
    ALLEGRO_FONT *font, ALLEGRO_FONT *font_menu, ALLEGRO_FONT *font_warn, ALLEGRO_FONT *font_info,
    ALLEGRO_BITMAP *background, ALLEGRO_BITMAP *background_2, ALLEGRO_BITMAP *background_3,
    ALLEGRO_BITMAP *player_sprite, ALLEGRO_BITMAP *player_sprite_dir, ALLEGRO_BITMAP *player_sprite_esq,
    ALLEGRO_BITMAP *player_sprite_dif1, ALLEGRO_BITMAP *player_sprite_dir_dif1, ALLEGRO_BITMAP *player_sprite_esq_dif1,
    ALLEGRO_BITMAP *player_sprite_dif2, ALLEGRO_BITMAP *player_sprite_dir_dif2, ALLEGRO_BITMAP *player_sprite_esq_dif2,
    ALLEGRO_BITMAP *bullet_sprite, ALLEGRO_BITMAP *bullet_sprite_2, ALLEGRO_BITMAP *bullet_sprite_3,
    ALLEGRO_BITMAP *bullet_sprite_dif1, ALLEGRO_BITMAP *bullet_sprite_dif2,
    ALLEGRO_BITMAP *enemy_sprite, ALLEGRO_BITMAP *enemy_sprite_2,
    ALLEGRO_BITMAP *shooting_enemy_sprite, ALLEGRO_BITMAP *shooting_enemy_sprite_2,
    ALLEGRO_BITMAP *enemy_bullet_sprite, ALLEGRO_BITMAP *boss_sprite, ALLEGRO_BITMAP *boss_sprite_2,
    ALLEGRO_BITMAP *boss_bullet_sprite, ALLEGRO_BITMAP *boss_bullet_special,
    ALLEGRO_BITMAP *heart_full, ALLEGRO_BITMAP *heart_null, ALLEGRO_BITMAP *icon,
    ALLEGRO_BITMAP *explosion_sprite, ALLEGRO_BITMAP *explosion_boss, ALLEGRO_BITMAP *item_sprite,
    ALLEGRO_BITMAP *item_sprite_2, ALLEGRO_BITMAP *bulletEnemy_boss2, ALLEGRO_BITMAP *sla);

void draw_pause_message(ALLEGRO_FONT *font, ALLEGRO_BITMAP *background, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_DISPLAY *display, Player *player, int *exit_game);
void draw_player_life(ALLEGRO_BITMAP *heart_full, ALLEGRO_BITMAP *heart_empty, Player *player);
void draw_timer(ALLEGRO_FONT *font, double elapsed_time);
void play_explosion(int x, int y, ALLEGRO_BITMAP *explosion_bitmaps[], ALLEGRO_BITMAP *background);

extern ALLEGRO_FONT *menu_font;
extern ALLEGRO_BITMAP *menu_background;

void draw_menu();
void init_menu_resources();
void cleanup_menu_resources();
void show_game_over_menu(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font, int *restart_game, int *exit_game);
void exibir_texto_gradualmente(const char *texto, ALLEGRO_FONT *fonte, float x, float y, ALLEGRO_COLOR cor, float intervalo_entre_letras, ALLEGRO_BITMAP *background_m, float background_x);
void show_transition_menu(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font, int *continue_game, int *exit_game);
void init_second_phase(Player *player, Enemy enemies[], Bullet bullets[], ShootingEnemy shooting_enemies[], Boss *boss, int *victory_state, int *player_won, double *start_time, int *enemy_destroyed_count, int game_phase);
void show_victory_message(ALLEGRO_FONT *font, int *exit_game);
void init_options(GameOptions *game_options);
void run_menu(ALLEGRO_FONT *font_menu, ALLEGRO_BITMAP *background, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_DISPLAY *display, ALLEGRO_AUDIO_STREAM *music_menu, GameOptions *game_options, int *exit_game);
void exibir_tela_controles(ALLEGRO_FONT *font, ALLEGRO_BITMAP *background, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_DISPLAY *display, int *exit_game, int *menu_running);
void exibir_tela_opcoes(ALLEGRO_FONT *font, ALLEGRO_BITMAP *background, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_DISPLAY *display, GameOptions *game_options, int *exit_game, int *menu_running);

#endif // ENEMY_H