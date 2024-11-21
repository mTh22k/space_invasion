
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

#include <stdbool.h>

void draw_pause_message(ALLEGRO_FONT *font, ALLEGRO_BITMAP *background, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_DISPLAY *display, Player *player, int *exit_game)
{
    float background_x = 0;
    int running = 1;                // Controle do loop
    int color_counter = 0;          // Contador para alternar entre cores
    int frame_counter = 0;          // Contador de frames para mudar a cor a cada 30 frames
    int color_change_interval = 30; // Intervalo de frames entre mudanças de cor (ajuste esse valor para alterar a velocidade)
    ALLEGRO_COLOR colors[] = {
        al_map_rgb(255, 0, 0),    // Vermelho
        al_map_rgb(0, 255, 0),    // Verde
        al_map_rgb(0, 0, 255),    // Azul
        al_map_rgb(255, 255, 0),  // Amarelo
        al_map_rgb(255, 165, 0),  // Laranja
        al_map_rgb(255, 255, 255) // Branco
    };
    int num_colors = sizeof(colors) / sizeof(colors[0]);

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

            // Atualiza o contador de frames e altera a cor do título a cada 30 frames
            frame_counter++;
            if (frame_counter >= color_change_interval)
            {
                frame_counter = 0; // Reseta o contador de frames
                color_counter++;   // Muda a cor
                if (color_counter >= num_colors)
                {
                    color_counter = 0; // Volta para a primeira cor
                }
            }

            // Desenha o fundo em movimento
            al_clear_to_color(al_map_rgb(0, 0, 0));                                           // Limpa a tela
            al_draw_bitmap(background, background_x, 0, 0);                                   // Fundo principal
            al_draw_bitmap(background, background_x + al_get_bitmap_width(background), 0, 0); // Fundo repetido para efeito de movimento

            // Desenha a mensagem de pausa e as opções
            al_draw_filled_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, al_map_rgba(0, 0, 0, 150)); // Cor semitransparente

            // Título com a cor que alterna a cada 30 frames
            al_draw_text(font, colors[color_counter], 400, 100, ALLEGRO_ALIGN_CENTER, "Jogo Pausado");

            // Fonte menor para os botões
            ALLEGRO_FONT *button_font = al_load_ttf_font("fonts/menu_f.ttf", 40, 0);
            if (!button_font)
            {
                fprintf(stderr, "Erro ao carregar a fonte dos botões.\n");
                return;
            }

            al_draw_text(button_font, al_map_rgb(255, 255, 255), 400, 250, ALLEGRO_ALIGN_CENTER, "Pressione R para Retomar");
            al_draw_text(button_font, al_map_rgb(255, 255, 255), 400, 400, ALLEGRO_ALIGN_CENTER, "Pressione Q para Sair");

            al_flip_display(); // Atualiza a tela

            al_destroy_font(button_font); // Libera a fonte dos botões após desenhar
        }

        // Detecta ações do teclado
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (ev.keyboard.keycode == ALLEGRO_KEY_R)
            {
                player->paused = 0; // Retorna ao jogo
                running = 0;        // Sai da tela de pausa
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_Q)
            {
                *exit_game = 1; // Atualiza o controle para sair do jogo
                running = 0;    // Sai da tela de pausa
            }
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            *exit_game = 1; // Atualiza o controle de saída se a janela for fechada
            running = 0;    // Sai da tela de pausa
        }
    }

    if (*exit_game == 1)
    {
        al_destroy_display(display);         // Libere recursos, como o display
        al_destroy_event_queue(event_queue); // Libere a fila de eventos
        exit(0);                             // Feche o jogo corretamente
    }
}

void draw_player_life(ALLEGRO_FONT *font, Player *player)
{
    char life_text[20];
    sprintf(life_text, "Vida: %d", player->lives);
    al_draw_text(font, al_map_rgb(255, 255, 255), 400, 10, 0, life_text);
}

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
        al_draw_bitmap(background, 0, 0, 0); // Desenha o fundo antes da explosão
        al_draw_bitmap(explosion_bitmaps[i], x, y, 0);
        al_flip_display();
        al_rest(0.3); // Duração de cada frame
    }
}

ALLEGRO_FONT *menu_font = NULL;
ALLEGRO_BITMAP *menu_background = NULL;

