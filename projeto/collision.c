#include "collision.h"

void check_boss_collision(Player *player, Bullet bullets[], int bullet_count, Boss *boss, int *score, int *player_won, int *game_over, int game_phase)
{
    // Verificar colisão entre balas do jogador e o chefe
    for (int i = 0; i < bullet_count; i++)
    {
        if (bullets[i].active && boss->active &&
            bullets[i].x < boss->x + boss->width &&
            bullets[i].x + bullets[i].width > boss->x &&
            bullets[i].y < boss->y + boss->height &&
            bullets[i].y + bullets[i].height > boss->y)
        {
            bullets[i].active = 0; // Desativa a bala do jogador

            if (game_phase == 2 && player->special_attack_active)
            {
                boss->health -= 2; // Aplica dano dobrado na fase 2 se o item foi consumido
            }
            else
            {
                boss->health--; // Aplica dano normal
            }
            boss->exploding = 1;
            boss->explosion_time = al_get_time();
            boss->damaged = 1;                  // Marca o chefe como danificado
            boss->damaged_time = al_get_time(); // Registra o tempo da colisão
            (*score)++;

            if (boss->health <= 0)
            {
                boss->active = 0; // Desativa o chefe
                *player_won = 1;  // Jogador ganhou
            }
        }
    }

    // Verificar colisão entre o jogador e o chefe
    if (boss->active &&
        player->x < boss->x + boss->width &&
        player->x + player->width > boss->x &&
        player->y < boss->y + boss->height &&
        player->y + player->height > boss->y)
    {
        // Verifica se o jogador não está invulnerável
        if (!player->invulnerable)
        {
            player->lives--;          // Diminui a vida do jogador
            printf("Colisão com o chefe! Perdeu uma vida. Vidas restantes: %d\n", player->lives);
            player->invulnerable = 1; // Ativa o estado de invulnerabilidade temporária
            player->invulnerable_time = al_get_time();

            if (player->lives <= 0)
            {
                *game_over = 1; // Fim de jogo caso as vidas cheguem a zero
            }
        }
    }
}

void check_boss_bullet_collisions(Player *player,Boss *boss ,BossBullet boss_bullets[], int *game_over, int game_phase)
{
    for (int i = 0; i < MAX_BOSS_BULLETS; i++)
    {
        if (boss_bullets[i].active &&
            boss_bullets[i].x < player->x + player->width &&
            boss_bullets[i].x + boss_bullets[i].width > player->x &&
            boss_bullets[i].y < player->y + player->height &&
            boss_bullets[i].y + boss_bullets[i].height > player->y)
        {
            if (!player->invulnerable)
            {
                int damage = (game_phase == 2) ? 1 : 1; // Se for a fase 2, aumenta o dano para 2
                player->lives -= damage;
                player->invulnerable = 1;
                player->invulnerable_time = al_get_time();

                if (player->lives <= 0)
                    *game_over = 1;
            }
            boss_bullets[i].active = 0; // Desativa a bala do chefe

            if (game_phase == 2)
            {
                if (boss->special_attack_active)
                {
                    player->speed_multiplier = 0.2;                     // Reduz a velocidade em 50%
                    player->slow_effect_end_time = al_get_time() + 1.0; // Duração de 2 segundos
                }
            }

            if (player->lives <= 0)
                *game_over = 1;
        }
    }
}



void check_enemy_bullet_collisions(Player *player, ShootingEnemy *enemy, int *game_over)
{
    for (int i = 0; i < 3; i++)
    {
        if (enemy->bullets[i].active &&
            enemy->bullets[i].x < player->x + player->width &&
            enemy->bullets[i].x + enemy->bullets[i].width > player->x &&
            enemy->bullets[i].y < player->y + player->height &&
            enemy->bullets[i].y + enemy->bullets[i].height > player->y)
        {
            if (!player->invulnerable)
            {
                player->lives--;
                printf("Colisão com a bala do inimigo! Perdeu uma vida. Vidas restantes: %d\n", player->lives);
                player->invulnerable = 1;
                player->invulnerable_time = al_get_time();

                if (player->lives <= 0)
                    *game_over = 1;
            }
            enemy->bullets[i].active = 0;
        }
    }
}

