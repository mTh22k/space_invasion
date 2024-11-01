#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define PLAYER_SPEED 6
#define BULLET_SPEED 6
#define ENEMY_SPEED 5
#define ENEMY_SPEED_SHOOTING 3
#define MAX_BULLETS 8
#define MAX_BULLET_COUNT 7
#define MAX_BOSS_BULLETS 8
#define MAX_ENEMIES 6
#define MAX_SHOOTING_ENEMIES 2
#define FIRE_INTERVAL 0.2        // Intervalo de disparo em segundos
#define INVULNERABILITY_TIME 1.5 // Tempo de invulnerabilidade em segundos
#define BOSS_SHOT_INTERVAL 0.4
#define TIME_TO_BOSS 5
#define SCROLL_SPEED 60

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

typedef struct
{
    float x, y;
    int width, height;
    int active;
    float speed;
} EnemyBullet;

typedef struct
{
    float x, y;
    int width, height;
    int active;
    float speed;
} BossBullet;

typedef struct
{
    float x, y;
    int width, height;
    int active;
    int health;
    float vertical_speed;
    EnemyBullet bullets[10];
    double last_shot_time;
    int moving_up; // Flag para direção de movimento
} ShootingEnemy;

typedef struct
{
    float x, y;
    int width, height;
    int active;
    float speed; // Velocidade de movimento vertical
    int health;
    double last_shot_time;
} Boss;

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

void init_boss(Boss *boss)
{
    boss->x = SCREEN_WIDTH;      // Inicia fora da tela
    boss->y = SCREEN_HEIGHT / 2; // Posição vertical inicial
    boss->width = 200;           // Largura do chefe
    boss->height = 200;          // Altura do chefe
    boss->active = 0;            // Inicialmente inativo
    boss->speed = 2;           // Velocidade do movimento vertical
    boss->health = 10;
    boss->last_shot_time = 0;
}

void init_shooting_enemies(ShootingEnemy *enemy)
{
    enemy->x = SCREEN_WIDTH + rand() % 100;
    enemy->y = rand() % (SCREEN_HEIGHT - 30);
    enemy->width = 50;
    enemy->height = 30;
    enemy->active = 1;
    enemy->health = 3;
    enemy->vertical_speed = 0.5; // Velocidade aleatória entre 1.0 e 2.0
    enemy->last_shot_time = 0;
    enemy->moving_up = 1;

    for (int i = 0; i < 4; i++)
    {
        enemy->bullets[i].active = 0;
        enemy->bullets[i].width = 10;
        enemy->bullets[i].height = 10;
        enemy->bullets[i].speed = 8;
    }
}

void init_enemy_bullets(Bullet enemy_bullets[], int count)
{
    for (int i = 0; i < count; i++)
        enemy_bullets[i].active = 0;
}

void shoot_enemy_bullet(ShootingEnemy *enemy, float player_x, float player_y)
{
    double current_time = al_get_time();
    if (current_time - enemy->last_shot_time >= 1.0)
    { // Dispara a cada 1 segundo
        for (int i = 0; i < 3; i++)
        {
            if (!enemy->bullets[i].active)
            {
                enemy->bullets[i].x = enemy->x - 20;
                enemy->bullets[i].y = enemy->y;
                enemy->bullets[i].active = 1;
                enemy->last_shot_time = current_time;
                break;
            }
        }
    }
}

void move_enemy_bullets(EnemyBullet bullets[], int count)
{
    for (int i = 0; i < count; i++)
    {
        if (bullets[i].active)
        {
            bullets[i].x -= bullets[i].speed;
            if (bullets[i].x < 0)
                bullets[i].active = 0;
        }
    }
}

void move_shooting_enemy(ShootingEnemy *enemy)
{
    if (enemy->active)
    {
        // Movimento horizontal
        enemy->x -= ENEMY_SPEED_SHOOTING;

        // Movimento vertical
        if (enemy->moving_up)
        {
            enemy->y -= enemy->vertical_speed;
            if (enemy->y <= 0)
                enemy->moving_up = 0;
        }
        else
        {
            enemy->y += enemy->vertical_speed;
            if (enemy->y >= SCREEN_HEIGHT - enemy->height)
                enemy->moving_up = 1;
        }

        // Reposicionar se sair da tela à esquerda
        if (enemy->x < -enemy->width)
        {
            enemy->x = SCREEN_WIDTH + rand() % 100;
            enemy->y = rand() % (SCREEN_HEIGHT - enemy->height);
        }
    }
}

