#include "collision.h"

void check_boss_collision(Player *player, Bullet bullets[], int bullet_count, Boss *boss, int *score, int *player_won, int *game_over)
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
            bullets[i].active = 0;              // Desativa a bala do jogador
            boss->health--;                     // Diminui a vida do chefe
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
            player->invulnerable = 1; // Ativa o estado de invulnerabilidade temporária
            player->invulnerable_time = al_get_time();

            if (player->lives <= 0)
            {
                *game_over = 1; // Fim de jogo caso as vidas cheguem a zero
            }
        }
    }
}

void check_shooting_enemy_collision(Player *player, ShootingEnemy *enemy, int *game_over)
{
    if (enemy->active &&
        player->x < enemy->x + enemy->width &&
        player->x + player->width > enemy->x &&
        player->y < enemy->y + enemy->height &&
        player->y + player->height > enemy->y)
    {
        if (!player->invulnerable)
        {
            player->lives--;
            player->invulnerable = 1;
            player->invulnerable_time = al_get_time();

            if (player->lives <= 0)
            {
                *game_over = 1;
            }
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
                player->invulnerable = 1;
                player->invulnerable_time = al_get_time();

                if (player->lives <= 0)
                    *game_over = 1;
            }
            enemy->bullets[i].active = 0;
        }
    }
}

void check_collisions(Player *player, Bullet bullets[], int bullet_count, Enemy enemies[], int enemy_count, ShootingEnemy shooting_enemies[], int shooting_enemy_count, int *score, int *game_over)
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
                    bullets[i].active = 0;                   // Desativa a bala
                    enemies[j].health--;                     // Diminui a vida
                    enemies[j].damaged = 1;                  // Marca como danificado
                    enemies[j].damaged_time = al_get_time(); // Registra o tempo
                    (*score)++;
                    if (enemies[j].health <= 0)
                        enemies[j].active = 0; // Desativa o inimigo
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
                    bullets[i].active = 0;                            // Desativa a bala
                    shooting_enemies[k].health--;                     // Diminui a vida
                    shooting_enemies[k].damaged = 1;                  // Marca como danificado
                    shooting_enemies[k].damaged_time = al_get_time(); // Registra o tempo
                    (*score)++;
                    if (shooting_enemies[k].health <= 0)
                        shooting_enemies[k].active = 0; // Desativa o inimigo
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
                player->invulnerable = 1;
                player->invulnerable_time = al_get_time();

                if (player->lives <= 0)
                    *game_over = 1;
            }
        }
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
                player->invulnerable = 1;
                player->invulnerable_time = al_get_time();

                if (player->lives <= 0)
                    *game_over = 1;
            }
        }
    }
}
