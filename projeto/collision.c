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
            player->invulnerable = 1; // Ativa o estado de invulnerabilidade temporária
            player->invulnerable_time = al_get_time();

            if (player->lives <= 0)
            {
                *game_over = 1; // Fim de jogo caso as vidas cheguem a zero
            }
        }
    }
}

// Função que verifica colisões entre as balas do chefe e o jogador
void check_boss_bullet_collisions(Player *player, Boss *boss, BossBullet boss_bullets[], int *game_over, int game_phase)
{
    // Itera sobre todas as balas do chefe
    for (int i = 0; i < MAX_BOSS_BULLETS; i++)
    {
        // Verifica se a bala está ativa e se houve colisão com o jogador
        if (boss_bullets[i].active &&
            boss_bullets[i].x < player->x + player->width && 
            boss_bullets[i].x + boss_bullets[i].width > player->x &&
            boss_bullets[i].y < player->y + player->height && 
            boss_bullets[i].y + boss_bullets[i].height > player->y)
        {
            // Se o jogador não estiver invulnerável, ele recebe dano
            if (!player->invulnerable)
            {
               
                int damage = (game_phase == 2) ? 1 : 1;    // Dano, que pode ser ajustado dependendo da fase
                player->lives -= damage;                   
                player->invulnerable = 1;                  
                player->invulnerable_time = al_get_time(); 

                // Se as vidas do jogador chegarem a 0, termina o jogo
                if (player->lives <= 0)
                    *game_over = 1; 
            }

            // Desativa a bala do chefe após a colisão
            boss_bullets[i].active = 0;

            // Verifica se está na fase 2 e se o ataque especial do chefe está ativo
            if (game_phase == 2)
            {
                if (boss->special_attack_active)
                {
                    // Se o ataque especial do chefe estiver ativo, o jogador tem sua velocidade reduzida
                    player->speed_multiplier = 0.2;                     
                    player->slow_effect_end_time = al_get_time() + 1.0; // Define o tempo de duração do efeito de desaceleração (1 segundo)
                }
            }

            // Se as vidas do jogador chegarem a 0, o jogo termina
            if (player->lives <= 0)
                *game_over = 1;
        }
    }
}
// Função que verifica colisões entre as balas dos inimigos e o jogador
void check_enemy_bullet_collisions(Player *player, ShootingEnemy *enemy, int *game_over)
{
    // Itera sobre as balas disparadas pelo inimigo (máximo de 3 balas)
    for (int i = 0; i < 3; i++)
    {
        // Verifica se a bala está ativa e se houve colisão com o jogador
        if (enemy->bullets[i].active &&
            enemy->bullets[i].x < player->x + player->width && 
            enemy->bullets[i].x + enemy->bullets[i].width > player->x &&
            enemy->bullets[i].y < player->y + player->height && 
            enemy->bullets[i].y + enemy->bullets[i].height > player->y)
        {
            // Se o jogador não estiver invulnerável, ele recebe dano
            if (!player->invulnerable)
            {
                // Diminui uma vida do jogador e imprime mensagem no console
                player->lives--;
                player->invulnerable = 1;                
                player->invulnerable_time = al_get_time(); 

                // Se as vidas do jogador chegarem a 0, termina o jogo
                if (player->lives <= 0)
                    *game_over = 1; 
            }

            // Desativa a bala após a colisão
            enemy->bullets[i].active = 0;
        }
    }
}

