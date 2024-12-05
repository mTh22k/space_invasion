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
#include "defines.h"
#include "render.h"

int main()
{
    // Inicializa os addons e componentes principais do Allegro.
    al_init();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(1);

    // Cria a janela principal do jogo com resolução 800x600.
    ALLEGRO_DISPLAY *display = al_create_display(800, 600);

    // Habilita o uso do teclado para receber eventos.
    al_install_keyboard();

    // Criação da fila de eventos para gerenciar entradas e saídas do sistema.
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();

    // Configura o temporizador do jogo para 50 FPS.
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 50);

    // fonts
    ALLEGRO_FONT *font = al_load_ttf_font("fonts/ATC.ttf", 12, 0);
    ALLEGRO_FONT *font_menu = al_load_ttf_font("fonts/menu_f.ttf", 50, 0);
    ALLEGRO_FONT *font_warn = al_load_ttf_font("fonts/menu_f.ttf", 12, 0);
    ALLEGRO_FONT *font_info = al_load_ttf_font("fonts/menu_f.ttf", 20, 0);

    // backgrounds
    ALLEGRO_BITMAP *background = al_load_bitmap("imagens/background.png");
    ALLEGRO_BITMAP *background_2 = al_load_bitmap("imagens/background_2.png");
    ALLEGRO_BITMAP *background_3 = al_load_bitmap("imagens/background_3.png");
    ALLEGRO_BITMAP *current_background = background;
    ALLEGRO_BITMAP *current_background_2 = background_2;

    // player
    ALLEGRO_BITMAP *player_sprite = al_load_bitmap("imagens/nave.png");
    ALLEGRO_BITMAP *player_sprite_dir = al_load_bitmap("imagens/nave_dir.png");
    ALLEGRO_BITMAP *player_sprite_esq = al_load_bitmap("imagens/nave_esq.png");
    ALLEGRO_BITMAP *player_sprite_dif1 = al_load_bitmap("imagens/nave1.png");
    ALLEGRO_BITMAP *player_sprite_dir_dif1 = al_load_bitmap("imagens/nave_1_dir.png");
    ALLEGRO_BITMAP *player_sprite_esq_dif1 = al_load_bitmap("imagens/nave_1_esq.png");
    ALLEGRO_BITMAP *player_sprite_dif2 = al_load_bitmap("imagens/nave2.png");
    ALLEGRO_BITMAP *player_sprite_dir_dif2 = al_load_bitmap("imagens/nave_2_dir.png");
    ALLEGRO_BITMAP *player_sprite_esq_dif2 = al_load_bitmap("imagens/nave_2_esq.png");

    // disparos player
    ALLEGRO_BITMAP *bullet_sprite_dif1 = al_load_bitmap("imagens/bullet_dif1.png");
    ALLEGRO_BITMAP *bullet_sprite_dif2 = al_load_bitmap("imagens/bullet_dif2.png");
    ALLEGRO_BITMAP *bullet_sprite = al_load_bitmap("imagens/bullet.png");
    ALLEGRO_BITMAP *bullet_sprite_2 = al_load_bitmap("imagens/bullet_2.png");
    ALLEGRO_BITMAP *bullet_sprite_3 = al_load_bitmap("imagens/bullet_3.png");

    // inimigos
    ALLEGRO_BITMAP *enemy_sprite = al_load_bitmap("imagens/enemy.png");
    ALLEGRO_BITMAP *enemy_sprite_2 = al_load_bitmap("imagens/enemy_2.png");
    ALLEGRO_BITMAP *shooting_enemy_sprite = al_load_bitmap("imagens/enemyShoot.png");
    ALLEGRO_BITMAP *shooting_enemy_sprite_2 = al_load_bitmap("imagens/enemyShoot_2.png");
    ALLEGRO_BITMAP *enemy_bullet_sprite = al_load_bitmap("imagens/bulletEnemy.png");
    ALLEGRO_BITMAP *explosion_sprite = al_load_bitmap("imagens/frame5.png");

    // boss
    ALLEGRO_BITMAP *boss_sprite = al_load_bitmap("imagens/ship_1.png");
    ALLEGRO_BITMAP *boss_sprite_2 = al_load_bitmap("imagens/ship_6.png");
    ALLEGRO_BITMAP *boss_bullet_sprite = al_load_bitmap("imagens/bulletEnemy.png");
    ALLEGRO_BITMAP *boss_bullet_special = al_load_bitmap("imagens/bullet_boss1.png");
    ALLEGRO_BITMAP *bulletEnemy_boss2 = al_load_bitmap("imagens/bulletEnemy_boss2.png");
    ALLEGRO_BITMAP *explosion_boss = al_load_bitmap("imagens/frame4.png");

    // icones e vidas
    ALLEGRO_BITMAP *heart_full = al_load_bitmap("imagens/heart_full.png");
    ALLEGRO_BITMAP *heart_null = al_load_bitmap("imagens/heart_null.png");
    ALLEGRO_BITMAP *icon = al_load_bitmap("imagens/icon.png");

    // itens
    ALLEGRO_BITMAP *item_sprite = al_load_bitmap("imagens/item.png");
    ALLEGRO_BITMAP *item_sprite_2 = al_load_bitmap("imagens/item_2.png");

    // audio
    ALLEGRO_AUDIO_STREAM *music = al_load_audio_stream("musicas/musica_fundo.ogg", 4, 1024);
    ALLEGRO_AUDIO_STREAM *music_menu = al_load_audio_stream("musicas/musica_menu.ogg", 4, 1024);

    // historia
    ALLEGRO_BITMAP *slide_1 = al_load_bitmap("imagens/img1.png");
    ALLEGRO_BITMAP *slide_2 = al_load_bitmap("imagens/img2.png");
    ALLEGRO_BITMAP *slide_3 = al_load_bitmap("imagens/img3.png");

    // eventos na fila
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    // declarações
    Player player;
    Bullet bullets[MAX_BULLETS];
    Enemy enemies[MAX_ENEMIES];
    BossBullet boss_bullets[MAX_BOSS_BULLETS];
    Bullet enemy_bullets[MAX_BULLET_COUNT];
    ShootingEnemy shooting_enemies[MAX_SHOOTING_ENEMIES];
    Boss boss;
    GameOptions game_options;
    Item item = {0, 0, false, item_sprite, 40, 15};
    Item item_phase2 = {0, 0, false, item_sprite_2, 40, 15};
    ALLEGRO_BITMAP *backgrounds[] = {background, background_2, background_3};

    // variaveis
    int paused = 0;
    double last_fire_time = 0;
    int score = 0;
    int game_over = 0;
    int player_won = 0;
    int boss_bullet_count = 0;
    float background_x = 0;
    int victory_state = 0;
    int restart_game = 0;
    float background_speed = 2.0;
    int enemy_destroyed_count = 0;
    double boss_start_time = 0;
    bool boss_waiting = false;
    double boss_shoot_start_time = 0;
    int game_phase = 1;
    int continue_game = 0;
    int exit_game = 0;
    int redraw = 0;
    int history_mode = 1;
    int checkbox_states[3][3] = {{1, 0, 0}, {1, 0, 0}, {1, 0, 0}};

    // inicializações
    init_boss(&boss);
    init_shooting_enemies(shooting_enemies, game_phase);
    init_player(&player);
    init_bullets(bullets, MAX_BULLETS);
    init_enemies(enemies, MAX_ENEMIES);
    init_enemy_bullets(enemy_bullets, MAX_BULLET_COUNT);
    init_boss_bullets(boss_bullets, MAX_BOSS_BULLETS);
    init_options(&game_options);
    al_start_timer(timer);

    // Loop do menu principal
    run_menu(font_menu, background, event_queue, display, music_menu, &game_options, &exit_game);

    // Desanexa e destrói o stream de áudio da música do menu
    al_detach_audio_stream(music_menu);
    al_destroy_audio_stream(music_menu);
    music_menu = NULL;

    // Limpa fila e começa o tempo
    al_flush_event_queue(event_queue);
    double start_time = al_get_time();

    // Define o modo de repetição da música de fundo do jogo
    al_set_audio_stream_playmode(music, ALLEGRO_PLAYMODE_LOOP);
    al_attach_audio_stream_to_mixer(music, al_get_default_mixer());

    // Atualiza o background do jogo
    update_backgrounds(game_options, backgrounds, &current_background, &current_background_2);

    // Inicia o loop principal do jogo
    while (1)
    {
        // Verifica se o jogador deseja sair do jogo
        if (exit_game == 1)
        {
            break;
        }

        // Verifica se o jogo está no modo historia
        if (history_mode == 1)
        {
            start_history_slide(display, event_queue, timer, slide_1, slide_2, slide_3, &exit_game, &history_mode);
            // Inicia o modo de exibição de slides e depois o jogo
        }

        ALLEGRO_EVENT ev;
        // Aguarda até que um evento aconteça na fila de eventos
        al_wait_for_event(event_queue, &ev);

        // Obtém o tempo atual
        double current_time = al_get_time();
        // Tempo decorrido desde o início do jogo
        double elapsed_time = al_get_time() - start_time;
        // Tempo restante até o boss aparecer
        double remaining_time = TIME_TO_BOSS - elapsed_time;

        if (ev.type == ALLEGRO_EVENT_TIMER)
        {

            if (game_over)
            {
                // Exibe o menu de fim de jogo
                show_game_over_menu(display, event_queue, font_menu, &restart_game, &exit_game);

                // Se o jogador escolher reiniciar o jogo
                if (restart_game)
                {
                    item_phase2.active = false;
                    item.active = false;
                    restart_init_game(&player, &boss, enemies, MAX_ENEMIES, bullets, MAX_BULLETS, &game_over, &player_won, &score, &start_time, &enemy_destroyed_count);
                }
            }
            // Se o jogo não acabou
            if (!game_over)
            {
                // Desloca o fundo para a esquerda
                background_x -= background_speed;

                // Controla a velocidade de rolagem do fundo
                if (background_x <= -al_get_bitmap_width(current_background))
                {
                    // Reseta a posição do fundo
                    background_x = 0;
                }

                // Verifica se o jogo não está pausado
                if (!game_over && !player.paused)
                {

                    static double phase2_start_time = -1.0;
                    static bool phase2_started = false;

                    // Se ainda há tempo restante para o boss aparecer
                    if (remaining_time > 0)
                    {
                        if (boss.active == 0)
                        {
                            // Se estamos na fase 1 do jogo
                            if (game_phase == 1)
                            {
                                    handle_phase1_enemy_generation(shooting_enemies, MAX_SHOOTING_ENEMIES,
                                                            enemies, MAX_ENEMIES, game_phase, player);
                                // Gera inimigos na fase 1
                            }
                            // Se estamos na fase 2 do jogo
                            else if (game_phase == 2)
                            {
                                    handle_phase2_enemy_generation(shooting_enemies, MAX_SHOOTING_ENEMIES,
                                                            enemies, MAX_ENEMIES, game_phase, player,
                                                            &phase2_start_time, &phase2_started);
                                // Gera inimigos na fase 2
                            }
                        }

                        // Move os inimigos
                        move_enemies(enemies, MAX_ENEMIES, game_phase);
                        // Verifica colisões entre jogador, inimigos, e projéteis
                        check_collisions(&player, bullets, MAX_BULLETS, enemies, MAX_ENEMIES, shooting_enemies, MAX_SHOOTING_ENEMIES, &item, &item_phase2, &score, &game_over, &enemy_destroyed_count, game_phase);

                        // Para cada inimigo que atira
                        for (int i = 0; i < MAX_SHOOTING_ENEMIES; i++)
                        {
                            // Mover todos os inimigos que atiram
                            move_shooting_enemy(&shooting_enemies[i]);

                            // Verifica se o inimigo está visível antes de atirar
                            if (shooting_enemies[i].x + shooting_enemies[i].width >= 0 &&
                                shooting_enemies[i].x <= SCREEN_WIDTH &&
                                shooting_enemies[i].y + shooting_enemies[i].height >= 0 &&
                                shooting_enemies[i].y <= SCREEN_HEIGHT)
                            {

                                // Permite disparo apenas se o inimigo está na tela
                                shoot_enemy_bullet(&shooting_enemies[i], player.x, player.y, game_phase);
                            }

                            // Move os projéteis
                            move_enemy_bullets(shooting_enemies[i].bullets, 3);

                            // Verifica colisões dos projéteis com o jogador
                            check_enemy_bullet_collisions(&player, &shooting_enemies[i], &game_over);
                        }
                    }

                    // Verifica e ativa o boss, se necessário
                    check_and_activate_boss(&boss, &boss_start_time, current_time,
                                            &boss_shoot_start_time, &boss_waiting,
                                            remaining_time, SCREEN_WIDTH);

                    for (int i = 0; i < MAX_BOSS_BULLETS; i++)
                    {
                        if (player.lives <= 0)
                        {
                            boss_bullets[i].active = 0;
                        }
                    }

                        // Se o boss está ativo
                        if (boss.active)
                        {
                            // Se estamos na fase 1
                            if (game_phase == 1)
                            {
                                item.active = false;
                                // Aguarda 2 segundos antes de o boss começar a disparar
                                if (current_time - boss_shoot_start_time >= 1.0)
                                {
                                    // Verificar colisões entre balas do jogador e o chefe
                                    shoot_boss_bullet(&boss, boss_bullets, &boss_bullet_count, game_phase);
                                    move_boss_bullets(boss_bullets, MAX_BOSS_BULLETS, &player);
                                    check_boss_bullet_collisions(&player, &boss, boss_bullets, &game_over, game_phase);
                                }
                            }
                            else
                            {
                                // colisões e movimentação fase 2
                                item_phase2.active = false;
                                shoot_boss_bullet(&boss, boss_bullets, &boss_bullet_count, game_phase);
                                move_boss_bullets(boss_bullets, MAX_BOSS_BULLETS, &player);
                                check_boss_bullet_collisions(&player, &boss, boss_bullets, &game_over, game_phase);
                            }

                            // Checar colisão direta com o boss
                            check_boss_collision(&player, bullets, MAX_BULLETS, &boss, &score, &player_won, &game_over, game_phase);
                        }

                        // Se o jogador venceu e a fase de vitória não foi ativada ainda
                        if (player_won && victory_state == 0)
                        {
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
                                break;
                            }
                            else if (continue_game)
                            {
                                // Reiniciar o jogo para a segunda fase ou configurar para próxima fase
                                init_second_phase(&player, enemies, bullets, shooting_enemies, &boss, &victory_state, &player_won, &start_time, &enemy_destroyed_count, game_phase);
                                game_phase = 2;
                                item_phase2.active = false;
                                player_won = 0;
                            }
                        }

                        // Move o jogador
                        move_player(&player);
                        // Move as balas
                        move_bullets(bullets, MAX_BULLETS, &player, font_menu, game_phase);

                        // Lógica de invulnerabilidade
                        if (player.invulnerable && (al_get_time() - player.invulnerable_time) > INVULNERABILITY_TIME)
                            player.invulnerable = 0;

                        // Verifica se o jogador pressionou o botão de disparo
                        if (player.joystick.fire && (al_get_time() - last_fire_time) > FIRE_INTERVAL)
                        {
                            fire_bullet(bullets, MAX_BULLETS, player.x + player.width, player.y + player.height / 2);
                            last_fire_time = al_get_time();
                        }
                    }

                // Indica que a tela precisa ser redesenhada
                redraw = 1;
            }
        }
        // Se o evento for fechar a tela
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;
        // Se o evento for pressionamento de tecla
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            handle_keyboard_event(ev, &player, &background_speed, font_menu, current_background, event_queue, display, &exit_game);
        }
        // Se o evento for soltura de tecla
        else if (ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            handle_keyboard_release_event(ev, &player, &background_speed);
        }
        // Se o jogo acabou

        // Se o jogador deseja sair do jogo
        if (exit_game)
        {
            break;
        }

        // Se precisar redesenhar a tela
        if (redraw && al_is_event_queue_empty(event_queue))
        {
            // Se estamos na fase 1
            if (game_phase == 1)
            {
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_bitmap(current_background, background_x, 0, 0);
                // Desenha o fundo na posição x atual
                al_draw_bitmap(current_background, background_x + al_get_bitmap_width(current_background), 0, 0);
                // Desenha o fundo repetido ao lado do fundo atual
            }
            // Se estamos na fase 2
            else if (game_phase == 2)
            {
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_bitmap(current_background_2, background_x, 0, 0);
                // Desenha o fundo da fase 2 na posição x atual
                al_draw_bitmap(current_background_2, background_x + al_get_bitmap_width(current_background_2), 0, 0);
                // Desenha o fundo da fase 2 repetido ao lado do fundo atual
            }

            // Desenha o jogador com diferentes sprites)
            draw_player_with_effects(&player, &game_options,
                                     player_sprite, player_sprite_dif1, player_sprite_dif2,
                                     player_sprite_dir, player_sprite_dir_dif1, player_sprite_dir_dif2,
                                     player_sprite_esq, player_sprite_esq_dif1, player_sprite_esq_dif2);

            // Chama a função para desenhar a vida do jogador
            draw_player_life(heart_full, heart_null, &player);

            // Desenha as balas na tela
            draw_bullets(bullets, MAX_BULLETS, game_phase, &player, &game_options,
                         bullet_sprite, bullet_sprite_2, bullet_sprite_3,
                         bullet_sprite_dif1, bullet_sprite_dif2);

            // Se há inimigos na tela e o tempo restante para o boss
            if (remaining_time > 0)
            {
                float last_time = al_get_time();
                // Obtém o tempo atual para controle de animação
                float delta_time = al_get_time() - last_time;
                // Calcula a diferença de tempo para animações

                // Desenha os inimigos na tela
                draw_enemies(enemies, MAX_ENEMIES, game_phase, enemy_sprite, enemy_sprite_2, explosion_sprite);

                // Desenha o item e o aviso para o jogador
                draw_item_and_warning(&item, &player, game_phase, font_warn);

                // Renderiza os inimigos que atiram
                draw_shooting_enemies(shooting_enemies, MAX_SHOOTING_ENEMIES,
                                      shooting_enemy_sprite, shooting_enemy_sprite_2,
                                      enemy_bullet_sprite, explosion_sprite,
                                      game_phase);
            }

            // Se o item da fase 1 está ativo, desenha o item
            if (item.active)
            {
                al_draw_bitmap(item.sprite, item.x, item.y, 0);
            }

            // Se o item da fase 2 está ativo, desenha o item
            if (item_phase2.active)
            {
                al_draw_bitmap(item_phase2.sprite, item_phase2.x, item_phase2.y, 0);
            }

            if (boss.active)
            {
                // Desenha o boss e seus projéteis
                draw_boss(&boss, boss_bullets, MAX_BOSS_BULLETS,
                          boss_sprite, boss_sprite_2,
                          boss_bullet_sprite, bulletEnemy_boss2,
                          explosion_boss, game_phase);
            }

            // Desenha o placar na tela
            draw_hud(font_info, icon, score, "Player 1");

            // Atualiza a tela para exibir os desenhos
            al_flip_display();
        }
    }

    // limpa tudo
    cleanup_resources(display, event_queue, timer, font, font_menu, font_warn, font_info, background, background_2,
                      background_3, player_sprite, player_sprite_dir, player_sprite_esq, player_sprite_dif1,
                      player_sprite_dir_dif1, player_sprite_esq_dif1, player_sprite_dif2, player_sprite_dir_dif2,
                      player_sprite_esq_dif2, bullet_sprite, bullet_sprite_2, bullet_sprite_3, bullet_sprite_dif1,
                      bullet_sprite_dif2, enemy_sprite, enemy_sprite_2, shooting_enemy_sprite, shooting_enemy_sprite_2,
                      enemy_bullet_sprite, boss_sprite, boss_sprite_2, boss_bullet_sprite, boss_bullet_special,
                      heart_full, heart_null, icon, explosion_sprite, explosion_boss, item_sprite, item_sprite_2,
                      bulletEnemy_boss2, slide_1, slide_2, slide_3);

    // destroi allegro
    al_destroy_audio_stream(music);
    al_destroy_audio_stream(music_menu);
    al_uninstall_audio();
    al_shutdown_font_addon();
    al_shutdown_ttf_addon();
    al_shutdown_image_addon();
    al_uninstall_system();

    return 0;
}
