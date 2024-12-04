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
// Define todas as balas como inativas e prepara as configurações iniciais
void init_bullets(Bullet bullets[], int count);

// Dispara uma nova bala a partir da posição (x, y)
// Verifica se há espaço para disparar uma nova bala, define a posição e a ativa
void fire_bullet(Bullet bullets[], int count, float x, float y);

// Move as balas do jogador
// Atualiza a posição das balas dependendo do estado do jogador e da fase do jogo
void move_bullets(Bullet bullets[], int count, Player *player, ALLEGRO_FONT *font, int game_phase);

// Inicializa as balas do boss
// Define todas as balas do boss como inativas e configura o tamanho delas
void init_boss_bullets(BossBullet boss_bullets[], int count);

// Move as balas disparadas pelo boss
// Atualiza a posição das balas do boss e desativa as balas quando saem da tela
void move_boss_bullets(BossBullet boss_bullets[], int count, Player *player);

// Dispara as balas do inimigo
// Permite que o inimigo dispare balas dependendo do tempo passado e da fase do jogo
void shoot_enemy_bullet(ShootingEnemy *enemy, float player_x, float player_y, int game_phase);

// Move as balas disparadas pelos inimigos
// Atualiza a posição das balas dos inimigos e desativa as balas quando saem da tela
void move_enemy_bullets(EnemyBullet bullets[], int count);

// Inicializa as balas disparadas pelos inimigos
// Define todas as balas do inimigo como inativas inicialmente
void init_enemy_bullets(Bullet enemy_bullets[], int count);



#endif // BULLET_H