void draw_menu()
{
    if (menu_background)
    {
        al_draw_bitmap(menu_background, 0, 0, 0);
    }
    else
    {
        al_clear_to_color(al_map_rgb(0, 0, 0));
    }

    const char *menu_options[] = {"SPACE IMPACT", "1. Jogar", "2. Sair"};
    int menu_option_count = 3;

    float start_y = SCREEN_HEIGHT / 3;
    float option_spacing = 100;

    for (int i = 0; i < menu_option_count; i++)
    {
        al_draw_text(menu_font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, start_y + i * option_spacing, ALLEGRO_ALIGN_CENTRE, menu_options[i]);
    }

    al_flip_display();
}

void init_menu_resources()
{
    menu_font = al_load_ttf_font("fonts/custom_font.ttf", 50, 0);
    if (!menu_font)
    {
        fprintf(stderr, "Erro ao carregar a fonte do menu.\n");
        exit(1);
    }

    menu_background = al_load_bitmap("imagens/menu_back.jpg");
    if (!menu_background)
    {
        fprintf(stderr, "Erro ao carregar o background do menu.\n");
        exit(1);
    }
}

void cleanup_menu_resources()
{
    if (menu_font)
    {
        al_destroy_font(menu_font);
        menu_font = NULL;
    }
    if (menu_background)
    {
        al_destroy_bitmap(menu_background);
        menu_background = NULL;
    }
}

void show_game_over_menu(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font, int *restart_game, int *exit_game)
{
    int menu_active = 1;
    float background_x = 0;
    int color_counter = 0;          // Contador para alternar entre cores
    int frame_counter = 0;          // Contador de frames para mudar a cor a cada 30 frames
    int color_change_interval = 30; // Intervalo de frames entre mudanças de cor

    ALLEGRO_COLOR colors[] = {
        al_map_rgb(255, 0, 0),    // Vermelho
        al_map_rgb(0, 255, 0),    // Verde
        al_map_rgb(0, 0, 255),    // Azul
        al_map_rgb(255, 255, 0),  // Amarelo
        al_map_rgb(255, 165, 0),  // Laranja
        al_map_rgb(255, 255, 255) // Branco
    };
    int num_colors = sizeof(colors) / sizeof(colors[0]);

    // Carregar recursos antes do loop
    ALLEGRO_FONT *menu_font = al_load_ttf_font("fonts/menu_f.ttf", 35, 0);
    if (!menu_font)
    {
        fprintf(stderr, "Erro ao carregar fonte do menu.\n");
        return;
    }

    ALLEGRO_BITMAP *menu_background = al_load_bitmap("imagens/menu_restart.png");
    if (!menu_background)
    {
        fprintf(stderr, "Erro ao carregar o fundo do menu.\n");
        al_destroy_font(menu_font);
        return;
    }

    while (menu_active)
    {
        // Atualiza o contador de frames e altera a cor do título
        frame_counter++;
        if (frame_counter >= color_change_interval)
        {
            frame_counter = 0; // Reseta o contador de frames
            color_counter++;   // Muda a cor
            if (color_counter >= num_colors)
            {
                color_counter = 0; // Volta para a primeira cor
            }
        }

        // Movimento do fundo
        background_x -= 0.5f;
        if (background_x <= -al_get_bitmap_width(menu_background))
        {
            background_x = 0; // Reseta a posição do fundo
        }

        // Limpa a tela e desenha o fundo
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(menu_background, background_x, 0, 0);
        al_draw_bitmap(menu_background, background_x + al_get_bitmap_width(menu_background), 0, 0);

        // Desenha mensagens no display
        al_draw_text(font, colors[color_counter], 400, 100, ALLEGRO_ALIGN_CENTER, "Você foi Derrotado!");
        al_draw_text(menu_font, al_map_rgb(255, 255, 255), 400, 350, ALLEGRO_ALIGN_CENTER, "Pressione R para Reiniciar");
        al_draw_text(menu_font, al_map_rgb(255, 255, 255), 400, 450, ALLEGRO_ALIGN_CENTER, "Pressione Q para Sair");

        al_flip_display();

        // Processa eventos
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (ev.keyboard.keycode == ALLEGRO_KEY_R)
            {
                *restart_game = 1;
                menu_active = 0;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_Q)
            {
                *exit_game = 1;
                menu_active = 0;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            *exit_game = 1;
            menu_active = 0;
        }
    }

    // Liberação de recursos
    al_destroy_font(menu_font);
    al_destroy_bitmap(menu_background);
}


