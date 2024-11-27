#include "player.h"

void init_player(Player *player)
{
    player->x = 50;
    player->y = SCREEN_HEIGHT / 2;
    player->width = 50;
    player->height = 30;
    player->lives = 4;
    player->invulnerable = 0;
    player->invulnerable_time = 0;
    player->joystick.right = player->joystick.left = player->joystick.up = player->joystick.down = player->joystick.fire = 0;
    player->paused = 0;
    player->special_attack_active = false;
}
void move_player(Player *player)
{
    // Atualiza o efeito de lentidão
    if (player->speed_multiplier < 1.0 && al_get_time() > player->slow_effect_end_time)
    {
        player->speed_multiplier = 1.0; // Remove o efeito de lentidão
    }

    // Move o jogador considerando o multiplicador de velocidade
    if (player->joystick.up && player->y > 0)
        player->y -= PLAYER_SPEED * player->speed_multiplier;
    if (player->joystick.down && player->y < SCREEN_HEIGHT - player->height)
        player->y += PLAYER_SPEED * player->speed_multiplier;
    if (player->joystick.left && player->x > 0)
        player->x -= PLAYER_SPEED * player->speed_multiplier;
    if (player->joystick.right && player->x < SCREEN_WIDTH - player->width)
        player->x += PLAYER_SPEED * player->speed_multiplier;
}
