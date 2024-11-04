
#include "game.h"

void init_allegro()
{
    al_init();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();
    al_init_image_addon();
    srand(time(0));
}

void cleanup(ALLEGRO_BITMAP *background, ALLEGRO_BITMAP *player_sprite, ALLEGRO_BITMAP *bullet_sprite,
             ALLEGRO_BITMAP *enemy_sprite, ALLEGRO_BITMAP *shooting_enemy_sprite, ALLEGRO_BITMAP *enemy_bullet_sprite,
             ALLEGRO_BITMAP *boss_sprite, ALLEGRO_BITMAP *boss_bullet_sprite, ALLEGRO_EVENT_QUEUE *event_queue,
             ALLEGRO_TIMER *timer, ALLEGRO_DISPLAY *display, ALLEGRO_FONT *font, ALLEGRO_BITMAP *explosion_bitmaps[])
{

    al_destroy_bitmap(background);
    al_destroy_bitmap(player_sprite);
    al_destroy_bitmap(bullet_sprite);
    al_destroy_bitmap(enemy_sprite);
    al_destroy_bitmap(shooting_enemy_sprite);
    al_destroy_bitmap(enemy_bullet_sprite);
    al_destroy_bitmap(boss_sprite);
    al_destroy_bitmap(boss_bullet_sprite);

    // Liberação dos bitmaps de explosão
    for (int i = 0; i < EXPLOSION_FRAME_COUNT; i++)
    {
        if (explosion_bitmaps[i])
        {
            al_destroy_bitmap(explosion_bitmaps[i]);
        }
    }

    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_font(font);
}
