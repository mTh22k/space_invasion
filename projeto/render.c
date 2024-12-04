#include "render.h"

// Função para desenhar o jogador com efeitos, como invulnerabilidade e movimento
void draw_player_with_effects(Player *player, GameOptions *game_options,
                              ALLEGRO_BITMAP *player_sprite, ALLEGRO_BITMAP *player_sprite_dif1,
                              ALLEGRO_BITMAP *player_sprite_dif2, ALLEGRO_BITMAP *player_sprite_dir,
                              ALLEGRO_BITMAP *player_sprite_dir_dif1, ALLEGRO_BITMAP *player_sprite_dir_dif2,
                              ALLEGRO_BITMAP *player_sprite_esq, ALLEGRO_BITMAP *player_sprite_esq_dif1,
                              ALLEGRO_BITMAP *player_sprite_esq_dif2)
{
    bool draw_sprite = true;

    // Aplica o efeito de piscamento se o jogador estiver invulnerável
    if (player->invulnerable && ((int)(al_get_time() * 10) % 2 != 0))
    {
        draw_sprite = false;
    }

    if (draw_sprite)
    {
        // Verifica o movimento do jogador e escolhe o sprite correspondente
        if (player->joystick.down)
        {
            switch (game_options->sprite_option)
            {
            case 0:
                al_draw_bitmap(player_sprite_dir, player->x, player->y, 0);
                break;
            case 1:
                al_draw_bitmap(player_sprite_dir_dif1, player->x, player->y, 0);
                break;
            case 2:
                al_draw_bitmap(player_sprite_dir_dif2, player->x, player->y, 0);
                break;
            }
        }
        else if (player->joystick.up)
        {
            switch (game_options->sprite_option)
            {
            case 0:
                al_draw_bitmap(player_sprite_esq, player->x, player->y, 0);
                break;
            case 1:
                al_draw_bitmap(player_sprite_esq_dif1, player->x, player->y, 0);
                break;
            case 2:
                al_draw_bitmap(player_sprite_esq_dif2, player->x, player->y, 0);
                break;
            }
        }
        else
        {
            // Caso o jogador não esteja se movendo, desenha o sprite padrão
            switch (game_options->sprite_option)
            {
            case 0:
                al_draw_bitmap(player_sprite, player->x, player->y, 0);
                break;
            case 1:
                al_draw_bitmap(player_sprite_dif1, player->x, player->y, 0);
                break;
            case 2:
                al_draw_bitmap(player_sprite_dif2, player->x, player->y, 0);
                break;
            }
        }
    }
}

// Função para desenhar as balas ativas no jogo
void draw_bullets(Bullet bullets[], int max_bullets, int game_phase,
                  Player *player, GameOptions *game_options,
                  ALLEGRO_BITMAP *bullet_sprite, ALLEGRO_BITMAP *bullet_sprite_2,
                  ALLEGRO_BITMAP *bullet_sprite_3, ALLEGRO_BITMAP *bullet_sprite_dif1,
                  ALLEGRO_BITMAP *bullet_sprite_dif2)
{
    for (int i = 0; i < max_bullets; i++)
    {
        if (bullets[i].active)
        {
            ALLEGRO_BITMAP *sprite_to_draw = NULL;

            // Determina o sprite a ser desenhado com base no ataque especial
            if (player->special_attack_active)
            {
                sprite_to_draw = (game_phase == 1) ? bullet_sprite_2 : bullet_sprite_3;
            }
            else
            {
                switch (game_options->new_option_1)
                {
                case 0:
                    sprite_to_draw = bullet_sprite;
                    break;
                case 1:
                    sprite_to_draw = bullet_sprite_dif1;
                    break;
                case 2:
                    sprite_to_draw = bullet_sprite_dif2;
                    break;
                }
            }

            // Desenha o sprite da bala
            if (sprite_to_draw)
            {
                al_draw_bitmap(sprite_to_draw, bullets[i].x + 15, bullets[i].y + 10, 0);
            }
        }
    }
}

// Função para desenhar os inimigos no jogo
void draw_enemies(Enemy enemies[], int max_enemies, int game_phase,
                  ALLEGRO_BITMAP *enemy_sprite, ALLEGRO_BITMAP *enemy_sprite_2,
                  ALLEGRO_BITMAP *explosion_sprite)
{
    for (int i = 0; i < max_enemies; i++)
    {
        if (enemies[i].active)
        {
            // Escolhe o sprite do inimigo baseado na fase do jogo
            ALLEGRO_BITMAP *sprite_to_draw = (game_phase == 1) ? enemy_sprite : enemy_sprite_2;

            // Desenha o inimigo
            al_draw_bitmap(sprite_to_draw, enemies[i].x, enemies[i].y, 0);
        }

        // Desenha a explosão, se o inimigo estiver destruído
        draw_explosion(&enemies[i], explosion_sprite, 0.1);
    }
}

