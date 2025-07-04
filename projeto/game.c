
#include "game.h"

// Inicializa os recursos do jogo, incluindo bibliotecas e addons do Allegro
void init_resources()
{
    al_init();                  // Inicializa a Allegro
    al_init_primitives_addon(); // Inicializa o addon de primitivas gráficas
    al_init_font_addon();       // Inicializa o addon de fontes
    al_init_ttf_addon();        // Inicializa o addon de fontes TTF
    al_init_image_addon();      // Inicializa o addon de imagens
    al_install_audio();         // Instala o sistema de áudio
    al_init_acodec_addon();     // Inicializa o addon de codecs de áudio
    al_reserve_samples(1);      // Reserva 1 sample de áudio
}

// Libera os recursos alocados para o jogo, incluindo bitmaps, fontes, eventos, etc.
void cleanup_resources(
    ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer,
    ALLEGRO_FONT *font, ALLEGRO_FONT *font_menu, ALLEGRO_FONT *font_warn, ALLEGRO_FONT *font_info,
    ALLEGRO_BITMAP *background, ALLEGRO_BITMAP *background_2, ALLEGRO_BITMAP *background_3,
    ALLEGRO_BITMAP *player_sprite, ALLEGRO_BITMAP *player_sprite_dir, ALLEGRO_BITMAP *player_sprite_esq,
    ALLEGRO_BITMAP *player_sprite_dif1, ALLEGRO_BITMAP *player_sprite_dir_dif1, ALLEGRO_BITMAP *player_sprite_esq_dif1,
    ALLEGRO_BITMAP *player_sprite_dif2, ALLEGRO_BITMAP *player_sprite_dir_dif2, ALLEGRO_BITMAP *player_sprite_esq_dif2,
    ALLEGRO_BITMAP *bullet_sprite, ALLEGRO_BITMAP *bullet_sprite_2, ALLEGRO_BITMAP *bullet_sprite_3,
    ALLEGRO_BITMAP *bullet_sprite_dif1, ALLEGRO_BITMAP *bullet_sprite_dif2,
    ALLEGRO_BITMAP *enemy_sprite, ALLEGRO_BITMAP *enemy_sprite_2,
    ALLEGRO_BITMAP *shooting_enemy_sprite, ALLEGRO_BITMAP *shooting_enemy_sprite_2,
    ALLEGRO_BITMAP *enemy_bullet_sprite, ALLEGRO_BITMAP *boss_sprite, ALLEGRO_BITMAP *boss_sprite_2,
    ALLEGRO_BITMAP *boss_bullet_sprite, ALLEGRO_BITMAP *boss_bullet_special,
    ALLEGRO_BITMAP *heart_full, ALLEGRO_BITMAP *heart_null, ALLEGRO_BITMAP *icon,
    ALLEGRO_BITMAP *explosion_sprite, ALLEGRO_BITMAP *explosion_boss, ALLEGRO_BITMAP *item_sprite,
    ALLEGRO_BITMAP *item_sprite_2, ALLEGRO_BITMAP *bulletEnemy_boss2, ALLEGRO_BITMAP *slide_1, ALLEGRO_BITMAP *slide_2, ALLEGRO_BITMAP *slide_3)
{
    // Libera todos os bitmaps usados no jogo
    if (background)
        al_destroy_bitmap(background);
    if (background_2)
        al_destroy_bitmap(background_2);
    if (background_3)
        al_destroy_bitmap(background_3);
    if (player_sprite)
        al_destroy_bitmap(player_sprite);
    if (player_sprite_dir)
        al_destroy_bitmap(player_sprite_dir);
    if (player_sprite_esq)
        al_destroy_bitmap(player_sprite_esq);
    if (player_sprite_dif1)
        al_destroy_bitmap(player_sprite_dif1);
    if (player_sprite_dir_dif1)
        al_destroy_bitmap(player_sprite_dir_dif1);
    if (player_sprite_esq_dif1)
        al_destroy_bitmap(player_sprite_esq_dif1);
    if (player_sprite_dif2)
        al_destroy_bitmap(player_sprite_dif2);
    if (player_sprite_dir_dif2)
        al_destroy_bitmap(player_sprite_dir_dif2);
    if (player_sprite_esq_dif2)
        al_destroy_bitmap(player_sprite_esq_dif2);
    if (bullet_sprite)
        al_destroy_bitmap(bullet_sprite);
    if (bullet_sprite_2)
        al_destroy_bitmap(bullet_sprite_2);
    if (bullet_sprite_3)
        al_destroy_bitmap(bullet_sprite_3);
    if (bullet_sprite_dif1)
        al_destroy_bitmap(bullet_sprite_dif1);
    if (bullet_sprite_dif2)
        al_destroy_bitmap(bullet_sprite_dif2);
    if (enemy_sprite)
        al_destroy_bitmap(enemy_sprite);
    if (enemy_sprite_2)
        al_destroy_bitmap(enemy_sprite_2);
    if (shooting_enemy_sprite)
        al_destroy_bitmap(shooting_enemy_sprite);
    if (shooting_enemy_sprite_2)
        al_destroy_bitmap(shooting_enemy_sprite_2);
    if (enemy_bullet_sprite)
        al_destroy_bitmap(enemy_bullet_sprite);
    if (boss_sprite)
        al_destroy_bitmap(boss_sprite);
    if (boss_sprite_2)
        al_destroy_bitmap(boss_sprite_2);
    if (boss_bullet_sprite)
        al_destroy_bitmap(boss_bullet_sprite);
    if (boss_bullet_special)
        al_destroy_bitmap(boss_bullet_special);
    if (heart_full)
        al_destroy_bitmap(heart_full);
    if (heart_null)
        al_destroy_bitmap(heart_null);
    if (icon)
        al_destroy_bitmap(icon);
    if (explosion_sprite)
        al_destroy_bitmap(explosion_sprite);
    if (explosion_boss)
        al_destroy_bitmap(explosion_boss);
    if (item_sprite)
        al_destroy_bitmap(item_sprite);
    if (item_sprite_2)
        al_destroy_bitmap(item_sprite_2);
    if (bulletEnemy_boss2)
        al_destroy_bitmap(bulletEnemy_boss2);
    if (slide_1)
        al_destroy_bitmap(slide_1);
    if (slide_2)
        al_destroy_bitmap(slide_2);
    if (slide_3)
        al_destroy_bitmap(slide_3);

    // Libera todas as fontes usadas no jogo
    if (font)
        al_destroy_font(font);
    if (font_menu)
        al_destroy_font(font_menu);
    if (font_warn)
        al_destroy_font(font_warn);
    if (font_info)
        al_destroy_font(font_info);

    // Libera outros recursos como a fila de eventos, timer e display
    if (event_queue)
        al_destroy_event_queue(event_queue);
    if (timer)
        al_destroy_timer(timer);
    if (display)
        al_destroy_display(display);
}

