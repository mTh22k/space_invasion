#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

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
#define MAX_ENEMIES 4
#define MAX_SHOOTING_ENEMIES 1
#define FIRE_INTERVAL 0.2        // Intervalo de disparo em segundos
#define INVULNERABILITY_TIME 1.5 // Tempo de invulnerabilidade em segundos
#define BOSS_SHOT_INTERVAL 0.4
#define TIME_TO_BOSS 12
#define SCROLL_SPEED 60
#define EXPLOSION_FRAME_COUNT 5

int main()
{
    al_init();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    ALLEGRO_DISPLAY *display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!al_install_keyboard())
    {
        fprintf(stderr, "Falha ao instalar o teclado!\n");
        return -1;
    }

    GameOptions game_options;

    init_options(&game_options);

    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 50);
    ALLEGRO_FONT *font = al_load_ttf_font("fonts/ATC.ttf", 12, 0);
    ALLEGRO_FONT *font_menu = al_load_ttf_font("fonts/menu_f.ttf", 50, 0);
    ALLEGRO_FONT *font_warn = al_load_ttf_font("fonts/menu_f.ttf", 12, 0);
    ALLEGRO_FONT *font_info = al_load_ttf_font("fonts/menu_f.ttf", 20, 0);
    ALLEGRO_BITMAP *background = al_load_bitmap("imagens/background.png");
    ALLEGRO_BITMAP *background_2 = al_load_bitmap("imagens/background_2.png");
    ALLEGRO_BITMAP *background_3 = al_load_bitmap("imagens/background_3.png");

    ALLEGRO_BITMAP *player_sprite = al_load_bitmap("imagens/nave.png");
    if (!player_sprite)
    {
        fprintf(stderr, "Erro ao carregar a imagem 'imagens/nave.png'.\n");
        return -1;
    }

    ALLEGRO_BITMAP *player_sprite_dir = al_load_bitmap("imagens/nave_dir.png");
    if (!player_sprite_dir)
    {
        fprintf(stderr, "Erro ao carregar a imagem 'imagens/nave_dir.png'.\n");
        return -1;
    }

    ALLEGRO_BITMAP *player_sprite_esq = al_load_bitmap("imagens/nave_esq.png");
    if (!player_sprite_esq)
    {
        fprintf(stderr, "Erro ao carregar a imagem 'imagens/nave_esq.png'.\n");
        return -1;
    }

    ALLEGRO_BITMAP *player_sprite_dif1 = al_load_bitmap("imagens/nave1.png");
    if (!player_sprite_dif1)
    {
        fprintf(stderr, "Erro ao carregar a imagem 'imagens/nave1.png'.\n");
        return -1;
    }

    ALLEGRO_BITMAP *player_sprite_dir_dif1 = al_load_bitmap("imagens/nave_1_dir.png");
    if (!player_sprite_dir_dif1)
    {
        fprintf(stderr, "Erro ao carregar a imagem 'imagens/nave_1_dir.png'.\n");
        return -1;
    }

    ALLEGRO_BITMAP *player_sprite_esq_dif1 = al_load_bitmap("imagens/nave_1_esq.png");
    if (!player_sprite_esq_dif1)
    {
        fprintf(stderr, "Erro ao carregar a imagem 'imagens/nave_1_esq.png'.\n");
        return -1;
    }

    ALLEGRO_BITMAP *player_sprite_dif2 = al_load_bitmap("imagens/nave2.png");
    if (!player_sprite_dif2)
    {
        fprintf(stderr, "Erro ao carregar a imagem 'imagens/nave2.png'.\n");
        return -1;
    }

    ALLEGRO_BITMAP *player_sprite_dir_dif2 = al_load_bitmap("imagens/nave_2_dir.png");
    if (!player_sprite_dir_dif2)
    {
        fprintf(stderr, "Erro ao carregar a imagem 'imagens/nave_2_dir.png'.\n");
        return -1;
    }

    ALLEGRO_BITMAP *player_sprite_esq_dif2 = al_load_bitmap("imagens/nave_2_esq.png");
    if (!player_sprite_esq_dif2)
    {
        fprintf(stderr, "Erro ao carregar a imagem 'imagens/nave_2_esq.png'.\n");
        return -1;
    }
    ALLEGRO_BITMAP *bullet_sprite_dif1 = al_load_bitmap("imagens/bullet_dif1.png");
    ALLEGRO_BITMAP *bullet_sprite_dif2 = al_load_bitmap("imagens/bullet_dif2.png");
    ALLEGRO_BITMAP *bullet_sprite = al_load_bitmap("imagens/bullet.png");
    ALLEGRO_BITMAP *bullet_sprite_2 = al_load_bitmap("imagens/bullet_2.png");
    ALLEGRO_BITMAP *bullet_sprite_3 = al_load_bitmap("imagens/bullet_3.png");
    ALLEGRO_BITMAP *enemy_sprite = al_load_bitmap("imagens/enemy.png");
    ALLEGRO_BITMAP *enemy_sprite_2 = al_load_bitmap("imagens/enemy_2.png");
    ALLEGRO_BITMAP *shooting_enemy_sprite = al_load_bitmap("imagens/enemyShoot.png");
    ALLEGRO_BITMAP *shooting_enemy_sprite_2 = al_load_bitmap("imagens/enemyShoot_2.png");
    ALLEGRO_BITMAP *enemy_bullet_sprite = al_load_bitmap("imagens/bulletEnemy.png");
    ALLEGRO_BITMAP *boss_sprite = al_load_bitmap("imagens/ship_1.png");
    ALLEGRO_BITMAP *boss_sprite_2 = al_load_bitmap("imagens/ship_6.png");
    ALLEGRO_BITMAP *boss_bullet_sprite = al_load_bitmap("imagens/bulletEnemy.png");
    ALLEGRO_BITMAP *boss_bullet_special = al_load_bitmap("imagens/bullet_boss1.png");
    ALLEGRO_BITMAP *heart_full = al_load_bitmap("imagens/heart_full.png");
    ALLEGRO_BITMAP *heart_null = al_load_bitmap("imagens/heart_null.png");
    ALLEGRO_BITMAP *icon = al_load_bitmap("imagens/icon.png");
    ALLEGRO_BITMAP *explosion_sprite = al_load_bitmap("imagens/frame5.png");
    ALLEGRO_BITMAP *explosion_boss = al_load_bitmap("imagens/frame4.png");
    ALLEGRO_BITMAP *bulletEnemy_boss2 = al_load_bitmap("imagens/bulletEnemy_boss2.png");
    ALLEGRO_BITMAP *sla = al_load_bitmap("imagens/sla.png");
    if (!al_install_audio())
    {
        fprintf(stderr, "Falha ao inicializar o sistema de áudio.\n");
        return -1;
    }

    if (!al_init_acodec_addon())
    {
        fprintf(stderr, "Falha ao inicializar os codecs de áudio.\n");
        return -1;
    }

    if (!al_reserve_samples(1))
    {
        fprintf(stderr, "Falha ao reservar canais de áudio.\n");
        return -1;
    }

    ALLEGRO_AUDIO_STREAM *music = al_load_audio_stream("musica_fundo.ogg", 4, 1024);
    if (!music)
    {
        fprintf(stderr, "Falha ao carregar a música de fundo.\n");
        return -1;
    }
    ALLEGRO_AUDIO_STREAM *music_menu = al_load_audio_stream("musica_menu.ogg", 4, 1024);
    if (!music)
    {
        fprintf(stderr, "Falha ao carregar a música de menu.\n");
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    int game_phase = 1;

    int bullet_sprite_width = al_get_bitmap_width(bullet_sprite);
    int bullet_sprite_height = al_get_bitmap_height(bullet_sprite);

    int bullet_sprite_2_width = al_get_bitmap_width(bullet_sprite_2);
    int bullet_sprite_2_height = al_get_bitmap_height(bullet_sprite_2);
    int enemy_bullet_width = al_get_bitmap_width(enemy_bullet_sprite);
    int enemy_bullet_height = al_get_bitmap_height(enemy_bullet_sprite);

    int continue_game = 0;
    int exit_game = 0;

    Player player;
    Bullet bullets[MAX_BULLETS];
    Enemy enemies[MAX_ENEMIES];
    BossBullet boss_bullets[MAX_BOSS_BULLETS];
    Bullet enemy_bullets[MAX_BULLET_COUNT]; // Adicione essa linha
    ShootingEnemy shooting_enemies[MAX_SHOOTING_ENEMIES];
    Boss boss; // Declaração do chefe
    init_boss(&boss);
    init_shooting_enemies(shooting_enemies, game_phase); // Adicione isso no main
    init_player(&player);
    init_bullets(bullets, MAX_BULLETS);
    init_enemies(enemies, MAX_ENEMIES);
    init_enemy_bullets(enemy_bullets, MAX_BULLET_COUNT); // Adicione essa linha na função main
    init_boss_bullets(boss_bullets, MAX_BOSS_BULLETS);

    ALLEGRO_BITMAP *item_sprite = al_load_bitmap("imagens/item.png");
    ALLEGRO_BITMAP *item_sprite_2 = al_load_bitmap("imagens/item_2.png");
    Item item = {0, 0, false, item_sprite}; // item_sprite é o sprite do item
    Item item_phase2 = {0, 0, false, item_sprite_2};

    ALLEGRO_BITMAP *current_background = background;
    ALLEGRO_BITMAP *current_background_2 = background_2;

    int firing = 0;
    int paused = 0;
    double last_fire_time = 0;
    int score = 0;
    int game_over = 0;
    int player_won = 0; // 0 = jogo em andamento, 1 = jogador ganhou
    int boss_bullet_count = 0;
    float background_x = 0;
    int victory_state = 0; // 0 = sem vitória, 1 = animação de explosão, 2 = tela de vitória
    int restart_game = 0;
    float background_speed = 2.0; // Velocidade padrão do plano de fundo
    int enemy_destroyed_count = 0;
    double boss_start_time = 0; // Armazena o tempo em que a espera para o boss começa
    int boss_waiting = 0;
    double boss_shoot_delay = 0; // Variável para controlar o tempo de ativação do boss
    bool boss_can_shoot = false;
    double boss_shoot_start_time = 0;

    int checkbox_states[3][3] = {{1, 0, 0}, {1, 0, 0}, {1, 0, 0}};

    int explosion_frame = 0;
    float explosion_timer = 0.0;

    const char *explosion_frames[EXPLOSION_FRAME_COUNT] = {
        "imagens/frame1.png",
        "imagens/frame2.png",
        "imagens/frame3.png",
        "imagens/frame4.png",
        "imagens/frame5.png"};

    ALLEGRO_BITMAP *explosion_bitmaps[EXPLOSION_FRAME_COUNT];
    for (int i = 0; i < EXPLOSION_FRAME_COUNT; i++)
    {
        explosion_bitmaps[i] = al_load_bitmap(explosion_frames[i]);
    }

    al_start_timer(timer);
    // Loop do menu
    run_menu(font_menu, background, event_queue, display, music_menu, &game_options, &exit_game);

    // Para parar a música
    al_detach_audio_stream(music_menu);

    // Opcional: destruir a música para liberar memória
    al_destroy_audio_stream(music_menu);
    music_menu = NULL;

    al_flush_event_queue(event_queue);

    double start_time = al_get_time(); // Tempo inicial

    al_set_audio_stream_playmode(music, ALLEGRO_PLAYMODE_LOOP);
    al_attach_audio_stream_to_mixer(music, al_get_default_mixer());

    if (game_options.new_option_2 == 0)
    {
        current_background = background; // Use o fundo 2
    }
    else if (game_options.new_option_2 == 1)
    {
        current_background = background_2; // Use um fundo adicional (defina-o antes)
    }
    else if (game_options.new_option_2 == 2)
    {
        current_background = background_3;
    }

    if (game_options.new_option_3 == 0)
    {
        current_background_2 = background; // Use o fundo 2
    }
    else if (game_options.new_option_3 == 1)
    {
        current_background_2 = background_2; // Use um fundo adicional (defina-o antes)
    }
    else if (game_options.new_option_3 == 2)
    {
        current_background_2 = background_3;
    }

    while (1)
    {

        if (exit_game == 1)
        {
            break; // Finaliza o jogo
        }

        int redraw = 0;
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        double current_time = al_get_time(); // Tempo atual

        double elapsed_time = al_get_time() - start_time;
        double remaining_time = TIME_TO_BOSS - elapsed_time;

        if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            if (!game_over)
            {
                background_x -= background_speed;
                // Controla a velocidade de rolagem do fundo
                if (background_x <= -al_get_bitmap_width(current_background))
                {
                    background_x = 0;
                }
                if (!game_over && !player.paused) // Verificar se o jogo não está pausado
                {
                    draw_timer(font, elapsed_time);

                    static double phase2_start_time = -1.0;
                    static bool phase2_started = false;

                    if (remaining_time > 0)
                    {
                        if (game_phase == 1)
                        {
                            if (rand() % 200 == 0)
                            {
                                generate_shooting_enemy(shooting_enemies, MAX_SHOOTING_ENEMIES, game_phase);
                            }

                            if (rand() % 60 == 0)
                            {
                                generate_enemy(enemies, MAX_ENEMIES, player.width, player.height);
                            }
                        }
                        else if (game_phase == 2)
                        {
                            // Inicializa o tempo quando a fase 2 começa
                            if (!phase2_started)
                            {
                                phase2_start_time = al_get_time();
                                phase2_started = true;
                            }

                            // Verifica se passou 1 segundo desde o início da fase 2
                            if (al_get_time() - phase2_start_time >= 1.0)
                            {
                                // Gerar inimigos após 1 segundo
                                if (rand() % 100 == 0)
                                {
                                    generate_shooting_enemy(shooting_enemies, MAX_SHOOTING_ENEMIES, game_phase);
                                }

                                if (rand() % 30 == 0)
                                {
                                    generate_enemy(enemies, MAX_ENEMIES, player.width, player.height);
                                }
                            }
                        }

                        move_enemies(enemies, MAX_ENEMIES, game_phase);
                        check_collisions(&player, bullets, MAX_BULLETS, enemies, MAX_ENEMIES, shooting_enemies, MAX_SHOOTING_ENEMIES, &item, &item_phase2, &score, &game_over, &enemy_destroyed_count, game_phase);

                        for (int i = 0; i < MAX_SHOOTING_ENEMIES; i++)
                        {
                            move_shooting_enemy(&shooting_enemies[i]);                                // Mover todos os inimigos que atiram
                            shoot_enemy_bullet(&shooting_enemies[i], player.x, player.y, game_phase); // Cada um atira
                            move_enemy_bullets(shooting_enemies[i].bullets, 3);                       // Move os projéteis
                            check_enemy_bullet_collisions(&player, &shooting_enemies[i], &game_over); // Verifica colisões
                            // check_shooting_enemy_collision(&player, &shooting_enemies[i], &game_over);
                        }

                        // for (int i = 0; i < MAX_SHOOTING_ENEMIES; i++)
                        // {
                        //     check_shooting_enemy_collision(&player, &shooting_enemies[i], &game_over);
                        // }
                    }

                    // Verifique se o boss deve começar a esperar
                    if (remaining_time <= 0 && !boss_waiting && !boss.active)
                    {
                        boss_waiting = 1;               // Marca que estamos aguardando
                        boss_start_time = current_time; // Registra o tempo inicial da espera
                    }

                    // Aguarda os 3 segundos antes de ativar o boss
                    if (boss_waiting && (current_time - boss_start_time >= 2.0))
                    {
                        boss_waiting = 0;                     // Reseta o estado de espera
                        boss.active = 1;                      // Ativa o boss
                        boss.x = SCREEN_WIDTH - boss.width;   // Posiciona o boss
                        boss_shoot_start_time = current_time; // Define o tempo inicial para começar a disparar
                    }

                    if (boss.active)
                    {
                        if (game_phase == 1)
                        {
                            item.active = false;
                            // Aguarda 2 segundos antes de o boss começar a disparar
                            if (current_time - boss_shoot_start_time >= 1.0)
                            {
                                // Verificar colisões entre balas do jogador e o chefe
                                shoot_boss_bullet(&boss, boss_bullets, &boss_bullet_count, game_phase);
                                move_boss_bullets(boss_bullets, MAX_BOSS_BULLETS);
                                check_boss_bullet_collisions(&player, &boss, boss_bullets, &game_over, game_phase);
                            }
                        }
                        else
                        {
                            item_phase2.active = false;
                            shoot_boss_bullet(&boss, boss_bullets, &boss_bullet_count, game_phase);
                            move_boss_bullets(boss_bullets, MAX_BOSS_BULLETS);
                            check_boss_bullet_collisions(&player, &boss, boss_bullets, &game_over, game_phase);
                        }

                        // Checar colisão direta com o boss
                        check_boss_collision(&player, bullets, MAX_BULLETS, &boss, &score, &player_won, &game_over, game_phase);
                    }

                    if (player_won && victory_state == 0)
                    {
                        // Reproduzir a animação de explosão do boss derrotado
                        // play_explosion(boss.x, boss.y, explosion_bitmaps, current_background);
                        // al_rest(0.5);

                        // Configurar variáveis para o menu de transição

                        if (game_phase < 2)
                        {
                            show_transition_menu(display, event_queue, font_menu, &continue_game, &exit_game);
                        }
                        else
                        {
                            show_victory_message(font_menu, &exit_game);
                        }
                        // Checar a escolha do jogador
                        if (exit_game)
                        {
                            break; // Finaliza o jogo
                        }
                        else if (continue_game)
                        {

                            // Reiniciar o jogo para a segunda fase ou configurar para próxima fase
                            init_second_phase(&player, enemies, bullets, shooting_enemies, &boss, &victory_state, &player_won, &start_time, &enemy_destroyed_count, game_phase);
                            game_phase = 2;
                            item_phase2.active = false; // Assegura que o item da fase 2 está desativado quando a fase 2 começa

                            player_won = 0; // Reinicia a condição de vitória
                        }
                    }

                    move_player(&player);
                    move_bullets(bullets, MAX_BULLETS, &player, font_menu, game_phase);

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
                background_speed = 0.5;
                break;
            case ALLEGRO_KEY_D:
                player.joystick.right = 1;
                background_speed = 4.0;
                break;
            case ALLEGRO_KEY_ENTER:
                player.joystick.fire = 1;
                break;
            case ALLEGRO_KEY_P: // Aperte P para pausar/despausar
                player.paused = !player.paused;
                if (player.paused)
                {
                    draw_pause_message(font_menu, current_background, event_queue, display, &player, &exit_game);
                }
                break;
            }
        }
        else if (game_over)
        {
            // Exibe o menu de fim de jogo
            show_game_over_menu(display, event_queue, font_menu, &restart_game, &exit_game);

            if (exit_game)
            {
                // Se o jogador escolher sair, fecha o jogo
                break;
            }
            else if (restart_game)
            {
                // Se o jogador escolher reiniciar, reinicia o jogo
                // Reinicializa os elementos do jogo
                init_player(&player);
                init_boss(&boss);
                init_enemies(enemies, MAX_ENEMIES);
                init_bullets(bullets, MAX_BULLETS);

                player.invulnerable = 1;
                player.invulnerable_time = al_get_time();

                // Zera o estado do jogo e reinicia o loop principal
                game_over = 0;
                player_won = 0;
                score = 0;
                start_time = al_get_time();
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
                background_speed = 2.0;
                break;
            case ALLEGRO_KEY_D:
                player.joystick.right = 0;
                background_speed = 2.0;
                break;
            case ALLEGRO_KEY_ENTER:
                player.joystick.fire = 0;
                break;
            }
        }

        if (player.paused)
        {
            draw_pause_message(font_menu, current_background, event_queue, display, &player, &exit_game); // Mostra a mensagem de pausa
        }
        if (exit_game)
        {
            break; // Finaliza o jogo
        }

        if (redraw && al_is_event_queue_empty(event_queue))
        {
            if (game_phase == 1)
            {
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_bitmap(current_background, background_x, 0, 0);
                al_draw_bitmap(current_background, background_x + al_get_bitmap_width(current_background), 0, 0);
            }
            else if (game_phase == 2)
            {
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_bitmap(current_background_2, background_x, 0, 0);
                al_draw_bitmap(current_background_2, background_x + al_get_bitmap_width(current_background_2), 0, 0);
            }
            // Desenhar o jogador com efeito de piscamento se estiver invulnerável

            if (player.invulnerable && ((int)(al_get_time() * 10) % 2 == 0))
            {
                // Verifica qual sprite foi escolhido no menu
                switch (game_options.sprite_option)
                {
                case 0: // Opção de sprite 1
                    if (player.joystick.down)
                    {
                        al_draw_bitmap(player_sprite_dir, player.x, player.y, 0); // Desenha a sprite para a direita
                    }
                    else if (player.joystick.up)
                    {
                        al_draw_bitmap(player_sprite_esq, player.x, player.y, 0); // Desenha a sprite para a esquerda
                    }
                    else
                    {
                        al_draw_bitmap(player_sprite, player.x, player.y, 0); // Desenha a sprite padrão
                    }
                    break;
                case 1: // Opção de sprite 2
                    if (player.joystick.down)
                    {
                        al_draw_bitmap(player_sprite_dir_dif1, player.x, player.y, 0); // Desenha o novo sprite para a direita
                    }
                    else if (player.joystick.up)
                    {
                        al_draw_bitmap(player_sprite_esq_dif1, player.x, player.y, 0); // Desenha o novo sprite para a esquerda
                    }
                    else
                    {
                        al_draw_bitmap(player_sprite_dif1, player.x, player.y, 0); // Desenha o novo sprite padrão
                    }
                    break;
                case 2: // Opção de sprite 3
                    if (player.joystick.down)
                    {
                        al_draw_bitmap(player_sprite_dir_dif2, player.x, player.y, 0); // Desenha o terceiro sprite para a direita
                    }
                    else if (player.joystick.up)
                    {
                        al_draw_bitmap(player_sprite_esq_dif2, player.x, player.y, 0); // Desenha o terceiro sprite para a esquerda
                    }
                    else
                    {
                        al_draw_bitmap(player_sprite_dif2, player.x, player.y, 0); // Desenha o terceiro sprite padrão
                    }
                    break;
                }
            }
            else if (!player.invulnerable)
            {
                // Verifica qual sprite foi escolhido no menu
                switch (game_options.sprite_option)
                {
                case 0: // Opção de sprite 1
                    if (player.joystick.down)
                    {
                        al_draw_bitmap(player_sprite_dir, player.x, player.y, 0); // Desenha a sprite para a direita
                    }
                    else if (player.joystick.up)
                    {
                        al_draw_bitmap(player_sprite_esq, player.x, player.y, 0); // Desenha a sprite para a esquerda
                    }
                    else
                    {
                        al_draw_bitmap(player_sprite, player.x, player.y, 0); // Desenha a sprite padrão
                    }
                    break;
                case 1: // Opção de sprite 2
                    if (player.joystick.down)
                    {
                        al_draw_bitmap(player_sprite_dir_dif1, player.x, player.y, 0); // Desenha o novo sprite para a direita
                    }
                    else if (player.joystick.up)
                    {
                        al_draw_bitmap(player_sprite_esq_dif1, player.x, player.y, 0); // Desenha o novo sprite para a esquerda
                    }
                    else
                    {
                        al_draw_bitmap(player_sprite_dif1, player.x, player.y, 0); // Desenha o novo sprite padrão
                    }
                    break;
                case 2: // Opção de sprite 3
                    if (player.joystick.down)
                    {
                        al_draw_bitmap(player_sprite_dir_dif2, player.x, player.y, 0); // Desenha o terceiro sprite para a direita
                    }
                    else if (player.joystick.up)
                    {
                        al_draw_bitmap(player_sprite_esq_dif2, player.x, player.y, 0); // Desenha o terceiro sprite para a esquerda
                    }
                    else
                    {
                        al_draw_bitmap(player_sprite_dif2, player.x, player.y, 0); // Desenha o terceiro sprite padrão
                    }
                    break;
                }
            }

            // Chamar a função para desenhar a vida do jogador
            draw_player_life(heart_full, heart_null, &player);

            // Desenhar as balas
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (bullets[i].active)
                {
                    // Desenhando o retângulo em volta da hitbox

                    // Desenhando a bala
                    if (game_phase == 1)
                    {
                        if (player.special_attack_active == true)
                        {
                            al_draw_bitmap(bullet_sprite_2, bullets[i].x + 15, bullets[i].y + 10, 0);
                        }
                        else
                        {
                            if (game_options.new_option_1 == 0)
                            {
                                al_draw_bitmap(bullet_sprite, bullets[i].x + 15, bullets[i].y + 10, 0);
                            }
                            else if (game_options.new_option_1 == 1)
                            {
                                al_draw_bitmap(bullet_sprite_dif1, bullets[i].x + 15, bullets[i].y + 10, 0);
                            }
                            else if (game_options.new_option_1 == 2)
                            {
                                al_draw_bitmap(bullet_sprite_dif2, bullets[i].x + 15, bullets[i].y + 10, 0);
                            }
                        }
                    }
                    else if (game_phase == 2)
                    {
                        if (player.special_attack_active == true)
                        {
                            al_draw_bitmap(bullet_sprite_3, bullets[i].x + 15, bullets[i].y + 10, 0);
                        }
                        else
                        {
                            if (game_options.new_option_1 == 0)
                            {
                                al_draw_bitmap(bullet_sprite, bullets[i].x + 15, bullets[i].y + 10, 0);
                            }
                            else if (game_options.new_option_1 == 1)
                            {
                                al_draw_bitmap(bullet_sprite_dif1, bullets[i].x + 15, bullets[i].y + 10, 0);
                            }
                            else if (game_options.new_option_1 == 2)
                            {
                                al_draw_bitmap(bullet_sprite_dif2, bullets[i].x + 15, bullets[i].y + 10, 0);
                            }
                        }
                    }
                }
            }

            if (remaining_time > 0)
            {
                float last_time = al_get_time();
                float delta_time = al_get_time() - last_time;
                // Desenhar inimigos
                for (int i = 0; i < MAX_ENEMIES; i++)
                {
                    if (enemies[i].active)
                        if (game_phase == 1)
                        {

                            al_draw_bitmap(enemy_sprite, enemies[i].x, enemies[i].y + 0, 0);
                        }
                        else if (game_phase == 2)
                        {

                            al_draw_bitmap(enemy_sprite_2, enemies[i].x, enemies[i].y, 0);
                        }

                    draw_explosion(&enemies[i], explosion_sprite, 0.1);
                }

                if (item.active)
                {
                    al_draw_bitmap(item.sprite, item.x, item.y, 0); // Desenha o item no lugar onde ele foi gerado
                }

                if (game_phase == 1 && player.special_attack_active == true)
                {
                    al_draw_text(font_warn, al_map_rgb(255, 255, 255), 400, 550, ALLEGRO_ALIGN_CENTRE, "Voce pegou disparos rapidos por 6 segundos!");
                }
                else if (game_phase == 2 && player.special_attack_active == true)
                {
                    al_draw_text(font_warn, al_map_rgb(255, 255, 255), 400, 550, ALLEGRO_ALIGN_CENTRE, "Voce pegou disparos com 2x de dano por 6 segundos!");
                }

                // Renderização dos inimigos que atiram
                for (int i = 0; i < MAX_SHOOTING_ENEMIES; i++)
                {
                    ShootingEnemy shooting_enemy = shooting_enemies[i]; // Obter o inimigo atual

                    if (shooting_enemy.active)
                    {
                        if (game_phase == 1)
                        {
                            if (!shooting_enemy.ready_to_shoot)
                            {
                                shooting_enemy.ready_to_shoot = true;
                            }

                            al_draw_bitmap(shooting_enemy_sprite, shooting_enemy.x, shooting_enemy.y, 0); // Desenhar o inimigo
                        }
                        else if (game_phase == 2)
                        {
                            if (!shooting_enemy.ready_to_shoot)
                            {
                                shooting_enemy.ready_to_shoot = true;
                            }

                            al_draw_bitmap(shooting_enemy_sprite_2, shooting_enemy.x, shooting_enemy.y, 0); // Desenhar o inimigo
                        }

                        // Renderizar os projéteis do inimigo
                        for (int j = 0; j < 10; j++) // Substitua 10 pelo número máximo de projéteis que cada inimigo pode ter
                        {
                            if (shooting_enemy.bullets[j].active)
                            {
                                // Desenhando o retângulo em volta da hitbox do projétil do inimigo

                                // Desenhando o projétil do inimigo
                                al_draw_bitmap(enemy_bullet_sprite, shooting_enemy.bullets[j].x - 30, shooting_enemy.bullets[j].y + 20, 0);
                            }
                        }
                        draw_explosion_shoot(&shooting_enemies[i], explosion_sprite, 0.1);
                    }
                }
            }

            if (item.active)
            {

                al_draw_bitmap(item.sprite, item.x, item.y, 0); // Desenha o item no lugar onde ele foi gerado
            }
            if (item_phase2.active)
            {

                al_draw_bitmap(item_phase2.sprite, item_phase2.x, item_phase2.y, 0);
            }

            if (boss.active)
            {
                for (int i = 0; i < MAX_BOSS_BULLETS; i++)
                {
                    if (boss_bullets[i].active)
                    {
                        if (game_phase == 2)
                        {
                            if (boss.special_attack_active == 1)
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
                move_boss(&boss, game_phase);
                if (game_phase == 1)
                {

                    al_draw_bitmap(boss_sprite, boss.x, boss.y, 0); // Desenhar o inimigo
                }
                else if (game_phase == 2)
                {

                    al_draw_bitmap(boss_sprite_2, boss.x, boss.y, 0); // Desenhar o inimigo
                }
                draw_explosion_boss(&boss, explosion_boss, 0.1);
            }

            // Desenhar o placar
            al_draw_textf(font_info, al_map_rgb(255, 255, 255), 680, 10, 0, "Score: %d", score);
            al_draw_bitmap(icon, 0, 0, 0);
            al_draw_textf(font_info, al_map_rgb(255, 255, 255), 90, 50, 0, "Player 1");
            // al_draw_textf(font, al_map_rgb(255, 0, 0), SCREEN_WIDTH - 100, 10, 0, "Time: %.2f", remaining_time);

            al_flip_display();
        }
    }

    cleanup_resources(display, event_queue, timer, font, font_menu, font_warn, font_info, background, background_2,
                      background_3, player_sprite, player_sprite_dir, player_sprite_esq, player_sprite_dif1,
                      player_sprite_dir_dif1, player_sprite_esq_dif1, player_sprite_dif2, player_sprite_dir_dif2,
                      player_sprite_esq_dif2, bullet_sprite, bullet_sprite_2, bullet_sprite_3, bullet_sprite_dif1,
                      bullet_sprite_dif2, enemy_sprite, enemy_sprite_2, shooting_enemy_sprite, shooting_enemy_sprite_2,
                      enemy_bullet_sprite, boss_sprite, boss_sprite_2, boss_bullet_sprite, boss_bullet_special,
                      heart_full, heart_null, icon, explosion_sprite, explosion_boss, item_sprite, item_sprite_2,
                      bulletEnemy_boss2, sla);

    al_destroy_audio_stream(music);
    al_destroy_audio_stream(music_menu);
    al_uninstall_audio();
    al_shutdown_font_addon();
    al_shutdown_ttf_addon();
    al_shutdown_image_addon();
    al_uninstall_system();

    return 0;
}
