#include "collision.h"

#include <stdio.h>

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
                al_rest(0.8);
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
    // Reiniciar a contagem de inimigos destruídos ao começar a fase 2
    // if (game_phase == 2)
    // {
    //     printf("atualizou pra 0\n");
    //     enemy_destroyed_count = 0;
    // }

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
                        printf("Inimigo destruído! Explosão iniciada.\n");

                        if (game_phase == 1 && item_phase1 != NULL && !item_phase1->active && *enemy_destroyed_count == 3)
                        {
                            item_phase1->x = enemies[j].x;
                            item_phase1->y = enemies[j].y;
                            item_phase1->active = true;                                                  // Gera o item da fase 1
                            printf("Item fase 1 gerado em (%d, %d)!\n", item_phase1->x, item_phase1->y); // Log de geração
                        }
                        else if (game_phase == 2 && item_phase2 != NULL && !item_phase2->active && *enemy_destroyed_count == 1) // Alterado para 5 para garantir que a fase 2 esteja bem avançada
                        {
                            item_phase2->x = enemies[j].x;
                            item_phase2->y = enemies[j].y;
                            item_phase2->active = true;                                                  // Gera o item da fase 2
                            printf("Item fase 2 gerado em (%d, %d)!\n", item_phase2->x, item_phase2->y); // Log de geração
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
                    bullets[i].active = 0;                            // Desativa a bala
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
                
                    printf("Enemy %d hit! Health=%d\n", k, shooting_enemies[k].health);
                    if (shooting_enemies[k].health <= 0) {
                        printf("Enemy %d defeated!\n", k);
                        shooting_enemies[k].active = 0; // Desativa o inimigo
                    }
        
                }
               
            }

            // Verificar colisão com o item da fase 1
            if (item_phase1 != NULL && item_phase1->active &&
                player->x < item_phase1->x + 40 &&
                player->x + player->width > item_phase1->x &&
                player->y < item_phase1->y + 40 &&
                player->y + player->height > item_phase1->y)
            {
                item_phase1->active = false; // Consome o item da fase 1
                player->special_attack_start_time = al_get_time();
                player->special_attack_active = true;
                printf("Item fase 1 consumido!\n"); // Log para depuração
            }

            // Verificar colisão com o item da fase 2
            if (item_phase2 != NULL && item_phase2->active &&
                player->x < item_phase2->x + 40 &&
                player->x + player->width > item_phase2->x &&
                player->y < item_phase2->y + 40 &&
                player->y + player->height > item_phase2->y)
            {
                item_phase2->active = false; // Consome o item da fase 2
                player->special_attack_start_time = al_get_time();
                player->special_attack_active = true;
                printf("Item fase 2 consumido!\n"); // Log para depuração
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

void draw_explosion(Enemy *enemy, ALLEGRO_BITMAP *explosion_sprite, double explosion_duration)
{
    if (enemy->exploding)
    {
        double elapsed_time = al_get_time() - enemy->explosion_time; // Tempo desde o início da explosão

        // Se o tempo da explosão ainda não passou, desenha a explosão
        if (elapsed_time < explosion_duration)
        {
            al_draw_bitmap(explosion_sprite, enemy->x - 5, enemy->y + 5, 0);
        }
        else
        {
            enemy->exploding = 0; // Finaliza a explosão após o tempo
        }
    }
}

void draw_explosion_shoot(ShootingEnemy *shooting_enemy, ALLEGRO_BITMAP *explosion_sprite, double explosion_duration) 
{
    if (shooting_enemy->exploding)
    {
        double elapsed_time = al_get_time() - shooting_enemy->explosion_time; // Tempo desde o início da explosão

        // Se o tempo da explosão ainda não passou, desenha a explosão
        if (elapsed_time < explosion_duration)
        {
            al_draw_bitmap(explosion_sprite, shooting_enemy->x - 5, shooting_enemy->y + 5, 0);
        }
        else
        {
            shooting_enemy->exploding = 0; // Finaliza a explosão após o tempo
        }
    }
}

void draw_explosion_boss(Boss *boss, ALLEGRO_BITMAP *explosion_sprite, double explosion_duration)
{
    if (boss->exploding)
    {
        double elapsed_time = al_get_time() - boss->explosion_time; // Tempo desde o início da explosão

        // Se o tempo da explosão ainda não passou, desenha a explosão
        if (elapsed_time < explosion_duration)
        {
            // Definindo a transparência (alfa) para 128 (meia opacidade)
            ALLEGRO_COLOR color = al_map_rgba_f(1.0, 1.0, 1.0, 0.01); // Branco com 50% de opacidade

            al_draw_tinted_bitmap(explosion_sprite, color, boss->x + 25, boss->y + 25, 0);
        }
        else
        {
            boss->exploding = 0; // Finaliza a explosão após o tempo
        }
    }
}
