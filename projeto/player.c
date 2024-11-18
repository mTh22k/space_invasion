#include "player.h"

void init_player(Player *player)
{
    player->x = 50;
    player->y = SCREEN_HEIGHT / 2;
    player->width = 50;
    player->height = 30;
    player->lives = 3;
    player->invulnerable = 0;
    player->invulnerable_time = 0;
    player->joystick.right = player->joystick.left = player->joystick.up = player->joystick.down = player->joystick.fire = 0;
    player->paused = 0;
}

void move_player(Player *player)
{
    if (player->joystick.up && player->y > 0)
        player->y -= PLAYER_SPEED;
    if (player->joystick.down && player->y < SCREEN_HEIGHT - player->height)
        player->y += PLAYER_SPEED;
    if (player->joystick.left && player->x > 0)
        player->x -= PLAYER_SPEED;
    if (player->joystick.right && player->x < SCREEN_WIDTH - player->width)
        player->x += PLAYER_SPEED;
}