// Função para verificar as colisões entre o jogador, balas, inimigos e itens
void check_collisions(Player *player, Bullet bullets[], int bullet_count, Enemy enemies[], int enemy_count, ShootingEnemy shooting_enemies[], int shooting_enemy_count, Item *item_phase1, Item *item_phase2, int *score, int *game_over, int *enemy_destroyed_count, int game_phase)
{
    // Verificar colisões entre balas do jogador e inimigos normais
    for (int i = 0; i < bullet_count; i++)
    {
        if (bullets[i].active) 
        {
            for (int j = 0; j < enemy_count; j++) // Itera pelos inimigos normais
            {
                // Verifica se a bala colide com o inimigo
                if (enemies[j].active &&
                    bullets[i].x < enemies[j].x + enemies[j].width &&
                    bullets[i].x + bullets[i].width > enemies[j].x &&
                    bullets[i].y < enemies[j].y + enemies[j].height &&
                    bullets[i].y + bullets[i].height > enemies[j].y)
                {
                    bullets[i].active = 0; // Desativa a bala

                    // Aplica dano ao inimigo (dobrado na fase 2 se o ataque especial estiver ativo)
                    if (game_phase == 2 && player->special_attack_active)
                    {
                        enemies[j].health -= 2;
                    }
                    else
                    {
                        enemies[j].health--;
                    }

                    enemies[j].damaged = 1;                  
                    enemies[j].damaged_time = al_get_time(); 
                    (*score)++;                            
                    enemies[j].exploding = 1;
                    enemies[j].explosion_time = al_get_time();

                    // Se o inimigo for destruído, desativa ele e gera um item
                    if (enemies[j].health <= 0)
                    {
                        enemies[j].active = 0;
                        (*enemy_destroyed_count)++; 

                        // Gera item da fase 1
                        if (game_phase == 1 && item_phase1 != NULL && !item_phase1->active && *enemy_destroyed_count == 5)
                        {
                            item_phase1->x = enemies[j].x;
                            item_phase1->y = enemies[j].y;
                            item_phase1->active = true; 
                        }
                        // Gera item da fase 2
                        else if (game_phase == 2 && item_phase2 != NULL && !item_phase2->active && *enemy_destroyed_count == 4)
                        {
                            item_phase2->x = enemies[j].x;
                            item_phase2->y = enemies[j].y;
                            item_phase2->active = true; 
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

                    // Aplica dano ao inimigo atirador (dobrado na fase 2 se o ataque especial estiver ativo)
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

                    // Se o inimigo atirador for destruído, desativa ele
                    if (shooting_enemies[k].health <= 0)
                    {
                        shooting_enemies[k].active = 0; 
                    }
                }
            }
        }
    }

    // Verificar colisões entre o jogador e inimigos normais
    for (int j = 0; j < enemy_count; j++)
    {
        // Verifica se o jogador colide com um inimigo
        if (enemies[j].active &&
            enemies[j].x < player->x + player->width &&
            enemies[j].x + enemies[j].width > player->x &&
            enemies[j].y < player->y + player->height &&
            enemies[j].y + enemies[j].height > player->y)
        {
            // Se o jogador não estiver invulnerável, ele perde uma vida
            if (!player->invulnerable)
            {
                player->lives--;
                player->invulnerable = 1;                 // Torna o jogador invulnerável
                player->invulnerable_time = al_get_time();

                // Se as vidas do jogador forem 0, o jogo termina
                if (player->lives <= 0)
                    *game_over = 1;
            }
        }
    }

    // Verificar colisão com o item da fase 1
    if (item_phase1 != NULL && item_phase1->active &&
        player->x < item_phase1->x + item_phase1->width &&
        player->x + player->width > item_phase1->x &&
        player->y < item_phase1->y + item_phase1->height &&
        player->y + player->height > item_phase1->y)
    {
        item_phase1->active = false;                       // Consome o item da fase 1
        player->special_attack_start_time = al_get_time(); // Marca o tempo do início do ataque especial
        player->special_attack_active = true;              // Ativa o ataque especial
    }

    // Verificar colisão com o item da fase 2
    if (item_phase2 != NULL && item_phase2->active &&
        player->x < item_phase2->x + item_phase2->width &&
        player->x + player->width > item_phase2->x &&
        player->y < item_phase2->y + item_phase2->height &&
        player->y + player->height > item_phase2->y)
    {
        item_phase2->active = false;                       // Consome o item da fase 2
        player->special_attack_start_time = al_get_time(); // Marca o tempo do início do ataque especial
        player->special_attack_active = true;              // Ativa o ataque especial
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
            // Se o jogador não estiver invulnerável, ele perde uma vida
            if (!player->invulnerable)
            {
                player->lives--;
                player->invulnerable = 1;                 
                player->invulnerable_time = al_get_time();

                // Se as vidas do jogador forem 0, o jogo termina
                if (player->lives <= 0)
                    *game_over = 1;
            }
        }
    }
}
