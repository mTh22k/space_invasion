#ifndef GAME_H
#define GAME_H

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
#include "defines.h"

// Função para inicializar os recursos do jogo (sprites, sons, fontes, etc.)
void init_resources();

// Função para liberar os recursos do jogo após o término
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
    ALLEGRO_BITMAP *item_sprite_2, ALLEGRO_BITMAP *bulletEnemy_boss2, ALLEGRO_BITMAP *slide_1, ALLEGRO_BITMAP *slide_2, ALLEGRO_BITMAP *slide_3);

// Função para desenhar a mensagem de pausa na tela
void draw_pause_message(ALLEGRO_FONT *font, ALLEGRO_BITMAP *background, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_DISPLAY *display, Player *player, int *exit_game);

// Função para desenhar a vida do jogador na tela
void draw_player_life(ALLEGRO_BITMAP *heart_full, ALLEGRO_BITMAP *heart_empty, Player *player);

// Variáveis globais externas para a fonte e fundo do menu
extern ALLEGRO_FONT *menu_font;
extern ALLEGRO_BITMAP *menu_background;

// Função para desenhar a explosão de um inimigo
void draw_explosion(Enemy *enemy, ALLEGRO_BITMAP *explosion_sprite, double explosion_duration);

// Função para exibir o menu de game over
void show_game_over_menu(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font, int *restart_game, int *exit_game);

// Função para exibir um texto gradualmente na tela, caractere por caractere
void exibir_texto_gradualmente(const char *texto, ALLEGRO_FONT *fonte, float x, float y, ALLEGRO_COLOR cor, float intervalo_entre_letras, ALLEGRO_BITMAP *background_m, float background_x);

// Função para exibir o menu de transição entre fases
void show_transition_menu(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font, int *continue_game, int *exit_game);

// Função para inicializar a segunda fase do jogo
void init_second_phase(Player *player, Enemy enemies[], Bullet bullets[], ShootingEnemy shooting_enemies[], Boss *boss, int *victory_state, int *player_won, double *start_time, int *enemy_destroyed_count, int game_phase);

// Função para exibir a mensagem de vitória
void show_victory_message(ALLEGRO_FONT *font, int *exit_game);

// Função para inicializar as opções de jogo
void init_options(GameOptions *game_options);

// Função para rodar o menu principal
void run_menu(ALLEGRO_FONT *font_menu, ALLEGRO_BITMAP *background, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_DISPLAY *display, ALLEGRO_AUDIO_STREAM *music_menu, GameOptions *game_options, int *exit_game);

// Função para exibir a tela de controles do jogo
void exibir_tela_controles(ALLEGRO_FONT *font, ALLEGRO_BITMAP *background, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_DISPLAY *display, int *exit_game, int *menu_running);

// Função para exibir a tela de opções do jogo
void exibir_tela_opcoes(ALLEGRO_FONT *font, ALLEGRO_BITMAP *background, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_DISPLAY *display, GameOptions *game_options, int *exit_game, int *menu_running);

// Função para desenhar a explosão de um inimigo que atira
void draw_explosion_shoot(ShootingEnemy *shooting_enemy, ALLEGRO_BITMAP *explosion_sprite, double explosion_duration);

// Função para desenhar a explosão do chefe
void draw_explosion_boss(Boss *boss, ALLEGRO_BITMAP *explosion_sprite, double explosion_duration);

// Função para selecionar o fundo com base na opção escolhida
void set_background(int option, ALLEGRO_BITMAP *backgrounds[], ALLEGRO_BITMAP **current_background);

// Função para atualizar os fundos com base nas opções do jogo
void update_backgrounds(GameOptions game_options, ALLEGRO_BITMAP *backgrounds[], ALLEGRO_BITMAP **current_background, ALLEGRO_BITMAP **current_background_2);

// Função para gerar inimigos da fase 1
void handle_phase1_enemy_generation(ShootingEnemy shooting_enemies[], int max_shooting_enemies,
                                    Enemy enemies[], int max_enemies, int game_phase, Player player);

// Função para gerar inimigos da fase 2
void handle_phase2_enemy_generation(ShootingEnemy shooting_enemies[], int max_shooting_enemies,
                                    Enemy enemies[], int max_enemies, int game_phase, Player player,
                                    double *phase2_start_time, bool *phase2_started);

// Função para lidar com os eventos de teclado enquanto o jogo está em andamento
void handle_keyboard_event(ALLEGRO_EVENT ev, Player *player, float *background_speed,
                           ALLEGRO_FONT *font_menu, ALLEGRO_BITMAP *current_background,
                           ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_DISPLAY *display,
                           int *exit_game);

// Função para lidar com o evento de liberação de teclas
void handle_keyboard_release_event(ALLEGRO_EVENT ev, Player *player, float *background_speed);

// Função para reiniciar o jogo
void restart_init_game(Player *player, Boss *boss, Enemy enemies[], int max_enemies,
                       Bullet bullets[], int max_bullets, int *game_over,
                       int *player_won, int *score, double *start_time, int *enemy_destroyed_count);

// Função para iniciar o slideshow e o jogo
void start_history_slide(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer,
                         ALLEGRO_BITMAP *image1, ALLEGRO_BITMAP *image2, ALLEGRO_BITMAP *image3, int *exit_game, int *history_game);

#endif // ENEMY_H