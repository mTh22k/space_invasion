#ifndef RENDER_H
#define RENDER_H

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
#include "game.h"

void draw_player_with_effects( Player *player, GameOptions *game_options, 
                              ALLEGRO_BITMAP *player_sprite, ALLEGRO_BITMAP *player_sprite_dif1, 
                              ALLEGRO_BITMAP *player_sprite_dif2, ALLEGRO_BITMAP *player_sprite_dir, 
                              ALLEGRO_BITMAP *player_sprite_dir_dif1, ALLEGRO_BITMAP *player_sprite_dir_dif2, 
                              ALLEGRO_BITMAP *player_sprite_esq, ALLEGRO_BITMAP *player_sprite_esq_dif1, 
                              ALLEGRO_BITMAP *player_sprite_esq_dif2);
void draw_bullets(Bullet bullets[], int max_bullets, int game_phase, 
                  Player *player,  GameOptions *game_options, 
                  ALLEGRO_BITMAP *bullet_sprite, ALLEGRO_BITMAP *bullet_sprite_2, 
                  ALLEGRO_BITMAP *bullet_sprite_3, ALLEGRO_BITMAP *bullet_sprite_dif1, 
                  ALLEGRO_BITMAP *bullet_sprite_dif2);
void draw_enemies(Enemy enemies[], int max_enemies, int game_phase, 
                  ALLEGRO_BITMAP *enemy_sprite, ALLEGRO_BITMAP *enemy_sprite_2, 
                  ALLEGRO_BITMAP *explosion_sprite);
void draw_item_and_warning(Item *item,  Player *player, int game_phase, 
                           ALLEGRO_FONT *font_warn);
void draw_shooting_enemies(ShootingEnemy *shooting_enemies, int max_shooting_enemies, 
                           ALLEGRO_BITMAP *shooting_enemy_sprite, ALLEGRO_BITMAP *shooting_enemy_sprite_2, 
                           ALLEGRO_BITMAP *enemy_bullet_sprite, ALLEGRO_BITMAP *explosion_sprite, 
                           int game_phase);
void draw_boss(Boss *boss, BossBullet *boss_bullets, int max_boss_bullets, 
               ALLEGRO_BITMAP *boss_sprite, ALLEGRO_BITMAP *boss_sprite_2, 
               ALLEGRO_BITMAP *boss_bullet_sprite, ALLEGRO_BITMAP *bulletEnemy_boss2, 
               ALLEGRO_BITMAP *explosion_boss, int game_phase);
void draw_hud(ALLEGRO_FONT *font_info, ALLEGRO_BITMAP *icon, int score, const char *player_name);

#endif // RENDER.H