#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>

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
    float x, y;
    int width, height;
    int lives;                // Adicionando vidas
    int invulnerable;         // Indica se o jogador está invulnerável
    double invulnerable_time; // Tempo de invulnerabilidade
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
    int health; // Adiciona saúde para o inimigo
} Enemy;

void init_player(Player *player)
{
    player->x = 50;
    player->y = SCREEN_HEIGHT / 2;
    player->width = 50;
    player->height = 30;
    player->lives = 3;             // Inicializa com 3 vidas
    player->invulnerable = 0;      // Não está invulnerável
    player->invulnerable_time = 0; // Inicializa o tempo de invulnerabilidade
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
        enemies[i].active = 0; // Inicia como inativo
        enemies[i].health = 2; // Define saúde do inimigo (2 disparos para derrotar)
    }
}

void move_player(Player *player, int direction_x, int direction_y)
{
    player->x += direction_x * PLAYER_SPEED;
    player->y += direction_y * PLAYER_SPEED; // Adiciona movimentação vertical

    // Limita o jogador dentro da tela
    if (player->x < 0)
        player->x = 0;
    if (player->x > SCREEN_WIDTH - player->width)
        player->x = SCREEN_WIDTH - player->width;
    if (player->y < 0)
        player->y = 0;
    if (player->y > SCREEN_HEIGHT - player->height)
        player->y = SCREEN_HEIGHT - player->height;
}

void fire_bullet(Bullet bullets[], int count, float x, float y)
{
    for (int i = 0; i < count; i++)
    {
        if (!bullets[i].active)
        {
            bullets[i].x = x;
            bullets[i].y = y;
            bullets[i].active = 1;
            bullets[i].width = 45; // Defina a largura da bala
            bullets[i].height = 25; // Defina a altura da bala
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
            enemies[i].x -= ENEMY_SPEED;
            if (enemies[i].x < -enemies[i].width)
            {
                enemies[i].x = SCREEN_WIDTH + rand() % 100;
                enemies[i].y = rand() % SCREEN_HEIGHT;
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

                    // Colisão entre bala e inimigo
                    bullets[i].active = 0; // Desativa a bala
                    enemies[j].health--;   // Reduz a saúde do inimigo

                    // Aumenta a pontuação sempre que um inimigo é atingido
                    (*score)++;

                    // Verifica se o inimigo foi derrotado
                    if (enemies[j].health <= 0)
                    {
                        enemies[j].active = 0; // Desativa o inimigo se derrotado
                    }
                }
            }
        }
    }

    // Verificação de colisão entre o jogador e inimigos
    for (int j = 0; j < enemy_count; j++)
    {
        if(enemies[j].active &&
            enemies[j].x < player->x + player->width &&
            enemies[j].x + enemies[j].width > player->x &&
            enemies[j].y < player->y + player->height &&
            enemies[j].y + enemies[j].height > player->y)

        {

            // Colisão detectada, reduz a vida do jogador
            if (!player->invulnerable)
            {
                player->lives--;
                player->invulnerable = 1;                  // Ativa a invulnerabilidade
                player->invulnerable_time = al_get_time(); // Marca o tempo atual

                // Verifica se o jogador ficou sem vidas
                if (player->lives <= 0)
                {
                    *game_over = 1; // Termina o jogo
                }
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
            enemies[i].width = player_width;   // Define a largura do inimigo igual à largura do jogador
            enemies[i].height = player_height; // Define a altura do inimigo igual à altura do jogador
            enemies[i].active = 1;
            enemies[i].health = 2; // Reinicia a saúde do inimigo
            break;
        }
    }
}