void move_boss(Boss *boss)
{
    if (boss->active)
    {
        // Movimento vertical
        boss->y += boss->speed;
        // Inverte a direção se atingir o topo ou fundo da tela
        if (boss->y <= 0 || boss->y >= SCREEN_HEIGHT - boss->height)
        {
            boss->speed = -boss->speed; // Inverte a direção
        }
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

            // Inverte a direção vertical ao encontrar as bordas superior ou inferior da tela
            if (enemies[i].y <= 0 || enemies[i].y >= SCREEN_HEIGHT - enemies[i].height)
            {
                enemies[i].vertical_speed *= -1; // Inverte a direção
            }

            // Reposicionar inimigos que saíram da tela à esquerda
            if (enemies[i].x < -enemies[i].width)
            {
                enemies[i].x = SCREEN_WIDTH + rand() % 100;
                enemies[i].y = rand() % (SCREEN_HEIGHT - enemies[i].height);               // Reposiciona verticalmente
                enemies[i].vertical_speed = (rand() % 3 + 1) * (rand() % 2 == 0 ? 1 : -1); // Atualiza a velocidade vertical
            }
        }
    }
}
void check_boss_collision(Player *player, Bullet bullets[], int bullet_count, Boss *boss, int *score, int *player_won, int *game_over)
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
            boss->health--;        // Diminui a vida do chefe
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

void check_shooting_enemy_collision(Player *player, ShootingEnemy *enemy, int *game_over)
{
    if (enemy->active &&
        player->x < enemy->x + enemy->width &&
        player->x + player->width > enemy->x &&
        player->y < enemy->y + enemy->height &&
        player->y + player->height > enemy->y)
    {

        if (!player->invulnerable)
        {
            player->lives--;
            player->invulnerable = 1;
            player->invulnerable_time = al_get_time();

            if (player->lives <= 0)
            {
                *game_over = 1;
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
                player->invulnerable = 1;
                player->invulnerable_time = al_get_time();

                if (player->lives <= 0)
                    *game_over = 1;
            }
            enemy->bullets[i].active = 0;
        }
    }
}

void check_collisions(Player *player, Bullet bullets[], int bullet_count, Enemy enemies[], int enemy_count, ShootingEnemy shooting_enemies[], int shooting_enemy_count, int *score, int *game_over)
{
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
                    bullets[i].active = 0;
                    enemies[j].health--;
                    (*score)++;
                    if (enemies[j].health <= 0)
                        enemies[j].active = 0;
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
                    bullets[i].active = 0;        // Desativa a bala do jogador
                    shooting_enemies[k].health--; // Diminui a vida do inimigo que dispara
                    (*score)++;
                    if (shooting_enemies[k].health <= 0)
                        shooting_enemies[k].active = 0; // Desativa o inimigo se a vida for zero
                }
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
                player->invulnerable = 1;
                player->invulnerable_time = al_get_time();

                if (player->lives <= 0)
                    *game_over = 1;
            }
        }
    }
}