// Exibe a tela de pausa com uma mensagem e opções para retomar ou sair
void draw_pause_message(ALLEGRO_FONT *font, ALLEGRO_BITMAP *background, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_DISPLAY *display, Player *player, int *exit_game)
{
    float background_x = 0;
    int running = 1;
    int color_counter = 0;
    int frame_counter = 0;
    int color_change_interval = 30;
    ALLEGRO_COLOR colors[] = {
        // Cores que alternam para o título
        al_map_rgb(255, 0, 0),
        al_map_rgb(0, 255, 0),
        al_map_rgb(0, 0, 255),
        al_map_rgb(255, 255, 0),
        al_map_rgb(255, 165, 0),
        al_map_rgb(255, 255, 255)};
    int num_colors = sizeof(colors) / sizeof(colors[0]);

    // Loop de exibição da tela de pausa
    while (running)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            // Movimento do fundo
            background_x -= 1;
            if (background_x <= -al_get_bitmap_width(background))
            {
                background_x = 0;
            }

            // Muda a cor do título a cada 30 frames
            frame_counter++;
            if (frame_counter >= color_change_interval)
            {
                frame_counter = 0;
                color_counter++;
                if (color_counter >= num_colors)
                    color_counter = 0;
            }

            // Desenha o fundo em movimento
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(background, background_x, 0, 0);
            al_draw_bitmap(background, background_x + al_get_bitmap_width(background), 0, 0);

            // Desenha a mensagem de pausa
            al_draw_filled_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, al_map_rgba(0, 0, 0, 150));
            al_draw_text(font, colors[color_counter], 400, 100, ALLEGRO_ALIGN_CENTER, "Jogo Pausado");

            // Exibe os botões para retomar ou sair
            ALLEGRO_FONT *button_font = al_load_ttf_font("fonts/menu_f.ttf", 40, 0);

            al_draw_text(button_font, al_map_rgb(255, 255, 255), 400, 250, ALLEGRO_ALIGN_CENTER, "Pressione ENTER para Retomar");
            al_draw_text(button_font, al_map_rgb(255, 255, 255), 400, 400, ALLEGRO_ALIGN_CENTER, "Pressione ESC para Sair");

            al_flip_display(); // Atualiza a tela

            al_destroy_font(button_font);
        }

        // Detecta a entrada do teclado
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
            {
                player->paused = 0; // Retorna ao jogo
                running = 0;        // Sai do loop de pausa
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            {
                running = 0;
                *exit_game = 1; // Define a flag para sair do jogo
            }
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            *exit_game = 1; // Define a flag para sair do jogo
            running = 0;    // Sai do loop de pausa
        }
    }

    if (*exit_game == 1)
    {
        al_destroy_display(display);
        al_destroy_event_queue(event_queue);
        exit(0);
    }
}

// Desenha as vidas do jogador na tela, representadas por corações cheios ou vazios
void draw_player_life(ALLEGRO_BITMAP *heart_full, ALLEGRO_BITMAP *heart_empty, Player *player)
{
    // Coordenadas para posicionar os corações na tela
    float x_start = 90;
    float y_start = 10;
    float spacing = 40;

    // Desenha um coração cheio ou vazio para cada vida
    for (int i = 0; i < 4; i++)
    {
        if (i < player->lives) // Se o índice for menor que o número de vidas, desenha um coração cheio
        {
            al_draw_bitmap(heart_full, x_start + i * spacing, y_start, 0);
        }
        else // Se o índice for maior ou igual ao número de vidas, desenha um coração vazio
        {
            al_draw_bitmap(heart_empty, x_start + i * spacing, y_start, 0);
        }
    }
}

