#include "bullet.h"

void init_bullets(Bullet bullets[], int count)
{
    for (int i = 0; i < count; i++)
        bullets[i].active = 0;
}

void move_bullets(Bullet bullets[], int count)
{
    for (int i = 0; i < count; i++)
    {
        if (bullets[i].active)
        {
            bullets[i].x += BULLET_SPEED;
            if (bullets[i].x > SCREEN_WIDTH)
                bullets[i].active = 0;
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
            bullets[i].width = 45;
            bullets[i].height = 25;
            break;
        }
    }
}
