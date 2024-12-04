#ifndef COLLISION_H
#define COLLISION_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include <time.h>
#include<stdio.h>

#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "collision.h"
#include "boss.h"
#include "structs.h"
#include "defines.h"


// Verifica colisões entre as balas do chefe e o jogador
// Esta função verifica se alguma bala disparada pelo chefe atingiu o jogador.
// Se houver colisão, a lógica correspondente (como redução de vidas ou invulnerabilidade) será aplicada.
void check_boss_bullet_collisions(Player *player, Boss *boss, BossBullet boss_bullets[], int *game_over, int game_phase);

// Verifica colisão entre o chefe e as balas do jogador
// Esta função avalia se as balas disparadas pelo jogador atingiram o chefe.
// Se o chefe for atingido, a função atualiza a pontuação, a saúde do chefe e determina se o jogo foi vencido.
void check_boss_collision(Player *player, Bullet bullets[], int bullet_count, Boss *boss, int *score, int *player_won, int *game_over, int game_phase);

// Verifica colisões entre as balas do inimigo que atira e o jogador
// Esta função determina se alguma bala disparada pelos inimigos que atiram colidiu com o jogador.
// Se houver colisão, o estado do jogo pode mudar, incluindo redução de vidas ou término do jogo.
void check_enemy_bullet_collisions(Player *player, ShootingEnemy *enemy, int *game_over);

// Verifica todas as colisões no jogo
// Essa função centraliza a lógica de colisões entre:
// - O jogador e inimigos básicos, inimigos que atiram e chefes
// - Balas disparadas por ambos os lados (jogador e inimigos)
// - Itens especiais nas fases 1 e 2
// Atualiza o estado do jogo, incluindo pontuação, destruição de inimigos, e determina se o jogo deve terminar.
void check_collisions(Player *player, Bullet bullets[], int bullet_count, Enemy enemies[], int enemy_count, ShootingEnemy shooting_enemies[], int shooting_enemy_count, Item *item_phase1, Item *item_phase2, int *score, int *game_over, int *enemy_destroyed_count, int game_phase);

// Desenha a explosão de inimigos que atiram
// Renderiza a animação de explosão de um inimigo que atira quando ele é destruído.
// Utiliza um sprite de explosão e um limite de tempo para a animação.
void draw_explosion_shoot(ShootingEnemy *shooting_enemy, ALLEGRO_BITMAP *explosion_sprite, double explosion_duration);

#endif // COLLISION_H