void check_collisions(Player *player, Bullet bullets[], int bullet_count, Enemy enemies[], int enemy_count, ShootingEnemy shooting_enemies[], int shooting_enemy_count, Item *item_phase1, Item *item_phase2, int *score, int *game_over, int *enemy_destroyed_count, int game_phase)
{
    // Verificar colisões entre balas do jogador e inimigos normais
    for (int i = 0; i < bullet_count; i++)
    {
        if (bullets[i].active)
        {
            for (int j = 0; j < enemy_count; j++)
            {
                if (enemies[j].active &&
                    bullets[i].x < enemies[j].x + enemies[j].width &&
                    bullets[i].x + bullets[i].width > enemies[j].x &&
                    bullets[i].y < enemies[j].y + enemies[j].height &&
                    bullets[i].y + bullets[i].height > enemies[j].y)
                {
                    bullets[i].active = 0; // Desativa a bala
                    // Aplica dano dobrado na fase 2 se o item foi consumido
                    if (game_phase == 2 && player->special_attack_active)
                    {
                        enemies[j].health -= 2;
                    }
                    else
                    {
                        enemies[j].health--;
                    } // Diminui a vida do inimigo
                    enemies[j].damaged = 1;                  // Marca como danificado
                    enemies[j].damaged_time = al_get_time(); // Registra o tempo
                    (*score)++;
                    enemies[j].exploding = 1;
                    enemies[j].explosion_time = al_get_time();

                    // Se o inimigo é destruído, conta e gera itens
                    if (enemies[j].health <= 0)
                    {
                        enemies[j].active = 0;
                        (*enemy_destroyed_count)++;

                        if (game_phase == 1 && item_phase1 != NULL && !item_phase1->active && *enemy_destroyed_count == 5)
                        {
                            item_phase1->x = enemies[j].x;
                            item_phase1->y = enemies[j].y;
                            item_phase1->active = true; // Gera o item da fase 1
                        }
                        else if (game_phase == 2 && item_phase2 != NULL && !item_phase2->active && *enemy_destroyed_count == 4) // Alterado para 5 para garantir que a fase 2 esteja bem avançada
                        {
                            item_phase2->x = enemies[j].x;
                            item_phase2->y = enemies[j].y;
                            item_phase2->active = true; // Gera o item da fase 2
                        }
                    }
                }
            }

            // Verificar colisões entre balas do jogador e inimigos que disparam
            for (int k = 0; k < shooting_enemy_count; k++)
            {
                if (shooting_enemies[k].active &&
                    bullets[i].x < shooting_enemies[k].x + shooting_enemies[k].width &&
                    bullets[i].x + bullets[i].width > shooting_enemies[k].x &&
                    bullets[i].y < shooting_enemies[k].y + shooting_enemies[k].height &&
                    bullets[i].y + bullets[i].height > shooting_enemies[k].y)
                {
                    bullets[i].active = 0; // Desativa a bala
                    if (game_phase == 2 && player->special_attack_active)
                    {
                        shooting_enemies[k].health -= 2;
                    }
                    else
                    {
                        shooting_enemies[k].health--;
                    }
                    shooting_enemies[k].exploding = 1;
                    shooting_enemies[k].explosion_time = al_get_time();
                    (*score)++;

                    if (shooting_enemies[k].health <= 0)
                    {
                        shooting_enemies[k].active = 0; // Desativa o inimigo

                    }
                }
            }
        }
    }

    // Verificar colisões entre o jogador e inimigos normais
    for (int j = 0; j < enemy_count; j++)
    {
        if (enemies[j].active &&
            enemies[j].x < player->x + player->width &&
            enemies[j].x + enemies[j].width > player->x &&
            enemies[j].y < player->y + player->height &&
            enemies[j].y + enemies[j].height > player->y)
        {
            if (!player->invulnerable)
            {
                player->lives--;
                printf("Colisão com inimigo normal! Perdeu uma vida. Vidas restantes: %d\n", player->lives);
                player->invulnerable = 1;
                player->invulnerable_time = al_get_time();

                if (player->lives <= 0)
                    *game_over = 1;
            }
        }
    }

    // Verificar colisão com o item da fase 1
    // Verificar colisão com o item da fase 1
    // Aumenta a área de colisão do item, mantendo a imagem do item inalterada

    // Verificar colisão com o item da fase 1
    if (item_phase1 != NULL && item_phase1->active &&
        player->x < item_phase1->x + item_phase1->width &&  // Aumenta a borda direita da colisão
        player->x + player->width > item_phase1->x &&       // Aumenta a borda esquerda da colisão
        player->y < item_phase1->y + item_phase1->height && // Aumenta a borda inferior da colisão
        player->y + player->height > item_phase1->y)        // Aumenta a borda superior da colisão
    {
        item_phase1->active = false; // Consome o item da fase 1
        player->special_attack_start_time = al_get_time();
        player->special_attack_active = true;
    }

    // Verificar colisão com o item da fase 2
    if (item_phase2 != NULL && item_phase2->active &&
        player->x < item_phase2->x + item_phase2->width &&  // Aumenta a borda direita da colisão
        player->x + player->width > item_phase2->x &&       // Aumenta a borda esquerda da colisão
        player->y < item_phase2->y + item_phase2->height && // Aumenta a borda inferior da colisão
        player->y + player->height > item_phase2->y)        // Aumenta a borda superior da colisão
    {

        item_phase2->active = false; // Consome o item da fase 2
        player->special_attack_start_time = al_get_time();
        player->special_attack_active = true;
    }

    // Verificar colisões entre o jogador e inimigos que disparam
    for (int k = 0; k < shooting_enemy_count; k++)
    {
        if (shooting_enemies[k].active &&
            shooting_enemies[k].x < player->x + player->width &&
            shooting_enemies[k].x + shooting_enemies[k].width > player->x &&
            shooting_enemies[k].y < player->y + player->height &&
            shooting_enemies[k].y + shooting_enemies[k].height > player->y)
        {
            if (!player->invulnerable)
            {
                player->lives--;
                printf("Colisão com o inimigo atirador! Perdeu uma vida. Vidas restantes: %d\n", player->lives);
                player->invulnerable = 1;
                player->invulnerable_time = al_get_time();

                if (player->lives <= 0)
                    *game_over = 1;
            }
        }
    }
}