// Função para desenhar um item e exibir um aviso relacionado ao item
void draw_item_and_warning(Item *item, Player *player, int game_phase,
                           ALLEGRO_FONT *font_warn)
{
    // Desenha o item, se ativo
    if (item->active)
    {
        al_draw_bitmap(item->sprite, item->x, item->y, 0);
    }

    // Exibe uma mensagem de aviso se o ataque especial estiver ativo
    if (player->special_attack_active)
    {
        const char *message = NULL;

        // Exibe a mensagem de acordo com a fase do jogo
        if (game_phase == 1)
        {
            message = "Voce pegou disparos rapidos por 6 segundos!";
        }
        else if (game_phase == 2)
        {
            message = "Voce pegou disparos com 2x de dano por 6 segundos!";
        }

        // Exibe a mensagem na tela
        if (message)
        {
            al_draw_text(font_warn, al_map_rgb(255, 255, 255), 400, 550, ALLEGRO_ALIGN_CENTRE, message);
        }
    }
}

// Função para desenhar os inimigos que atiram no jogo
void draw_shooting_enemies(ShootingEnemy *shooting_enemies, int max_shooting_enemies,
                           ALLEGRO_BITMAP *shooting_enemy_sprite, ALLEGRO_BITMAP *shooting_enemy_sprite_2,
                           ALLEGRO_BITMAP *enemy_bullet_sprite, ALLEGRO_BITMAP *explosion_sprite,
                           int game_phase)
{
    for (int i = 0; i < max_shooting_enemies; i++)
    {
        ShootingEnemy *shooting_enemy = &shooting_enemies[i]; // Obter o inimigo atual

        if (shooting_enemy->active)
        {
            // Fase 1: Desenha o inimigo
            if (game_phase == 1)
            {
                if (!shooting_enemy->ready_to_shoot)
                {
                    shooting_enemy->ready_to_shoot = true;
                }
                al_draw_bitmap(shooting_enemy_sprite, shooting_enemy->x, shooting_enemy->y, 0);
            }
            // Fase 2: Desenha o inimigo com outro sprite
            else if (game_phase == 2)
            {
                if (!shooting_enemy->ready_to_shoot)
                {
                    shooting_enemy->ready_to_shoot = true;
                }
                al_draw_bitmap(shooting_enemy_sprite_2, shooting_enemy->x, shooting_enemy->y, 0);
            }

            // Desenha os projéteis do inimigo
            for (int j = 0; j < 10; j++)
            { // Substitua 10 pelo número máximo de projéteis que cada inimigo pode ter
                if (shooting_enemy->bullets[j].active)
                {
                    al_draw_bitmap(enemy_bullet_sprite,
                                   shooting_enemy->bullets[j].x - 30,
                                   shooting_enemy->bullets[j].y + 20,
                                   0);
                }
            }

            // Renderiza a explosão do inimigo
            draw_explosion_shoot(shooting_enemy, explosion_sprite, 0.1);
        }
    }
}

// Função para desenhar o chefe do jogo, incluindo seus projéteis
void draw_boss(Boss *boss, BossBullet *boss_bullets, int max_boss_bullets,
               ALLEGRO_BITMAP *boss_sprite, ALLEGRO_BITMAP *boss_sprite_2,
               ALLEGRO_BITMAP *boss_bullet_sprite, ALLEGRO_BITMAP *bulletEnemy_boss2,
               ALLEGRO_BITMAP *explosion_boss, int game_phase)
{
    // Desenha os projéteis do chefe
    for (int i = 0; i < max_boss_bullets; i++)
    {
        if (boss_bullets[i].active)
        {
            // Fase 2: Se o chefe está com ataque especial, desenha outro tipo de projétil
            if (game_phase == 2)
            {
                if (boss->special_attack_active == 1)
                {
                    al_draw_bitmap(bulletEnemy_boss2, boss_bullets[i].x, boss_bullets[i].y, 0);
                }
                else
                {
                    al_draw_bitmap(boss_bullet_sprite, boss_bullets[i].x, boss_bullets[i].y, 0);
                }
            }
            else
            {
                al_draw_bitmap(boss_bullet_sprite, boss_bullets[i].x, boss_bullets[i].y, 0);
            }
        }
    }

    // Mover o chefe
    move_boss(boss, game_phase);

    // Desenha o chefe com o sprite correspondente à fase
    if (game_phase == 1)
    {
        al_draw_bitmap(boss_sprite, boss->x, boss->y, 0);
    }
    else if (game_phase == 2)
    {
        al_draw_bitmap(boss_sprite_2, boss->x, boss->y, 0);
    }

    // Renderiza a explosão do chefe
    draw_explosion_boss(boss, explosion_boss, 0.1);
}

// Função para desenhar o HUD (informações de score, ícone e nome do jogador)
void draw_hud(ALLEGRO_FONT *font_info, ALLEGRO_BITMAP *icon, int score, const char *player_name)
{
    // Desenha a pontuação na tela
    al_draw_textf(font_info, al_map_rgb(255, 255, 255), 680, 10, 0, "Score: %d", score);

    // Desenha o ícone do jogador
    al_draw_bitmap(icon, 0, 0, 0);

    // Desenha o nome do jogador
    al_draw_textf(font_info, al_map_rgb(255, 255, 255), 90, 50, 0, "%s", player_name);
}
