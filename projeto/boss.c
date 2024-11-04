#include "boss.h"

void init_boss(Boss *boss)
{
    boss->x = SCREEN_WIDTH;      // Inicia fora da tela
    boss->y = SCREEN_HEIGHT / 2; // Posição vertical inicial
    boss->width = 200;           // Largura do chefe
    boss->height = 200;          // Altura do chefe
    boss->active = 0;            // Inicialmente inativo
    boss->speed = 2;             // Velocidade do movimento vertical
    boss->health = 10;
    boss->last_shot_time = 0;
}

void move_boss(Boss *boss)
{
    if (boss->active)
    {
        // Movimento vertical
        boss->y += boss->speed;
        // Inverte a direção se atingir o topo ou fundo da tela
        if (boss->y <= 0 || boss->y >= SCREEN_HEIGHT - boss->height)
        {
            boss->speed = -boss->speed; // Inverte a direção
        }
    }
}

void init_boss_bullets(BossBullet boss_bullets[], int count)
{
    for (int i = 0; i < count; i++)
        boss_bullets[i].active = 0;
}

void shoot_boss_bullet(Boss *boss, BossBullet boss_bullets[], int *boss_bullet_count)
{
    double current_time = al_get_time();

    if (current_time - boss->last_shot_time >= BOSS_SHOT_INTERVAL) // Verifica o intervalo
    {
        for (int i = 0; i < MAX_BOSS_BULLETS; i++)
        {
            if (!boss_bullets[i].active)
            {
                boss_bullets[i].x = boss->x;
                boss_bullets[i].y = boss->y + boss->height / 2 - 20;
                boss_bullets[i].active = 1;
                boss_bullets[i].speed = BULLET_SPEED;
                boss->last_shot_time = current_time; // Atualiza o último disparo do chefe
                (*boss_bullet_count)++;
                break;
            }
        }
    }
}

void move_boss_bullets(BossBullet boss_bullets[], int count)
{
    for (int i = 0; i < count; i++)
    {
        if (boss_bullets[i].active)
        {
            boss_bullets[i].x -= boss_bullets[i].speed;
            if (boss_bullets[i].x < 0)
                boss_bullets[i].active = 0;
        }
    }
}

void check_boss_bullet_collisions(Player *player, BossBullet boss_bullets[], int *game_over)
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
                player->lives--;
                player->invulnerable = 1;
                player->invulnerable_time = al_get_time();

                if (player->lives <= 0)
                    *game_over = 1;
            }
            boss_bullets[i].active = 0; // Desativa a bala do chefe
        }
    }
}