// Exibe o menu de Game Over com opções para reiniciar ou sair do jogo
void show_game_over_menu(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font, int *restart_game, int *exit_game)
{
    int menu_active = 1;
    float background_x = 0;
    int color_counter = 0;
    int frame_counter = 0;
    int color_change_interval = 30;

    // Cores que o título alternará
    ALLEGRO_COLOR colors[] = {
        al_map_rgb(255, 0, 0),
        al_map_rgb(0, 255, 0),
        al_map_rgb(0, 0, 255),
        al_map_rgb(255, 255, 0),
        al_map_rgb(255, 165, 0),
        al_map_rgb(255, 255, 255)};
    int num_colors = sizeof(colors) / sizeof(colors[0]);

    // Carregar os recursos necessários
    ALLEGRO_FONT *menu_font = al_load_ttf_font("fonts/menu_f.ttf", 35, 0);
    ALLEGRO_BITMAP *menu_background = al_load_bitmap("imagens/menu_restart.png");

    while (menu_active)
    {
        // Atualiza a cor do título a cada 30 frames
        frame_counter++;
        if (frame_counter >= color_change_interval)
        {
            frame_counter = 0;
            color_counter++;
            if (color_counter >= num_colors)
            {
                color_counter = 0;
            }
        }

        // Movimento do fundo
        background_x -= 0.5f; // Movimenta o fundo para a esquerda
        if (background_x <= -al_get_bitmap_width(menu_background))
        {
            background_x = 0; // Reseta a posição do fundo
        }

        // Limpa a tela e desenha o fundo
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(menu_background, background_x, 0, 0);
        al_draw_bitmap(menu_background, background_x + al_get_bitmap_width(menu_background), 0, 0);

        // Desenha as mensagens no display
        al_draw_text(font, colors[color_counter], 400, 100, ALLEGRO_ALIGN_CENTER, "Você foi Derrotado!");
        al_draw_text(menu_font, al_map_rgb(255, 255, 255), 400, 350, ALLEGRO_ALIGN_CENTER, "Pressione ENTER para Reiniciar");
        al_draw_text(menu_font, al_map_rgb(255, 255, 255), 400, 450, ALLEGRO_ALIGN_CENTER, "Pressione ESQ para Sair");

        al_flip_display();
        // Processa os eventos de teclado
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) // Se pressionar R, reinicia o jogo
            {
                *restart_game = 1;
                menu_active = 0;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) // Se pressionar Q, sai do jogo
            {
                *exit_game = 1;
                menu_active = 0;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) // Se o usuário fechar a janela, sai do jogo
        {
            *exit_game = 1;
            menu_active = 0;
        }
    }

    // Liberação dos recursos carregados
    al_destroy_font(menu_font);
    al_destroy_bitmap(menu_background);
}

// Exibe o menu de transição entre fases com opções para continuar ou sair do jogo
void show_transition_menu(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font, int *continue_game, int *exit_game)
{
    int menu_active = 1;
    int color_counter = 0;
    int frame_counter = 0;
    int color_change_interval = 30;
    ALLEGRO_COLOR colors[] = {
        al_map_rgb(255, 0, 0),
        al_map_rgb(0, 255, 0),
        al_map_rgb(0, 0, 255),
        al_map_rgb(255, 255, 0),
        al_map_rgb(255, 165, 0),
        al_map_rgb(255, 255, 255)};
    int num_colors = sizeof(colors) / sizeof(colors[0]);

    // Carrega o fundo de transição
    ALLEGRO_BITMAP *background = al_load_bitmap("imagens/menu.jpg");

    // Variáveis para o movimento do fundo
    float background_x = 0.0f;
    float background_speed = 0.2f;

    while (menu_active)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        // Atualiza a cor do título a cada 30 frames
        frame_counter++;
        if (frame_counter >= color_change_interval)
        {
            frame_counter = 0;
            color_counter++;
            if (color_counter >= num_colors)
            {
                color_counter = 0;
            }
        }

        // Movimento do fundo
        background_x -= background_speed;
        if (background_x <= -al_get_bitmap_width(background))
        {
            background_x = 0;
        }

        // Limpa a tela e desenha o fundo
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(background, background_x, 0, 0);
        al_draw_bitmap(background, background_x + al_get_bitmap_width(background), 0, 0);

        // Exibe o título com a cor alternada
        al_draw_text(font, colors[color_counter], SCREEN_WIDTH / 2, 100, ALLEGRO_ALIGN_CENTER, "Boss Derrotado!");

        // Desenha os botões de controle com fonte menor
        ALLEGRO_FONT *button_font = al_load_ttf_font("fonts/game_f.ttf", 35, 0);

        al_draw_text(button_font, al_map_rgb(200, 200, 200), 400, 230, ALLEGRO_ALIGN_CENTER, "Pressione ENTER para");
        al_draw_text(button_font, al_map_rgb(200, 200, 200), 400, 280, ALLEGRO_ALIGN_CENTER, "Avancar a Proxima Fase");
        al_draw_text(button_font, al_map_rgb(200, 200, 200), 400, 390, ALLEGRO_ALIGN_CENTER, "Pressione ESC para Sair");

        al_flip_display();

        // Detecta as ações do teclado
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) // Se pressionar 1, continua para a próxima fase
            {
                *continue_game = 1;
                menu_active = 0;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) // Se pressionar 2, sai do jogo
            {
                *exit_game = 1;
                menu_active = 0;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) // Se o usuário fechar a janela, sai do jogo
        {
            *exit_game = 1;
            menu_active = 0;
        }

        al_destroy_font(button_font); // Libera a fonte dos botões após desenhar
    }

    // Libera o bitmap do fundo
    al_destroy_bitmap(background);
}

