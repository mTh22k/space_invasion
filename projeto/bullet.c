#include "bullet.h"

#include <stdio.h>

void init_bullets(Bullet bullets[], int count)
{
    for (int i = 0; i < count; i++) {
        bullets[i].active = 0;
    }
}

void move_bullets(Bullet bullets[], int count, Player *player, ALLEGRO_FONT *font)
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
            if (player->special_attack_active == false)
            {
                bullets[i].x += BULLET_SPEED;
            }
            else
            {
                bullets[i].x += BULLET_SPEED * 4; // Tiro mais rápido durante o ataque especial
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
