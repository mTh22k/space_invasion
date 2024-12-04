#include "bullet.h"

// Função para inicializar as balas do jogador
void init_bullets(Bullet bullets[], int count)
{
    // Define todas as balas como inativas inicialmente
    for (int i = 0; i < count; i++)
    {
        bullets[i].active = 0;
    }
}

// Função para mover as balas do jogador e lidar com o ataque especial
void move_bullets(Bullet bullets[], int count, Player *player, ALLEGRO_FONT *font, int game_phase)
{
    // Verifica se o ataque especial do jogador está ativo
    if (player->special_attack_active)
    {
        float tempo_passado = al_get_time() - player->special_attack_start_time;
        if (tempo_passado > 6.0f) // Desativa o ataque especial após 6 segundos
        {
            al_draw_text(font, al_map_rgb(255, 255, 255), 300, 400, ALLEGRO_ALIGN_CENTRE, "Voce pegou disparos rapidos por 6 segundos!");
            player->special_attack_active = false;
        }
    }

    // Move as balas do jogador
    for (int i = 0; i < count; i++)
    {
        if (bullets[i].active)
        {
            // Verifica a velocidade de disparo dependendo do ataque especial e fase do jogo
            if (!player->special_attack_active)
            {
                bullets[i].x += BULLET_SPEED; // Velocidade normal
            }
            else if (game_phase == 1)
            {
                bullets[i].x += BULLET_SPEED * 4; // Velocidade aumentada na fase 1
            }
            else if (game_phase == 2)
            {
                bullets[i].x += BULLET_SPEED * 1.5; // Velocidade reduzida na fase 2
            }

            // Desativa a bala se ela sair da tela
            if (bullets[i].x > SCREEN_WIDTH)
            {
                bullets[i].active = 0;
            }
        }
    }
}

// Função para disparar uma nova bala
void fire_bullet(Bullet bullets[], int count, float x, float y)
{
    for (int i = 0; i < count; i++)
    {
        if (!bullets[i].active) // Encontra a primeira bala inativa
        {
            bullets[i].x = x + 20;
            bullets[i].y = y - 5;
            bullets[i].active = 1;
            bullets[i].width = 50; // Define o tamanho da bala
            bullets[i].height = 30;
            break;
        }
    }
}

// Função para inicializar as balas do boss
void init_boss_bullets(BossBullet boss_bullets[], int count)
{
    for (int i = 0; i < count; i++)
    {
        boss_bullets[i].active = 0;
        boss_bullets[i].width = 45; // Define o tamanho das balas do boss
        boss_bullets[i].height = 25;
    }
}

// Função para mover as balas disparadas pelo boss
void move_boss_bullets(BossBullet boss_bullets[], int count)
{
    for (int i = 0; i < count; i++)
    {
        if (boss_bullets[i].active)
        {
            boss_bullets[i].x -= boss_bullets[i].speed; // Move a bala para a esquerda
            if (boss_bullets[i].x < 0)                  // Desativa a bala quando ela sair da tela
                boss_bullets[i].active = 0;
        }
    }
}

// Função para disparar as balas do inimigo
void shoot_enemy_bullet(ShootingEnemy *enemy, float player_x, float player_y, int game_phase)
{
    double current_time = al_get_time();
    enemy->ready_to_shoot = true; // O inimigo está pronto para disparar

    if (enemy->ready_to_shoot)
    {
        if (game_phase == 1) // Na fase 1, o inimigo atira a cada 1 segundo
        {
            if (current_time - enemy->last_shot_time >= 1.0)
            {
                for (int i = 0; i < 3; i++) // Limite de 3 balas disparadas
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
        else if (game_phase == 2) // Na fase 2, o inimigo atira mais rápido
        {
            if (current_time - enemy->last_shot_time >= 0.1)
            {
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

// Função para mover as balas disparadas pelos inimigos
void move_enemy_bullets(EnemyBullet bullets[], int count)
{
    for (int i = 0; i < count; i++)
    {
        if (bullets[i].active)
        {
            bullets[i].x -= bullets[i].speed; // Move a bala para a esquerda
            if (bullets[i].x < 0)             // Desativa a bala quando ela sair da tela
                bullets[i].active = 0;
        }
    }
}

// Função para inicializar as balas disparadas pelos inimigos
void init_enemy_bullets(Bullet enemy_bullets[], int count)
{
    for (int i = 0; i < count; i++)
        enemy_bullets[i].active = 0; // Inicializa todas as balas como inativas
}
