#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define PLAYER_SPEED 5
#define BULLET_SPEED 8
#define ENEMY_SPEED 3
#define MAX_BULLETS 8
#define MAX_ENEMIES 5
#define FIRE_INTERVAL 0.2        // Intervalo de disparo em segundos
#define INVULNERABILITY_TIME 1.5 // Tempo de invulnerabilidade em segundos

typedef struct
{
    unsigned char right, left, up, down, fire;
} Joystick;

typedef struct
{
    float x, y;
    int width, height;
    int lives;
    int invulnerable;
    double invulnerable_time;
    Joystick joystick;
} Player;

typedef struct
{
    float x, y;
    int width, height;
    int active;
} Bullet;

typedef struct
{
    float x, y;
    int width, height;
    int active;
    int health;
    float vertical_speed; // Velocidade vertical
    float original_y;     // Posição vertical original
    int moving_up;        // Flag para indicar se está se movendo para cima
} Enemy;

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
}

void init_bullets(Bullet bullets[], int count)
{
    for (int i = 0; i < count; i++)
        bullets[i].active = 0;
}

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
    }
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

void move_enemies(Enemy enemies[], int count)
{
    for (int i = 0; i < count; i++)
    {
        if (enemies[i].active)
        {
            enemies[i].x -= ENEMY_SPEED;               // Movimento horizontal
            enemies[i].y += enemies[i].vertical_speed; // Movimento vertical

            // Limitar o movimento vertical para que os inimigos não saiam da tela
            if (enemies[i].y < 0)
                enemies[i].y = 0;
            else if (enemies[i].y > SCREEN_HEIGHT - enemies[i].height)
                enemies[i].y = SCREEN_HEIGHT - enemies[i].height;

            // Reposicionar inimigos que saíram da tela à direita
            if (enemies[i].x < -enemies[i].width)
            {
                enemies[i].x = SCREEN_WIDTH + rand() % 100;
                enemies[i].y = rand() % (SCREEN_HEIGHT - enemies[i].height);               // Reposiciona verticalmente
                enemies[i].vertical_speed = (rand() % 3 + 1) * (rand() % 2 == 0 ? 1 : -1); // Atualiza a velocidade vertical
            }
        }
    }
}

void check_collisions(Player *player, Bullet bullets[], int bullet_count, Enemy enemies[], int enemy_count, int *score, int *game_over)
{
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
                    bullets[i].active = 0;
                    enemies[j].health--;
                    (*score)++;
                    if (enemies[j].health <= 0)
                        enemies[j].active = 0;
                }
            }
        }
    }

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
                player->invulnerable = 1;
                player->invulnerable_time = al_get_time();

                if (player->lives <= 0)
                    *game_over = 1;
            }
        }
    }
}

void generate_enemy(Enemy enemies[], int count, int player_width, int player_height)
{
    for (int i = 0; i < count; i++)
    {
        if (!enemies[i].active)
        {
            enemies[i].x = SCREEN_WIDTH + rand() % 100;
            enemies[i].y = rand() % SCREEN_HEIGHT;
            enemies[i].width = player_width;
            enemies[i].height = player_height;
            enemies[i].active = 1;
            enemies[i].health = 2;
            break;
        }
    }
}

int main()
{
    al_init();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();
    al_init_image_addon();
    srand(time(0));

    ALLEGRO_DISPLAY *display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60);
    ALLEGRO_FONT *font = al_create_builtin_font();
    ALLEGRO_BITMAP *background = al_load_bitmap("background.jpg");
    ALLEGRO_BITMAP *player_sprite = al_load_bitmap("nave.png");
    ALLEGRO_BITMAP *bullet_sprite = al_load_bitmap("bullet.png");
    ALLEGRO_BITMAP *enemy_sprite = al_load_bitmap("enemy.png");

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    Player player;
    Bullet bullets[MAX_BULLETS];
    Enemy enemies[MAX_ENEMIES];
    init_player(&player);
    init_bullets(bullets, MAX_BULLETS);
    init_enemies(enemies, MAX_ENEMIES);

    int firing = 0;
    double last_fire_time = 0;
    int score = 0;
    int game_over = 0;

    al_start_timer(timer);

    while (1)
    {
        int redraw = 0;
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            if (!game_over)
            {
                move_player(&player);
                move_bullets(bullets, MAX_BULLETS);
                move_enemies(enemies, MAX_ENEMIES);
                check_collisions(&player, bullets, MAX_BULLETS, enemies, MAX_ENEMIES, &score, &game_over);

                // Lógica de invulnerabilidade
                if (player.invulnerable && (al_get_time() - player.invulnerable_time) > INVULNERABILITY_TIME)
                    player.invulnerable = 0;

                if (player.joystick.fire && (al_get_time() - last_fire_time) > FIRE_INTERVAL)
                {
                    fire_bullet(bullets, MAX_BULLETS, player.x + player.width, player.y + player.height / 2);
                    last_fire_time = al_get_time();
                }

                if (rand() % 60 == 0)
                    generate_enemy(enemies, MAX_ENEMIES, player.width, player.height);
            }
            redraw = 1;
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch (ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_W:
                player.joystick.up = 1;
                break;
            case ALLEGRO_KEY_S:
                player.joystick.down = 1;
                break;
            case ALLEGRO_KEY_A:
                player.joystick.left = 1;
                break;
            case ALLEGRO_KEY_D:
                player.joystick.right = 1;
                break;
            case ALLEGRO_KEY_ENTER:
                player.joystick.fire = 1;
                break;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch (ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_W:
                player.joystick.up = 0;
                break;
            case ALLEGRO_KEY_S:
                player.joystick.down = 0;
                break;
            case ALLEGRO_KEY_A:
                player.joystick.left = 0;
                break;
            case ALLEGRO_KEY_D:
                player.joystick.right = 0;
                break;
            case ALLEGRO_KEY_ENTER:
                player.joystick.fire = 0;
                break;
            }
        }

        if (redraw && al_is_event_queue_empty(event_queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(background, 0, 0, 0);

            // Desenhar o jogador com efeito de piscamento se estiver invulnerável
            if (player.invulnerable)
            {
                // Alternar a visibilidade a cada 0.1 segundos
                if ((int)(al_get_time() * 10) % 2 == 0)
                {
                    al_draw_bitmap(player_sprite, player.x, player.y, 0);
                }
            }
            else
            {
                al_draw_bitmap(player_sprite, player.x, player.y, 0);
            }

            // Desenhar as balas
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (bullets[i].active)
                    al_draw_bitmap(bullet_sprite, bullets[i].x, bullets[i].y, 0);
            }

            // Desenhar inimigos
            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                if (enemies[i].active)
                    al_draw_bitmap(enemy_sprite, enemies[i].x, enemies[i].y, 0);
            }

            // Desenhar o placar
            al_draw_textf(font, al_map_rgb(255, 255, 255), 10, 10, 0, "Score: %d", score);
            al_draw_textf(font, al_map_rgb(255, 0, 0), SCREEN_WIDTH - 100, 10, 0, "Lives: %d", player.lives);

            // Verificar se o jogo acabou
            if (game_over)
            {
                al_draw_text(font, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "Game Over!");
            }

            al_flip_display();
        }
    }

    al_destroy_bitmap(background);
    al_destroy_bitmap(player_sprite);
    al_destroy_bitmap(bullet_sprite);
    al_destroy_bitmap(enemy_sprite);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_font(font);

    return 0;
}