void show_transition_menu(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font, int *continue_game, int *exit_game)
{
    int menu_active = 1;
    int color_counter = 0;          // Contador para alternar entre cores
    int frame_counter = 0;          // Contador de frames para mudar a cor a cada 30 frames
    int color_change_interval = 30; // Intervalo de frames entre mudanças de cor
    ALLEGRO_COLOR colors[] = {
        al_map_rgb(255, 0, 0),    // Vermelho
        al_map_rgb(0, 255, 0),    // Verde
        al_map_rgb(0, 0, 255),    // Azul
        al_map_rgb(255, 255, 0),  // Amarelo
        al_map_rgb(255, 165, 0),  // Laranja
        al_map_rgb(255, 255, 255) // Branco
    };
    int num_colors = sizeof(colors) / sizeof(colors[0]);

    // Carregar o background
    ALLEGRO_BITMAP *background = al_load_bitmap("imagens/menu.jpg");
    if (!background)
    {
        fprintf(stderr, "Erro ao carregar o fundo!\n");
        return;
    }

    // Variáveis para controle do movimento do fundo
    float background_x = 0.0f;
    float background_speed = 0.2f; // Ajuste a velocidade do fundo conforme necessário

    while (menu_active)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        // Atualiza o contador de frames e altera a cor do título a cada 30 frames
        frame_counter++;
        if (frame_counter >= color_change_interval)
        {
            frame_counter = 0; // Reseta o contador de frames
            color_counter++;   // Muda a cor
            if (color_counter >= num_colors)
            {
                color_counter = 0; // Volta para a primeira cor
            }
        }

        // Movimento do fundo
        background_x -= background_speed;
        if (background_x <= -al_get_bitmap_width(background))
        {
            background_x = 0; // Reseta a posição do fundo para criar um looping contínuo
        }

        // Limpa a tela e desenha o fundo
        al_clear_to_color(al_map_rgb(0, 0, 0)); // Fundo preto

        // Desenha o fundo em movimento
        al_draw_bitmap(background, background_x, 0, 0);                                   // Desenha o fundo
        al_draw_bitmap(background, background_x + al_get_bitmap_width(background), 0, 0); // Desenha o fundo repetido para efeito de loop

        // Título com a cor que alterna a cada 30 frames
        al_draw_text(font, colors[color_counter], SCREEN_WIDTH / 2, 100, ALLEGRO_ALIGN_CENTER, "Boss Derrotado!");

        // Fonte menor para os botões de controle
        ALLEGRO_FONT *button_font = al_load_ttf_font("fonts/game_f.ttf", 35, 0);
        if (!button_font)
        {
            fprintf(stderr, "Erro ao carregar a fonte dos botões.\n");
            return;
        }

        al_draw_text(button_font, al_map_rgb(200, 200, 200), 400, 230, ALLEGRO_ALIGN_CENTER, "Pressione 1 para");
        al_draw_text(button_font, al_map_rgb(200, 200, 200), 400, 280, ALLEGRO_ALIGN_CENTER, "Avancar a Proxima Fase");
        al_draw_text(button_font, al_map_rgb(200, 200, 200), 400, 390, ALLEGRO_ALIGN_CENTER, "Pressione 2 para Sair");

        al_flip_display();

        // Detecta as ações do teclado
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (ev.keyboard.keycode == ALLEGRO_KEY_1)
            {
                *continue_game = 1;
                menu_active = 0;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_2)
            {
                *exit_game = 1;
                menu_active = 0;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            *exit_game = 1;
            menu_active = 0;
        }

        al_destroy_font(button_font); // Libera a fonte dos botões após desenhar
    }

    // Libera o bitmap de fundo
    al_destroy_bitmap(background);
}