// Função para inicializar a segunda fase do jogo
void init_second_phase(Player *player, Enemy enemies[], Bullet bullets[], ShootingEnemy shooting_enemies[], Boss *boss, int *victory_state, int *player_won, double *start_time, int *enemy_destroyed_count, int game_phase)
{
    // Configurações iniciais do chefe (
    boss->health = 18;
    boss->speed = 6;
    boss->horizontal_speed = 2;
    boss->y = SCREEN_HEIGHT / 4;
    boss->x = SCREEN_WIDTH;
    *victory_state = 0;
    *player_won = 0;
    *start_time = al_get_time();
    game_phase = 2;
    *enemy_destroyed_count = 0;

    // Inicializa as configurações dos inimigos que disparam
    for (int i = 0; i < MAX_SHOOTING_ENEMIES; i++)
    {
        shooting_enemies[i].vertical_speed = 4; // Define a velocidade vertical dos inimigos que atiram
    }

    // Inicializa as configurações dos inimigos normais
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        enemies[i].active = 0;
        enemies[i].health = 2;
        enemies[i].vertical_speed = 1.0;
        enemies[i].original_y = rand() % (SCREEN_HEIGHT - 30);
        enemies[i].y = enemies[i].original_y;
        enemies[i].moving_up = 1;
    }

    // Inicializa o jogador, inimigos e projéteis
    init_player(player);
    init_enemies(enemies, MAX_ENEMIES);
    init_bullets(bullets, MAX_BULLETS);
}

// Função para exibir a mensagem de vitória ao jogador
void show_victory_message(ALLEGRO_FONT *font, int *exit_game)
{
    ALLEGRO_FONT *font_title = al_load_ttf_font("fonts/menu_f.ttf", 35, 0);
    if (!font_title)
    {
        fprintf(stderr, "Erro ao carregar a fonte do título.\n");
        return;
    }

    ALLEGRO_FONT *font_normal = al_load_ttf_font("fonts/menu_f.ttf", 50, 0);
    if (!font_normal)
    {
        fprintf(stderr, "Erro ao carregar a fonte normal.\n");
        al_destroy_font(font_title);
        return;
    }

    ALLEGRO_BITMAP *background_m = al_load_bitmap("imagens/menu.jpg");
    if (!background_m)
    {
        fprintf(stderr, "Erro ao carregar o fundo do menu.\n");
        al_destroy_font(font_title);
        al_destroy_font(font_normal);
        return;
    }

    float background_x = 0.0f;     // Posição inicial do fundo
    float background_speed = 0.2f; // Velocidade de movimento do fundo

    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    if (!event_queue)
    {
        fprintf(stderr, "Erro ao criar a fila de eventos.\n");
        al_destroy_font(font_title);
        al_destroy_font(font_normal);
        al_destroy_bitmap(background_m);
        return;
    }

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(al_get_current_display()));

    al_clear_to_color(al_map_rgb(0, 0, 0));
    text_game_over_screen("Parabens!", font_normal, 400, 180, al_map_rgb(255, 255, 255), 0.1, background_m, background_x);
    text_game_over_screen("Voce zerou o jogo!", font_normal, 400, 250, al_map_rgb(255, 255, 255), 0.1, background_m, background_x);

    int running = 1; // Controle do loop principal

    while (running)
    {
        // Movimento do fundo
        background_x -= background_speed;
        if (background_x <= -al_get_bitmap_width(background_m))
        {
            background_x = 0; // Reseta a posição do fundo
        }

        // Limpa e desenha o fundo
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(background_m, background_x, 0, 0);
        al_draw_bitmap(background_m, background_x + al_get_bitmap_width(background_m), 0, 0);

        // Desenha mensagens estáticas
        al_draw_text(font_normal, al_map_rgb(255, 255, 255), 400, 180, ALLEGRO_ALIGN_CENTER, "Parabens!");
        al_draw_text(font_normal, al_map_rgb(255, 255, 255), 400, 250, ALLEGRO_ALIGN_CENTER, "Voce zerou o jogo!");
        al_draw_text(font_title, al_map_rgb(255, 255, 255), 400, 500, ALLEGRO_ALIGN_CENTER, "Pressione Enter para Sair");

        al_flip_display();

        // Verifica eventos
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
        {
            running = 0; // Sai do loop
            *exit_game = 1;
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            running = 0; // Sai do loop ao fechar o display
            *exit_game = 1;
        }
    }
    al_destroy_event_queue(event_queue);
    al_destroy_bitmap(background_m);
    al_destroy_font(font_title);
    al_destroy_font(font_normal);
}

// Função para exibir o texto gradualmente na tela
void text_game_over_screen(const char *texto, ALLEGRO_FONT *fonte, float x, float y, ALLEGRO_COLOR cor, float intervalo_entre_letras, ALLEGRO_BITMAP *background_m, float background_x)
{
    char buffer[128] = ""; // Buffer para armazenar o texto que vai sendo construído
    int tamanho_texto = strlen(texto);

    for (int i = 0; i < tamanho_texto; i++)
    {
        // Adiciona a próxima letra ao buffer
        buffer[i] = texto[i];
        buffer[i + 1] = '\0';

        // Redesenha o fundo e as frases já exibidas
        al_draw_bitmap(background_m, background_x, 0, 0);
        al_draw_bitmap(background_m, background_x + al_get_bitmap_width(background_m), 0, 0);

        // Desenha as frases na tela
        al_draw_text(fonte, cor, x, y, ALLEGRO_ALIGN_CENTER, buffer);

        al_flip_display();

        // Pausa para dar o efeito de digitação
        al_rest(intervalo_entre_letras);
    }
}

