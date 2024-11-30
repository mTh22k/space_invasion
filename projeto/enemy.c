#include "enemy.h"
#include <stdlib.h>
#include <stdio.h>

void init_enemies(Enemy enemies[], int count)
{
    for (int i = 0; i < count; i++)
    {
        enemies[i].active = 0;
        enemies[i].health = 2;
        enemies[i].vertical_speed = 1.0;                       // Velocidade vertical constante
        enemies[i].original_y = rand() % (SCREEN_HEIGHT - 30); // Posição inicial aleatória
        enemies[i].y = enemies[i].original_y;
        enemies[i].moving_up = 1; // Começar movendo para cima
        enemies[i].exploding = 0;
        enemies[i].explosion_time = 0;
    }
}

void move_enemies(Enemy enemies[], int count, int game_phase)
{

    if (game_phase == 1)
    {
        for (int i = 0; i < count; i++)
        {
            if (enemies[i].active)
            {
                enemies[i].x -= ENEMY_SPEED;               // Movimento horizontal
                enemies[i].y += enemies[i].vertical_speed; // Movimento vertical

                // Inverte a direção vertical ao encontrar as bordas superior ou inferior da tela
                if (enemies[i].y <= 0 || enemies[i].y >= SCREEN_HEIGHT - enemies[i].height)
                {
                    enemies[i].vertical_speed *= -1; // Inverte a direção
                }

                // Reposicionar inimigos que saíram da tela à esquerda
                if (enemies[i].x < -enemies[i].width)
                {
                    enemies[i].x = SCREEN_WIDTH + rand() % 100;
                    enemies[i].y = rand() % (SCREEN_HEIGHT - enemies[i].height);               // Reposiciona verticalmente
                    enemies[i].vertical_speed = (rand() % 3 + 1) * (rand() % 2 == 0 ? 1 : -1); // Atualiza a velocidade vertical
                }
            }
        }
    }
    else if (game_phase == 2)
    {
        
            for (int i = 0; i < count; i++)
            {
                if (enemies[i].active)
                {
                    enemies[i].x -= 8;                         // Movimento horizontal mais lento
                    enemies[i].y += enemies[i].vertical_speed; // Movimento vertical mais lento

                    // Inverte a direção vertical ao encontrar as bordas superior ou inferior da tela
                    if (enemies[i].y <= 0 || enemies[i].y >= SCREEN_HEIGHT - enemies[i].height)
                    {
                        enemies[i].vertical_speed *= -1; // Inverte a direção
                    }

                    // Reposicionar inimigos que saíram da tela à esquerda
                    if (enemies[i].x < -enemies[i].width)
                    {
                        enemies[i].x = SCREEN_WIDTH + rand() % 50; // Menos aleatoriedade no reposicionamento
                        enemies[i].y = rand() % (SCREEN_HEIGHT - enemies[i].height);
                        enemies[i].vertical_speed = rand() % 2 + 1; // Velocidade vertical mais lenta e menos aleatória
                    }
                }
            }
        
    }
}

void init_shooting_enemies(ShootingEnemy *enemy, int game_phase)
{
    enemy->x = SCREEN_WIDTH + rand() % 100;
    enemy->y = rand() % (SCREEN_HEIGHT - 30);
    enemy->width = 100;
    enemy->height = 50;
    enemy->active = 1;
    enemy->health = 2;
    enemy->vertical_speed = 0.5; // Velocidade aleatória entre 1.0 e 2.0
    enemy->last_shot_time = 0;
    enemy->moving_up = 1;
    enemy->explosion_time = 0;
    enemy->exploding = 0;

    for (int i = 0; i < 4; i++)
    {
        enemy->bullets[i].active = 0;
        enemy->bullets[i].width = 10;
        enemy->bullets[i].height = 10;
        enemy->bullets[i].speed = 8;
            
           
        }
    }

void move_shooting_enemy(ShootingEnemy *enemy)
{
    if (enemy->active)
    {
        // Movimento horizontal
        enemy->x -= ENEMY_SPEED_SHOOTING;

        // Movimento vertical
        if (enemy->moving_up)
        {
            enemy->y -= enemy->vertical_speed;
            if (enemy->y <= 0)
                enemy->moving_up = 0;
        }
        else
        {
            enemy->y += enemy->vertical_speed;
            if (enemy->y >= SCREEN_HEIGHT - enemy->height)
                enemy->moving_up = 1;
        }

        // Reposicionar se sair da tela à esquerda
        if (enemy->x < -enemy->width)
        {
            enemy->x = SCREEN_WIDTH + rand() % 100;
            enemy->y = rand() % (SCREEN_HEIGHT - enemy->height);
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

void generate_enemy(Enemy enemies[], int count, int player_width, int player_height)
{
    for (int i = 0; i < count; i++)
    {
        if (!enemies[i].active)
        {
            enemies[i].x = SCREEN_WIDTH + rand() % 100;
            enemies[i].y = rand() % SCREEN_HEIGHT;
            enemies[i].width = 70;
            enemies[i].height = 40;
            enemies[i].active = 1;
            enemies[i].health = 2;
            break;
        }
    }
}

void generate_shooting_enemy(ShootingEnemy enemies[], int count, int game_phase)
{
    for (int i = 0; i < count; i++)
    {
        if (!enemies[i].active)
        {
            init_shooting_enemies(&enemies[i], game_phase);
            break; // Quebra para gerar apenas um por vez
        }
    }
}