void init_second_phase(Player *player, Enemy enemies[], Bullet bullets[], ShootingEnemy shooting_enemies[], Boss *boss, int *victory_state, int *player_won, double *start_time, int game_phase)
{
    boss->health = 23;
    boss->speed = 6;             // Velocidade vertical
    boss->horizontal_speed = 2;  // Velocidade horizontal mais lenta
    boss->y = SCREEN_HEIGHT / 4; // Posição inicial no topo
    boss->x = SCREEN_WIDTH; // Coloca o boss fora da tela
    *victory_state = 0;
    *player_won = 0;
    *start_time = al_get_time(); // Reinicia o tempo
    game_phase = 2;

    for (int i = 0; i < MAX_SHOOTING_ENEMIES; i++)
    {
        shooting_enemies[i].vertical_speed = 5;
    }

    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        enemies[i].active = 0;
        enemies[i].health = 2;
        enemies[i].vertical_speed = 1.0;                       // Velocidade vertical constante
        enemies[i].original_y = rand() % (SCREEN_HEIGHT - 30); // Posição inicial aleatória
        enemies[i].y = enemies[i].original_y;
        enemies[i].moving_up = 1; // Começar movendo para cima
    }

    // Inicialize as outras entidades da fase se necessário
    init_player(player);
    init_enemies(enemies, MAX_ENEMIES);
    init_bullets(bullets, MAX_BULLETS);


}

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
    exibir_texto_gradualmente("Parabens!", font_normal, 400, 180, al_map_rgb(255, 255, 255), 0.2, background_m, background_x);
    exibir_texto_gradualmente("Voce zerou o jogo!", font_normal, 400, 250, al_map_rgb(255, 255, 255), 0.2, background_m, background_x);

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

