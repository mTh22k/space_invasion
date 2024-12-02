#include "bullet.h"

void init_bullets(Bullet bullets[], int count)
{
    for (int i = 0; i < count; i++) {
        bullets[i].active = 0;
    }
}

void move_bullets(Bullet bullets[], int count, Player *player, ALLEGRO_FONT *font, int game_phase)
{
    // Verifica o tempo do ataque especial
    if (player->special_attack_active)
    {
        float tempo_passado = al_get_time() - player->special_attack_start_time;
        if (tempo_passado > 6.0f)
        {
            al_draw_text(font, al_map_rgb(255, 255, 255), 300, 400, ALLEGRO_ALIGN_CENTRE, "Voce pegou disparos rapidos por 6 segundos!");
            player->special_attack_active = false; // Desativa o ataque especial após 6 segundos
        }
    }

    // Movimento das balas
    for (int i = 0; i < count; i++)
    {
       if (bullets[i].active)
{
    // Verificar se o jogador tem o ataque especial ativo
    if (player->special_attack_active == false)
    {
        // Normal: velocidade padrão de disparo
        bullets[i].x += BULLET_SPEED;
    }
    else if (game_phase == 1 && player->special_attack_active == true)
    {
        // Fase 1: ataque especial aumenta a velocidade do disparo
        bullets[i].x += BULLET_SPEED * 4; // Tiro mais rápido durante o ataque especial
    }
    else if (game_phase == 2 && player->special_attack_active == true)
    {
        // Fase 2: ataque especial diminui a velocidade do disparo pela metade
        bullets[i].x += BULLET_SPEED * 1.5; // Tiro mais lento durante o ataque especial da fase 2
    }

    // Desativa a bala se ela sair da tela
    if (bullets[i].x > SCREEN_WIDTH)
    {
        bullets[i].active = 0;
    }
}

    }
}

void fire_bullet(Bullet bullets[], int count, float x, float y)
{
    for (int i = 0; i < count; i++)
    {
        if (!bullets[i].active)
        {
            bullets[i].x = x + 20;
            bullets[i].y = y - 5;
            bullets[i].active = 1;
            bullets[i].width = 50;
            bullets[i].height = 30;
            break;
        }
    }
}

void init_boss_bullets(BossBullet boss_bullets[], int count)
{
    for (int i = 0; i < count; i++)
    {
        boss_bullets[i].active = 0;
        boss_bullets[i].width = 45;
        boss_bullets[i].height = 25;
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

void shoot_enemy_bullet(ShootingEnemy *enemy, float player_x, float player_y, int game_phase)
{
    double current_time = al_get_time();
    enemy->ready_to_shoot = true;
    if (enemy->ready_to_shoot)
    {
        printf("kk \n");
        if (game_phase == 1)
        {
            printf("alo \n");

            if (current_time - enemy->last_shot_time >= 1.0)
            { // Dispara a cada 1 segundo
                for (int i = 0; i < 3; i++)
                {
                    if (!enemy->bullets[i].active)
                    {
                        enemy->bullets[i].x = enemy->x - 20;
                        enemy->bullets[i].y = enemy->y;
                        enemy->bullets[i].active = 1;
                        enemy->last_shot_time = current_time;
                         printf("Enemy %p shooting bullet %d at X: %f, Y: %f\n", enemy, i, enemy->bullets[i].x, enemy->bullets[i].y);
                        break;
                    }
                }
            }
        }
        else if (game_phase == 2)
        {

            if (current_time - enemy->last_shot_time >= 0.1)
            { // Dispara a cada 1 segundo
                for (int i = 0; i < 3; i++)
                {
                    if (!enemy->bullets[i].active)
                    {
                        enemy->bullets[i].x = enemy->x - 20;
                        enemy->bullets[i].y = enemy->y;
                        enemy->bullets[i].active = 1;
                        enemy->last_shot_time = current_time;
                        break;
                    }
                }
            }
        }
    }
}

void move_enemy_bullets(EnemyBullet bullets[], int count)
{
    for (int i = 0; i < count; i++)
    {
        if (bullets[i].active)
        {
            bullets[i].x -= bullets[i].speed;
             printf("Bullet %d moving. Active: %d, X: %f\n", i, bullets[i].active, bullets[i].x);
            if (bullets[i].x < 0)
                bullets[i].active = 0;
        }
    }
}

void init_enemy_bullets(Bullet enemy_bullets[], int count)
{
    for (int i = 0; i < count; i++)
        enemy_bullets[i].active = 0;
}