void init_boss_bullets(BossBullet boss_bullets[], int count)
{
    for (int i = 0; i < count; i++)
        boss_bullets[i].active = 0;
}
// Dispara uma bala do chefe
void shoot_boss_bullet(Boss *boss, BossBullet boss_bullets[], int *boss_bullet_count)
{
    double current_time = al_get_time();

    if (current_time - boss->last_shot_time >= BOSS_SHOT_INTERVAL) // Verifica o intervalo
    {
        for (int i = 0; i < MAX_BOSS_BULLETS; i++)
        {
            if (!boss_bullets[i].active)
            {
                boss_bullets[i].x = boss->x;
                boss_bullets[i].y = boss->y + boss->height / 2 - 20;
                boss_bullets[i].active = 1;
                boss_bullets[i].speed = BULLET_SPEED;
                boss->last_shot_time = current_time; // Atualiza o último disparo do chefe
                (*boss_bullet_count)++;
                break;
            }
        }
    }
}
void move_boss_bullets(BossBullet boss_bullets[], int count)
{
    for (int i = 0; i < count; i++)
    {
        if (boss_bullets[i].active)
        {
            boss_bullets[i].x -= boss_bullets[i].speed;
            if (boss_bullets[i].x < 0)
                boss_bullets[i].active = 0;
        }
    }
}
void check_boss_bullet_collisions(Player *player, BossBullet boss_bullets[], int *game_over)
{
    for (int i = 0; i < MAX_BOSS_BULLETS; i++)
    {
        if (boss_bullets[i].active &&
            boss_bullets[i].x < player->x + player->width &&
            boss_bullets[i].x + boss_bullets[i].width > player->x &&
            boss_bullets[i].y < player->y + player->height &&
            boss_bullets[i].y + boss_bullets[i].height > player->y)
        {
            if (!player->invulnerable)
            {
                player->lives--;
                player->invulnerable = 1;
                player->invulnerable_time = al_get_time();

                if (player->lives <= 0)
                    *game_over = 1;
            }
            boss_bullets[i].active = 0; // Desativa a bala do chefe
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

void generate_shooting_enemy(ShootingEnemy enemies[], int count)
{
    for (int i = 0; i < count; i++)
    {
        if (!enemies[i].active)
        {
            init_shooting_enemies(&enemies[i]);
            break; // Quebra para gerar apenas um por vez
        }
    }
}

void draw_timer(ALLEGRO_FONT *font, double elapsed_time)
{
    char timer_text[20];
    sprintf(timer_text, "Tempo: %.1f", 10.0 - elapsed_time);
    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH - 200, 10, 0, timer_text);
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
    ALLEGRO_BITMAP *background = al_load_bitmap("background.png");
    ALLEGRO_BITMAP *player_sprite = al_load_bitmap("nave.png");
    ALLEGRO_BITMAP *bullet_sprite = al_load_bitmap("bullet.png");
    ALLEGRO_BITMAP *enemy_sprite = al_load_bitmap("enemy.png");
    ALLEGRO_BITMAP *shooting_enemy_sprite = al_load_bitmap("enemyShoot.png");
    ALLEGRO_BITMAP *enemy_bullet_sprite = al_load_bitmap("bulletEnemy.png");
    ALLEGRO_BITMAP *boss_sprite = al_load_bitmap("ship_1.png");
    ALLEGRO_BITMAP *boss_bullet_sprite = al_load_bitmap("bulletEnemy.png");

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    Player player;
    Bullet bullets[MAX_BULLETS];
    Enemy enemies[MAX_ENEMIES];
    BossBullet boss_bullets[MAX_BOSS_BULLETS];
    Bullet enemy_bullets[MAX_BULLET_COUNT]; // Adicione essa linha
    ShootingEnemy shooting_enemies[MAX_SHOOTING_ENEMIES];
    Boss boss; // Declaração do chefe
    init_boss(&boss);
    init_shooting_enemies(shooting_enemies); // Adicione isso no main
    init_player(&player);
    init_bullets(bullets, MAX_BULLETS);
    init_enemies(enemies, MAX_ENEMIES);
    init_enemy_bullets(enemy_bullets, MAX_BULLET_COUNT); // Adicione essa linha na função main
    init_boss_bullets(boss_bullets, MAX_BOSS_BULLETS);

    int firing = 0;
    double last_fire_time = 0;
    int score = 0;
    int game_over = 0;
    int player_won = 0; // 0 = jogo em andamento, 1 = jogador ganhou
    int boss_bullet_count = 0;
    float background_x = 0;

    al_start_timer(timer);
    double start_time = al_get_time(); // Tempo inicial

    while (1)
    {
        int redraw = 0;
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        double current_time = al_get_time(); // Tempo atua]l

        double elapsed_time = al_get_time() - start_time;
        double remaining_time = TIME_TO_BOSS - elapsed_time;

        if (ev.type == ALLEGRO_EVENT_TIMER)

        {
            background_x -= 2; // Controla a velocidade de rolagem do fundo
            if (background_x <= -al_get_bitmap_width(background))
            {
                background_x = 0;
            }
            if (!game_over)
            {

                // Gera inimigos apenas enquanto o tempo restante for maior que zero

                draw_timer(font, elapsed_time);

                if (remaining_time > 0)
                {
                    if (rand() % 200 == 0)
                    {
                        generate_shooting_enemy(shooting_enemies, MAX_SHOOTING_ENEMIES);
                    }

                    if (rand() % 60 == 0)
                    {
                        generate_enemy(enemies, MAX_ENEMIES, player.width, player.height);
                    }

                    move_enemies(enemies, MAX_ENEMIES);
                    check_collisions(&player, bullets, MAX_BULLETS, enemies, MAX_ENEMIES, shooting_enemies, MAX_SHOOTING_ENEMIES, &score, &game_over);
                    for (int i = 0; i < MAX_SHOOTING_ENEMIES; i++)
                    {
                        move_shooting_enemy(&shooting_enemies[i]);                                // Mover todos os inimigos que atiram
                        shoot_enemy_bullet(&shooting_enemies[i], player.x, player.y);             // Cada um atira
                        move_enemy_bullets(shooting_enemies[i].bullets, 3);                       // Move os projéteis
                        check_enemy_bullet_collisions(&player, &shooting_enemies[i], &game_over); // Verifica colisões
                        check_shooting_enemy_collision(&player, &shooting_enemies[i], &game_over);
                    }

                    for (int i = 0; i < MAX_SHOOTING_ENEMIES; i++)
                    {
                        check_shooting_enemy_collision(&player, &shooting_enemies[i], &game_over);
                    }
                }

                if (remaining_time <= 0 && !boss.active)
                {
                    boss.active = 1;                    // Ativa o chefe
                    boss.x = SCREEN_WIDTH - boss.width; // Posiciona o chefe na tela
                }

                if (boss.active)
                {
                    // Verificar colisões entre balas do jogador e o chefe
                    shoot_boss_bullet(&boss, boss_bullets, &boss_bullet_count);
                    move_boss_bullets(boss_bullets, MAX_BOSS_BULLETS);
                    check_boss_bullet_collisions(&player, boss_bullets, &game_over);
                    check_boss_collision(&player, bullets, MAX_BULLETS, &boss, &score, &player_won, &game_over);
                }

                if (player_won)
                {
                    al_draw_text(font, al_map_rgb(0, 255, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "Você ganhou! e CHUUUUUUUUUUPA");
                    al_flip_display(); // Atualiza a tela para mostrar a mensagem
                    al_rest(2.5);      // Pausa por 3 segundos
                    break;             // Sai do loop principal
                }

                move_player(&player);
                move_bullets(bullets, MAX_BULLETS);

                // Lógica de invulnerabilidade
                if (player.invulnerable && (al_get_time() - player.invulnerable_time) > INVULNERABILITY_TIME)
                    player.invulnerable = 0;

                if (player.joystick.fire && (al_get_time() - last_fire_time) > FIRE_INTERVAL)
                {
                    fire_bullet(bullets, MAX_BULLETS, player.x + player.width, player.y + player.height / 2);
                    last_fire_time = al_get_time();
                }
                

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
            al_draw_bitmap(background, background_x, 0, 0);
            al_draw_bitmap(background, background_x + al_get_bitmap_width(background), 0, 0);

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

            if (remaining_time > 0)
            {
                // Desenhar inimigos
                for (int i = 0; i < MAX_ENEMIES; i++)
                {
                    if (enemies[i].active)
                        al_draw_bitmap(enemy_sprite, enemies[i].x, enemies[i].y, 0);
                }

                // Renderização dos inimigos que atiram
                for (int i = 0; i < MAX_SHOOTING_ENEMIES; i++)
                {
                    ShootingEnemy shooting_enemy = shooting_enemies[i]; // Obter o inimigo atual

                    if (shooting_enemy.active)
                    {
                        al_draw_bitmap(shooting_enemy_sprite, shooting_enemy.x, shooting_enemy.y, 0); // Desenhar o inimigo

                        // Renderizar os projéteis do inimigo
                        for (int j = 0; j < 10; j++) // Substitua 10 pelo número máximo de projéteis que cada inimigo pode ter
                        {
                            if (shooting_enemy.bullets[j].active)
                            {
                                al_draw_bitmap(enemy_bullet_sprite, shooting_enemy.bullets[j].x, shooting_enemy.bullets[j].y, 0); // Desenhar o projétil
                            }
                        }
                    }
                }
            }

            if (boss.active)
            {
                for (int i = 0; i < MAX_BOSS_BULLETS; i++)
                {
                    if (boss_bullets[i].active)
                    {
                        al_draw_bitmap(boss_bullet_sprite, boss_bullets[i].x, boss_bullets[i].y, 0);
                    }
                }
                move_boss(&boss);
                al_draw_bitmap(boss_sprite, boss.x, boss.y, 0);

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