void exibir_texto_gradualmente(const char *texto, ALLEGRO_FONT *fonte, float x, float y, ALLEGRO_COLOR cor, float intervalo_entre_letras, ALLEGRO_BITMAP *background_m, float background_x)
    {
        char buffer[128] = ""; // Buffer para armazenar o texto que vai sendo construído
        int tamanho_texto = strlen(texto);

        for (int i = 0; i < tamanho_texto; i++)
        {
            // Adiciona a próxima letra ao buffer
            buffer[i] = texto[i];
            buffer[i + 1] = '\0'; // Garante que o buffer seja uma string válida

            // Redesenha o fundo e as frases já exibidas
            al_draw_bitmap(background_m, background_x, 0, 0);                                     // Fundo principal
            al_draw_bitmap(background_m, background_x + al_get_bitmap_width(background_m), 0, 0); // Fundo para looping

            // Desenha as frases na tela
            al_draw_text(fonte, cor, x, y, ALLEGRO_ALIGN_CENTER, buffer);

            al_flip_display();

            // Pausa para dar o efeito de digitação
            al_rest(intervalo_entre_letras);
        }
    }

    void exibir_tela_controles(ALLEGRO_FONT * font, ALLEGRO_BITMAP * background, ALLEGRO_EVENT_QUEUE * event_queue, ALLEGRO_DISPLAY * display)
    {
        float background_x = 0;
        int running = 1; // Controle do loop

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

                // Desenha o fundo e a tela de controles
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_bitmap(background, background_x, 0, 0);
                al_draw_bitmap(background, background_x + al_get_bitmap_width(background), 0, 0);
                al_draw_filled_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, al_map_rgba(0, 0, 0, 150));
                al_draw_text(font, al_map_rgb(0, 255, 255), 400, 20, ALLEGRO_ALIGN_CENTER, "CONTROLES");
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 120, ALLEGRO_ALIGN_CENTER, "MOVER: W, A, S, D");
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 220, ALLEGRO_ALIGN_CENTER, "ATIRAR: ENTER");
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 320, ALLEGRO_ALIGN_CENTER, "HABILIDADE ESPECIAL: E");
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 420, ALLEGRO_ALIGN_CENTER, "PAUSAR: P");
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 520, ALLEGRO_ALIGN_CENTER, "VOLTAR: ENTER");

                al_flip_display(); // Atualiza a tela
            }

            // Detecta ações do teclado
            else if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
            {
                running = 0; // Sai da tela de controles
            }
            else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                // Liberar recursos antes de sair
                al_destroy_display(display);         // Destruir o display
                al_destroy_event_queue(event_queue); // Destruir a fila de eventos
                exit(0);                             // Fechar o programa corretamente
            }
        }
    }

    void run_menu(ALLEGRO_FONT * font_menu, ALLEGRO_BITMAP * background, ALLEGRO_EVENT_QUEUE * event_queue, ALLEGRO_DISPLAY * display)
    {
        int menu_running = 1;
        int selected_option = 0;
        float background_x = 0;
        float background_speed = 1.0;

        // Carregar uma fonte maior para o título
        ALLEGRO_FONT *font_title = al_load_ttf_font("fonts/game_f.ttf", 72, 0);
        if (!font_title)
        {
            fprintf(stderr, "Erro ao carregar a fonte do título.\n");
            return;
        }

        ALLEGRO_BITMAP *background_m = al_load_bitmap("imagens/menu.jpg");

        int color_counter = 0;          // Contador para alternar entre cores
        int color_change_timer = 0;     // Timer para controlar a mudança de cor
        int color_change_interval = 30; // Intervalo de frames entre mudanças de cor (ajuste esse valor para alterar a velocidade)
        ALLEGRO_COLOR colors[] = {
            al_map_rgb(255, 0, 0),    // Vermelho
            al_map_rgb(0, 255, 0),    // Verde
            al_map_rgb(0, 0, 255),    // Azul
            al_map_rgb(255, 255, 0),  // Amarelo
            al_map_rgb(255, 165, 0),  // Laranja
            al_map_rgb(255, 255, 255) // Branco
        };
        int num_colors = sizeof(colors) / sizeof(colors[0]);

        while (menu_running)
        {
            ALLEGRO_EVENT ev;
            al_wait_for_event(event_queue, &ev);

            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                menu_running = 0;
            }
            else if (ev.type == ALLEGRO_EVENT_TIMER)
            {
                // Mover o fundo
                background_x -= background_speed;
                if (background_x <= -al_get_bitmap_width(background_m))
                {
                    background_x = 0;
                }

                // Desenhar fundo
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_bitmap(background_m, background_x, 0, 0);
                al_draw_bitmap(background_m, background_x + al_get_bitmap_width(background_m), 0, 0);

                // Atualizar o timer de mudança de cor
                color_change_timer++;
                if (color_change_timer >= color_change_interval)
                {
                    color_change_timer = 0; // Resetar o timer
                    color_counter++;        // Alterar para a próxima cor
                    if (color_counter >= num_colors)
                    {
                        color_counter = 0; // ALLEGRO_BITMAP *background = al_load_bitmap("imagens/background.png");Voltar para a primeira cor
                    }
                }

                // Mudar a cor do título
                ALLEGRO_COLOR current_color = colors[color_counter % num_colors]; // Alterna entre as cores

                // Desenhar título
                al_draw_text(font_title, current_color, 400, 130, ALLEGRO_ALIGN_CENTER, "Space Impact");

                // Desenhar opções do menu
                for (int i = 0; i < 3; i++)
                {
                    ALLEGRO_COLOR color = (i == selected_option) ? al_map_rgb(255, 255, 255) : al_map_rgb(0, 0, 0);
                    const char *text;
                    float pos_y = 320 + i * 80; // Maior distância entre os botões
                    if (i == 0)
                        text = "Começar";
                    else if (i == 1)
                        text = "Controles";
                    else
                        text = "Sair";
                    al_draw_textf(font_menu, color, 400, pos_y, ALLEGRO_ALIGN_CENTER, "%s", text);
                }

                al_flip_display();
            }
            else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                switch (ev.keyboard.keycode)
                {
                case ALLEGRO_KEY_DOWN:
                    selected_option = (selected_option + 1) % 3;
                    break;
                case ALLEGRO_KEY_UP:
                    selected_option = (selected_option + 2) % 3;
                    break;
                case ALLEGRO_KEY_ENTER:
                    if (selected_option == 0)
                    {
                        menu_running = 0; // Começar o jogo
                    }
                    else if (selected_option == 1)
                    {
                        exibir_tela_controles(font_menu, background_m, event_queue, display); // Exibir controles
                    }
                    else if (selected_option == 2)
                    {
                        menu_running = 0; // Sair do jogo
                        cleanup_menu_resources();
                        al_destroy_display(display);
                        al_destroy_event_queue(event_queue);
                        exit(0);
                    }
                    break;
                }
            }
        }

        // Limpar recursos da fonte do título
        al_destroy_font(font_title);
    }
