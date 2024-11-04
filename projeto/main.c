#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "collision.h"
#include "boss.h"
#include "structs.h"
#include "game.h"

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
#define EXPLOSION_FRAME_COUNT 6

void draw_timer(ALLEGRO_FONT *font, double elapsed_time)
{
    char timer_text[20];
    sprintf(timer_text, "Tempo: %.1f", 10.0 - elapsed_time);
    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH - 200, 10, 0, timer_text);
}

void play_explosion(int x, int y, ALLEGRO_BITMAP *explosion_bitmaps[], ALLEGRO_BITMAP *background)
{
    for (int i = 0; i < EXPLOSION_FRAME_COUNT; i++)
    {
        al_draw_bitmap(background, 0, 0, 0); // Desenhar o fundo antes da explosão
        al_draw_bitmap(explosion_bitmaps[i], x, y, 0);
        al_flip_display();
        al_rest(0.3); // Duração de cada frame
    }
}

int main()
{
    init_allegro();

    ALLEGRO_DISPLAY *display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60);
    ALLEGRO_FONT *font = al_create_builtin_font();
    ALLEGRO_BITMAP *background = al_load_bitmap("imagens/background.png");
    ALLEGRO_BITMAP *player_sprite = al_load_bitmap("imagens/nave.png");
    ALLEGRO_BITMAP *bullet_sprite = al_load_bitmap("imagens/bullet.png");
    ALLEGRO_BITMAP *enemy_sprite = al_load_bitmap("imagens/enemy.png");
    ALLEGRO_BITMAP *shooting_enemy_sprite = al_load_bitmap("imagens/enemyShoot.png");
    ALLEGRO_BITMAP *enemy_bullet_sprite = al_load_bitmap("imagens/bulletEnemy.png");
    ALLEGRO_BITMAP *boss_sprite = al_load_bitmap("imagens/ship_1.png");
    ALLEGRO_BITMAP *boss_bullet_sprite = al_load_bitmap("imagens/bulletEnemy.png");

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
    int victory_state = 0; // 0 = sem vitória, 1 = animação de explosão, 2 = tela de vitória

    const char *explosion_frames[EXPLOSION_FRAME_COUNT] = {
        "imagens/frame1.png",
        "imagens/frame2.png",
        "imagens/frame3.png",
        "imagens/frame4.png",
        "imagens/frame5.png",
        "imagens/frame6.png"};

    ALLEGRO_BITMAP *explosion_bitmaps[EXPLOSION_FRAME_COUNT];
    for (int i = 0; i < EXPLOSION_FRAME_COUNT; i++)
    {
        explosion_bitmaps[i] = al_load_bitmap(explosion_frames[i]);
    }

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
                    // Posicione a explosão nas coordenadas do chefe derrotado
                    play_explosion(boss.x, boss.y, explosion_bitmaps, background);

                    // Exibir mensagem de vitória
                    al_draw_text(font, al_map_rgb(0, 255, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "Você ganhou! e CHUUUUUUUUUUPA");
                    al_flip_display();
                    al_rest(2.5);
                    break;
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

    cleanup(background, player_sprite, bullet_sprite, enemy_sprite, shooting_enemy_sprite, enemy_bullet_sprite,
            boss_sprite, boss_bullet_sprite, event_queue, timer, display, font, explosion_bitmaps);

    return 0;
}
