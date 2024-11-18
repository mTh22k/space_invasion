
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

void draw_pause_message(ALLEGRO_FONT *font, ALLEGRO_DISPLAY *display)
{
    al_clear_to_color(al_map_rgb(0, 0, 0)); // Limpa a tela com cor preta
    al_draw_text(font, al_map_rgb(255, 0, 0), 400, 300, ALLEGRO_ALIGN_CENTER, "Jogo Pausado");
    al_flip_display(); // Atualiza a tela
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

    while (menu_active)
    {
        al_clear_to_color(al_map_rgb(0, 0, 0)); // Fundo preto
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 20, ALLEGRO_ALIGN_CENTER, "Você foi Derrotado!");
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "Pressione 1 para Reiniciar");
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 20, ALLEGRO_ALIGN_CENTER, "Pressione 2 para Sair");

        al_flip_display();

        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (ev.keyboard.keycode == ALLEGRO_KEY_1)
            {
                *restart_game = 1;
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
    }
}

void show_transition_menu(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font, int *continue_game, int *exit_game)
{
    int menu_active = 1;

    while (menu_active)
    {
        al_clear_to_color(al_map_rgb(0, 0, 0)); // Fundo preto
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 20, ALLEGRO_ALIGN_CENTER, "Boss Derrotado!");
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "Pressione 1 para Avançar para a Próxima Fase");
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 20, ALLEGRO_ALIGN_CENTER, "Pressione 2 para Sair");

        al_flip_display();

        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

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
    }
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

void show_victory_message(ALLEGRO_FONT *font)
{
    al_clear_to_color(al_map_rgb(0, 0, 0)); // Limpa a tela com a cor preta

    // Desenha o texto de vitória
    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50, ALLEGRO_ALIGN_CENTER, "Você Ganhou!");

    // Desenha a opção de fechar o jogo
    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 50, ALLEGRO_ALIGN_CENTER, "Parabens.");

    al_flip_display(); // Exibe tudo na tela
}