void show_controls_screen(ALLEGRO_FONT *font, ALLEGRO_BITMAP *background, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_DISPLAY *display, int *exit_game, int *menu_running)
{
    float background_x = 0;
    int running = 1;

    while (running)
    {
        if (*exit_game)
        {
            break; // Finaliza o jogo
        }
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            // Movimento do fundo
            background_x -= 1;
            if (background_x <= -al_get_bitmap_width(background))
            {
                background_x = 0;
            }

            // Desenha o fundo e a tela de controles
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(background, background_x, 0, 0);
            al_draw_bitmap(background, background_x + al_get_bitmap_width(background), 0, 0);
            al_draw_filled_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, al_map_rgba(0, 0, 0, 0));
            al_draw_text(font, al_map_rgb(0, 255, 255), 400, 20, ALLEGRO_ALIGN_CENTER, "CONTROLES");
            al_draw_text(font, al_map_rgb(255, 255, 255), 400, 120, ALLEGRO_ALIGN_CENTER, "MOVER: W, A, S, D");
            al_draw_text(font, al_map_rgb(255, 255, 255), 400, 220, ALLEGRO_ALIGN_CENTER, "ATIRAR: ENTER");
            al_draw_text(font, al_map_rgb(255, 255, 255), 400, 320, ALLEGRO_ALIGN_CENTER, "PAUSAR: P");
            al_draw_text(font, al_map_rgb(0, 255, 255), 400, 520, ALLEGRO_ALIGN_CENTER, "VOLTAR: ESC");

            al_flip_display(); // Atualiza a tela
        }

        // Detecta ações do teclado
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
        {
            running = 0; // Sai da tela de controles
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            running = 0;
            *exit_game = 1;
            *menu_running = 0;
        }
    }
}

// Função principal para executar o menu do jogo
void run_menu(ALLEGRO_FONT *font_menu, ALLEGRO_BITMAP *background, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_DISPLAY *display, ALLEGRO_AUDIO_STREAM *music_menu, GameOptions *game_options, int *exit_game)
{
    int menu_running = 1;
    int selected_option = 0;
    float background_x = 0;
    float background_speed = 1.0;

    // Configurar música do menu para tocar em loop
    al_set_audio_stream_playmode(music_menu, ALLEGRO_PLAYMODE_LOOP);
    al_attach_audio_stream_to_mixer(music_menu, al_get_default_mixer());

    // Carregar fonte maior para o título
    ALLEGRO_FONT *font_title = al_load_ttf_font("fonts/game_f.ttf", 72, 0);
    // Carregar imagem de fundo do menu
    ALLEGRO_BITMAP *background_m = al_load_bitmap("imagens/menu.jpg");

    // Configurações de alternância de cores do título
    int color_counter = 0;
    int color_change_timer = 0;
    int color_change_interval = 30;
    ALLEGRO_COLOR colors[] = {
        // Paleta de cores para o título
        al_map_rgb(255, 0, 0),
        al_map_rgb(0, 255, 0),
        al_map_rgb(0, 0, 255),
        al_map_rgb(255, 255, 0),
        al_map_rgb(255, 165, 0),
        al_map_rgb(255, 255, 255)};
    int num_colors = sizeof(colors) / sizeof(colors[0]);

    // Loop principal do menu
    while (menu_running)
    {
        // Verifica se o jogador decidiu sair do jogo
        if (*exit_game == 1)
        {
            break;
        }

        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            menu_running = 0;
            *exit_game = 1;
        }
        else if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            // Movimento do fundo
            background_x -= background_speed;
            if (background_x <= -al_get_bitmap_width(background_m))
            {
                background_x = 0;
            }

            // Desenho do fundo
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(background_m, background_x, 0, 0);
            al_draw_bitmap(background_m, background_x + al_get_bitmap_width(background_m), 0, 0);

            // Alternância de cores do título
            color_change_timer++;
            if (color_change_timer >= color_change_interval)
            {
                color_change_timer = 0;
                color_counter++;
                if (color_counter >= num_colors)
                {
                    color_counter = 0;
                }
            }

            ALLEGRO_COLOR current_color = colors[color_counter % num_colors]; // Cor atual do título

            // Desenho do título do menu
            al_draw_text(font_title, current_color, 400, 80, ALLEGRO_ALIGN_CENTER, "Space Impact");

            // Desenho das opções do menu
            for (int i = 0; i < 4; i++)
            {
                // Determina a cor da opção atual (branca se selecionada, preta caso contrário)
                ALLEGRO_COLOR color = (i == selected_option) ? al_map_rgb(255, 255, 255) : al_map_rgb(0, 0, 0);
                const char *text;
                float pos_y = 270 + i * 75; // Espaçamento entre as opções

                // Define o texto de cada opção
                if (i == 0)
                    text = "Começar";
                else if (i == 1)
                    text = "Controles";
                else if (i == 2)
                    text = "Opções";
                else
                    text = "Sair";

                // Desenha a opção no menu
                al_draw_textf(font_menu, color, 400, pos_y, ALLEGRO_ALIGN_CENTER, "%s", text);
            }

            al_flip_display();
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            // Controle de navegação e seleção
            switch (ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_DOWN:
                selected_option = (selected_option + 1) % 4; // Move para a próxima opção
                break;
            case ALLEGRO_KEY_UP:
                selected_option = (selected_option - 1 + 4) % 4; // Move para a opção anterior
                break;
            case ALLEGRO_KEY_ENTER:
                // Executa a ação correspondente à opção selecionada
                if (selected_option == 0)
                {
                    menu_running = 0;
                }
                else if (selected_option == 1)
                {
                    // Exibe os controles
                    show_controls_screen(font_menu, background_m, event_queue, display, exit_game, &menu_running);
                }
                else if (selected_option == 2)
                {
                    // Exibe as opções de configuração
                    show_options_screen(font_menu, background_m, event_queue, display, game_options, exit_game, &menu_running);
                }
                else if (selected_option == 3)
                {
                    menu_running = 0;
                    *exit_game = 1;
                }
                break;
            }
        }
    }

    // Libera a fonte do título após o fim do menu
    al_destroy_font(font_title);
}