void draw_player(Player *player, ALLEGRO_BITMAP *player_sprite)
{
    // Calcula a posição centralizada para a hitbox em relação ao sprite
    int sprite_width = al_get_bitmap_width(player_sprite);
    int sprite_height = al_get_bitmap_height(player_sprite);

    player->width = sprite_width; // Define a largura e altura da hitbox para coincidir com o sprite
    player->height = sprite_height;

    if (player->invulnerable)
    {
        // Desenha o jogador piscando
        if (((int)(al_get_time() * 10) % 2) == 0)
        {
            al_draw_tinted_bitmap(player_sprite, al_map_rgba_f(1, 1, 1, 0.5), player->x, player->y, 0);
        }
    }
    else
    {
        al_draw_bitmap(player_sprite, player->x, player->y, 0);
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

    ALLEGRO_DISPLAY *display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60);
    ALLEGRO_FONT *font = al_create_builtin_font();

    ALLEGRO_BITMAP *background = al_load_bitmap("background.jpg");
    ALLEGRO_BITMAP *player_sprite = al_load_bitmap("nave.png");
    ALLEGRO_BITMAP *bullet_sprite = al_load_bitmap("bullet.png"); // Carregue a imagem da bala
    ALLEGRO_BITMAP *enemy_sprite = al_load_bitmap("enemy.png");   // Carregue a imagem do inimigo

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    Player player;
    Bullet bullets[MAX_BULLETS];
    Enemy enemies[MAX_ENEMIES];
    init_player(&player);
    init_bullets(bullets, MAX_BULLETS);
    init_enemies(enemies, MAX_ENEMIES);

    int direction_x = 0, direction_y = 0;
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
                move_player(&player, direction_x, direction_y);
                move_bullets(bullets, MAX_BULLETS);
                move_enemies(enemies, MAX_ENEMIES);
                check_collisions(&player, bullets, MAX_BULLETS, enemies, MAX_ENEMIES, &score, &game_over);

                // Verifica o tempo de invulnerabilidade do jogador
                if (player.invulnerable && (al_get_time() - player.invulnerable_time) > INVULNERABILITY_TIME)
                {
                    player.invulnerable = 0; // Termina a invulnerabilidade
                }

                // Dispara balas de acordo com o intervalo
                if (firing && (al_get_time() - last_fire_time) > FIRE_INTERVAL)
                {
                    fire_bullet(bullets, MAX_BULLETS, player.x + player.width, player.y + player.height / 2);
                    last_fire_time = al_get_time();
                }

                // Gera inimigos aleatoriamente
                if (rand() % 60 == 0)
                {
                    generate_enemy(enemies, MAX_ENEMIES, player.width, player.height);
                }
            }

            redraw = 1;
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch (ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_W:
                direction_y = -1;
                break;
            case ALLEGRO_KEY_S:
                direction_y = 1;
                break;
            case ALLEGRO_KEY_A:
                direction_x = -1;
                break;
            case ALLEGRO_KEY_D:
                direction_x = 1;
                break;
            case ALLEGRO_KEY_ENTER:
                firing = 1;
                break;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch (ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_W:
            case ALLEGRO_KEY_S:
                direction_y = 0;
                break;
            case ALLEGRO_KEY_A:
            case ALLEGRO_KEY_D:
                direction_x = 0;
                break;
            case ALLEGRO_KEY_ENTER:
                firing = 0;
                break;
            }
        }

        if (redraw && al_is_event_queue_empty(event_queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(background, 0, 0, 0);

            draw_player(&player, player_sprite);

            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (bullets[i].active)
                {
                    al_draw_bitmap(bullet_sprite, bullets[i].x, bullets[i].y, 0); // Desenhe a sprite da bala
                }
            }

            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                if (enemies[i].active)
                {
                    al_draw_bitmap(enemy_sprite, enemies[i].x, enemies[i].y, 0); // Desenhe a sprite do inimigo
                }
            }

            al_draw_textf(font, al_map_rgb(255, 255, 255), 10, 10, 0, "Score: %d", score);
            al_draw_textf(font, al_map_rgb(255, 255, 255), 10, 30, 0, "Lives: %d", player.lives);

            if (game_over)
            {
                al_draw_text(font, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "Game Over");
            }

            al_flip_display();
        }
    }

    al_destroy_bitmap(background);
    al_destroy_bitmap(player_sprite);
    al_destroy_font(font);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_bitmap(bullet_sprite); // Destruir a imagem da bala
    al_destroy_bitmap(enemy_sprite);  // Destruir a imagem do inimigo

    return 0;
}