// Inicializa as opções do jogo com valores padrão
void init_options(GameOptions *game_options)
{
    game_options->new_option_1 = 0;
    game_options->new_option_2 = 0;
    game_options->sprite_option = 0;
    game_options->new_option_3 = 0;
}

void show_options_screen(ALLEGRO_FONT *font, ALLEGRO_BITMAP *background, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_DISPLAY *display, GameOptions *game_options, int *exit_game, int *menu_running)
{
    int running = 1;
    int selected_group = 0;                                         // Grupo atualmente selecionado (0 = grupo 1, 1 = grupo 2, 2 = grupo 3, 3 = grupo 4)
    int selected_option = 0;                                        // Opção dentro do grupo selecionado
    int checkbox_states[12] = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0}; // 3 opções do primeiro grupo + 3 do segundo grupo + 3 do terceiro grupo + 3 do quarto grupo
    float background_x = 0;                                         // Posição do fundo

    ALLEGRO_FONT *font_menu = al_load_ttf_font("fonts/menu_f.ttf", 20, 0);
    ALLEGRO_FONT *font_option = al_load_ttf_font("fonts/menu_f.ttf", 10, 0);

    // Carregar imagens para as opções
    ALLEGRO_BITMAP *option_images[12];
    option_images[0] = al_load_bitmap("imagens/nave.png");
    option_images[1] = al_load_bitmap("imagens/nave1.png");
    option_images[2] = al_load_bitmap("imagens/nave2.png");
    option_images[3] = al_load_bitmap("imagens/bullet.png");
    option_images[4] = al_load_bitmap("imagens/bullet_dif1.png");
    option_images[5] = al_load_bitmap("imagens/bullet_dif2.png");
    option_images[6] = al_load_bitmap("imagens/background_1_icon.png");
    option_images[7] = al_load_bitmap("imagens/background_2_icon.png");
    option_images[8] = al_load_bitmap("imagens/background_3_icon.png");
    option_images[9] = al_load_bitmap("imagens/background_1_icon.png");
    option_images[10] = al_load_bitmap("imagens/background_2_icon.png");
    option_images[11] = al_load_bitmap("imagens/background_3_icon.png");

    // Flags de opções
    int sprite_option = game_options->group_1_selected;
    int new_option_1 = game_options->group_2_selected;
    int new_option_2 = game_options->group_3_selected;
    int new_option_3 = game_options->group_4_selected;

    while (running)
    {
        if (*exit_game)
        {
            break; // Finaliza o jogo
        }

        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            // Movimento do fundo
            background_x -= 1;
            if (background_x <= -al_get_bitmap_width(background))
            {
                background_x = 0;
            }

            // Desenha o fundo
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(background, background_x, 0, 0);
            al_draw_bitmap(background, background_x + al_get_bitmap_width(background), 0, 0);

            // Desenha o título principal
            al_draw_text(font, al_map_rgb(0, 255, 255), 400, 40, ALLEGRO_ALIGN_CENTER, "CONFIGURAÇÕES");

            // Calcula posições centralizadas horizontalmente
            int screen_center_x = 360;
            int center_y = 300;
            int option_spacing = 250;
            int checkbox_size = 20;

            const char *group_titles[4] = {
                "Escolha sua Nave",
                "Escolha seu disparo",
                "Escolha o Cenário da fase 1",
                "Escolha o Cenário da fase 2"};

            // Desenha as opções dos grupos
            for (int group = 0; group < 4; group++)
            {
                int start_y = center_y + (group - 1) * 100;

                // Título do grupo
                al_draw_text(font_option, al_map_rgb(255, 0, 0), screen_center_x + 22, start_y - 60, ALLEGRO_ALIGN_CENTER, group_titles[group]);

                // Subgrupos e opções
                for (int i = 0; i < 3; i++)
                {

                    int current_option = group * 3 + i;

                    // Coordenadas da checkbox
                    int checkbox_x = screen_center_x - (3 - 1) * option_spacing / 2 + i * option_spacing - checkbox_size / 2;
                    int checkbox_y = start_y - checkbox_size / 2;

                    // Coordenadas da imagem
                    float image_scale = 0.8;
                    int image_width = al_get_bitmap_width(option_images[current_option]) * image_scale;
                    int image_height = al_get_bitmap_height(option_images[current_option]) * image_scale;
                    int image_x = checkbox_x + checkbox_size + 10;
                    int image_y = start_y - image_height / 2;

                    // Opacidade da imagem
                    ALLEGRO_COLOR tint = (current_option == selected_group * 3 + selected_option) ? al_map_rgba_f(1.0, 1.0, 1.0, 0.1) : al_map_rgba_f(1.0, 1.0, 1.0, 1.0);

                    // Desenhar checkbox
                    if (checkbox_states[current_option]) // Marcar checkbox
                    {
                        al_draw_filled_rectangle(checkbox_x, checkbox_y, checkbox_x + checkbox_size, checkbox_y + checkbox_size, al_map_rgb(200, 200, 200));
                    }
                    else // Checkbox vazia
                    {
                        al_draw_rectangle(checkbox_x, checkbox_y, checkbox_x + checkbox_size, checkbox_y + checkbox_size, al_map_rgb(255, 255, 255), 2);
                    }

                    // Desenhar imagem com opacidade
                    al_draw_tinted_scaled_bitmap(option_images[current_option], tint, 0, 0, al_get_bitmap_width(option_images[current_option]), al_get_bitmap_height(option_images[current_option]),
                                                 image_x, image_y, image_width, image_height, 0);
                }
            }

            ALLEGRO_COLOR back_color = al_map_rgb(255, 255, 255);
            al_draw_text(font_menu, back_color, 400, 560, ALLEGRO_ALIGN_CENTER, "ESC PARA SAIR");

            al_flip_display();
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            // Lógica de navegação
            if (ev.keyboard.keycode == ALLEGRO_KEY_UP)
            {
                // Navegar verticalmente para o grupo de cima
                if (selected_group > 0)
                {
                    selected_group--;
                    selected_option = 0; // Resetar para a primeira opção ao mudar de grupo
                }
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN)
            {
                // Navegar verticalmente para o grupo de baixo
                if (selected_group < 3)
                {
                    selected_group++;
                    selected_option = 0;
                }
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT)
            {
                // Navegar horizontalmente para a opção à esquerda
                if (selected_option > 0)
                {
                    selected_option--;
                }
                else
                {
                    selected_option = 2;
                }
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT)
            {
                // Navegar horizontalmente para a opção à direita
                if (selected_option < 2)
                {
                    selected_option++;
                }
                else
                {
                    selected_option = 0;
                }
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
            {
                // Ação ao pressionar Enter (marcar a checkbox da opção selecionada)
                int current_option = selected_group * 3 + selected_option;

                // Desmarcar todas as opções do grupo antes de marcar a nova
                for (int i = 0; i < 3; i++)
                {
                    int option_index = selected_group * 3 + i;
                    checkbox_states[option_index] = 0;
                }

                // Marcar a opção selecionada
                checkbox_states[current_option] = 1;

                // Atualiza as flags dos grupos baseadas na opção selecionada
                if (selected_group == 0)
                {
                    game_options->sprite_option = selected_option;
                    game_options->group_1_selected = selected_option;
                }
                else if (selected_group == 1)
                {
                    game_options->new_option_1 = selected_option;
                    game_options->group_2_selected = selected_option;
                }
                else if (selected_group == 2)
                {
                    game_options->new_option_2 = selected_option;
                    game_options->group_3_selected = selected_option;
                }
                else if (selected_group == 3)
                {
                    game_options->new_option_3 = selected_option;
                    game_options->group_4_selected = selected_option;
                }
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            {
                running = 0;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            *exit_game = 1;
            *menu_running = 0;
            running = 0;
        }
    }
    for (int i = 0; i < 12; i++)
    {
        if (option_images[i])
        {
            al_destroy_bitmap(option_images[i]);
            option_images[i] = NULL;
        }
    }

    if (font_menu)
    {
        al_destroy_font(font_menu);
        font_menu = NULL;
    }
    if (font_option)
    {
        al_destroy_font(font_option);
        font_option = NULL;
    }
}

void draw_explosion(Enemy *enemy, ALLEGRO_BITMAP *explosion_sprite, double explosion_duration)
{
    if (enemy->exploding)
    {
        double elapsed_time = al_get_time() - enemy->explosion_time; // Tempo desde o início da explosão

        // Se o tempo da explosão ainda não passou, desenha a explosão
        if (elapsed_time < explosion_duration)
        {
            al_draw_bitmap(explosion_sprite, enemy->x - 5, enemy->y + 5, 0);
        }
        else
        {
            enemy->exploding = 0; // Finaliza a explosão após o tempo
        }
    }
}

void draw_explosion_shoot(ShootingEnemy *shooting_enemy, ALLEGRO_BITMAP *explosion_sprite, double explosion_duration)
{
    if (shooting_enemy->exploding)
    {
        double elapsed_time = al_get_time() - shooting_enemy->explosion_time;

        // Se o tempo da explosão ainda não passou, desenha a explosão
        if (elapsed_time < explosion_duration)
        {
            al_draw_bitmap(explosion_sprite, shooting_enemy->x - 5, shooting_enemy->y + 5, 0);
        }
        else
        {
            shooting_enemy->exploding = 0; // Finaliza a explosão após o tempo
        }
    }
}
void draw_explosion_boss(Boss *boss, ALLEGRO_BITMAP *explosion_sprite, double explosion_duration)
{
    if (boss->exploding)
    {
        double elapsed_time = al_get_time() - boss->explosion_time;

        // Se o tempo da explosão ainda não passou, desenha a explosão
        if (elapsed_time < explosion_duration)
        {

            ALLEGRO_COLOR color = al_map_rgba_f(1.0, 1.0, 1.0, 0.01);

            al_draw_tinted_bitmap(explosion_sprite, color, boss->x + 25, boss->y + 25, 0);
        }
        else
        {
            boss->exploding = 0; // Finaliza a explosão após o tempo
        }
    }
}

void set_background(int option, ALLEGRO_BITMAP *backgrounds[], ALLEGRO_BITMAP **current_background)
{
    if (option >= 0 && option < 3)
    {
        *current_background = backgrounds[option];
    }
}

void update_backgrounds(GameOptions game_options, ALLEGRO_BITMAP *backgrounds[], ALLEGRO_BITMAP **current_background, ALLEGRO_BITMAP **current_background_2)
{
    set_background(game_options.new_option_2, backgrounds, current_background);
    set_background(game_options.new_option_3, backgrounds, current_background_2);
}

void handle_phase1_enemy_generation(ShootingEnemy shooting_enemies[], int max_shooting_enemies,
                                    Enemy enemies[], int max_enemies, int game_phase, Player player)
{
    if (rand() % 200 == 0)
    {
        generate_shooting_enemy(shooting_enemies, max_shooting_enemies, game_phase);
    }
    if (rand() % 60 == 0)
    {
        generate_enemy(enemies, max_enemies, player.width, player.height);
    }
}

void handle_phase2_enemy_generation(ShootingEnemy shooting_enemies[], int max_shooting_enemies,
                                    Enemy enemies[], int max_enemies, int game_phase, Player player,
                                    double *phase2_start_time, bool *phase2_started)
{
    // Inicializa o tempo quando a fase 2 começa
    if (!(*phase2_started))
    {
        *phase2_start_time = al_get_time();
        *phase2_started = true;
    }

    // Verifica se passou 1 segundo desde o início da fase 2
    if (al_get_time() - *phase2_start_time >= 1.0)
    {
        if (rand() % 100 == 0)
        {
            generate_shooting_enemy(shooting_enemies, max_shooting_enemies, game_phase);
        }
        if (rand() % 30 == 0)
        {
            generate_enemy(enemies, max_enemies, player.width, player.height);
        }
    }
}

void handle_keyboard_event(ALLEGRO_EVENT ev, Player *player, float *background_speed,
                           ALLEGRO_FONT *font_menu, ALLEGRO_BITMAP *current_background,
                           ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_DISPLAY *display,
                           int *exit_game)
{
    switch (ev.keyboard.keycode)
    {
    case ALLEGRO_KEY_W:
        player->joystick.up = 1;
        break;
    case ALLEGRO_KEY_S:
        player->joystick.down = 1;
        break;
    case ALLEGRO_KEY_A:
        player->joystick.left = 1;
        *background_speed = 0.5;
        break;
    case ALLEGRO_KEY_D:
        player->joystick.right = 1;
        *background_speed = 4.0;
        break;
    case ALLEGRO_KEY_ENTER:
        player->joystick.fire = 1;
        break;
    case ALLEGRO_KEY_P: // Aperte P para pausar/despausar
        player->paused = !player->paused;
        if (player->paused)
        {
            draw_pause_message(font_menu, current_background, event_queue, display, player, exit_game);
        }
        break;
    }
}

void handle_keyboard_release_event(ALLEGRO_EVENT ev, Player *player, float *background_speed)
{
    switch (ev.keyboard.keycode)
    {
    case ALLEGRO_KEY_W:
        player->joystick.up = 0;
        break;
    case ALLEGRO_KEY_S:
        player->joystick.down = 0;
        break;
    case ALLEGRO_KEY_A:
        player->joystick.left = 0;
        *background_speed = 2.0;
        break;
    case ALLEGRO_KEY_D:
        player->joystick.right = 0;
        *background_speed = 2.0;
        break;
    case ALLEGRO_KEY_ENTER:
        player->joystick.fire = 0;
        break;
    }
}
void restart_init_game(Player *player, Boss *boss, Enemy enemies[], int max_enemies,
                       Bullet bullets[], int max_bullets, int *game_over,
                       int *player_won, int *score, double *start_time, int *enemy_destroyed_count)
{
    // Reinicializa os elementos do jogo
    init_player(player);
    init_boss(boss);
    init_enemies(enemies, max_enemies);
    init_bullets(bullets, max_bullets);
    *enemy_destroyed_count = 0;

    // Define o jogador como invulnerável temporariamente
    player->invulnerable = 1;
    player->invulnerable_time = al_get_time();

    // Zera o estado do jogo
    *game_over = 0;
    *player_won = 0;
    *score = 0;

    // Reinicia o cronômetro do jogo
    *start_time = al_get_time();
}

void start_history_slide(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer,
                         ALLEGRO_BITMAP *image1, ALLEGRO_BITMAP *image2, ALLEGRO_BITMAP *image3, int *exit_game, int *history_game)
{
    // Variáveis de controle
    int current_image = 0;
    double image_timer = 0.0;
    bool running = true;
    *history_game = 0;

    al_start_timer(timer);

    // Loop principal do slideshow
    while (running)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            // Atualizar timer da imagem
            image_timer += 1.0 / 60.0;

            // Trocar de imagem automaticamente a cada 8 segundos
            if (image_timer >= IMAGE_SWITCH_TIME)
            {
                image_timer = 0.0;
                current_image++;
                if (current_image >= 3)
                {
                    running = false; // Termina o slideshow
                }
            }

            // Renderizar tela
            al_clear_to_color(al_map_rgb(0, 0, 0));
            if (current_image == 0)
            {
                al_draw_bitmap(image1, 0, 0, 0);
            }
            else if (current_image == 1)
            {
                al_draw_bitmap(image2, 0, 0, 0);
            }
            else if (current_image == 2)
            {
                al_draw_bitmap(image3, 0, 0, 0);
            }
            al_flip_display();
        }

        // Tecla Enter para trocar de imagem
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
            {
                current_image++;
                image_timer = 0.0; // Reiniciar o timer
                if (current_image >= 3)
                {
                    running = false; // Termina o slideshow
                }
            }
        }

        // Fechar janela
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            running = false;
            *exit_game = 1;
        }
    }
}
