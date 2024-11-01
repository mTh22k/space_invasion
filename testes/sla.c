#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FRAME_COUNT 6      // Número de frames da explosão
#define FRAME_DURATION 0.1 // Duração de cada frame em segundos

int main()
{
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_BITMAP *explosion_frames[FRAME_COUNT];
    int current_frame = 0;
    bool explosion_active = true;

    al_init();
    al_init_image_addon();

    display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);

    // Nomes dos arquivos de imagem para os frames da explosão
    const char *frame_files[FRAME_COUNT] = {
        "frame1.png",
        "frame2.png",
        "frame3.png",
        "frame4.png",
        "frame5.png",
        "frame6.png"};

    // Carregar os frames da explosão usando os nomes no array
    for (int i = 0; i < FRAME_COUNT; i++)
    {
        explosion_frames[i] = al_load_bitmap(frame_files[i]);
        if (!explosion_frames[i])
        {
            fprintf(stderr, "Erro ao carregar o arquivo %s\n", frame_files[i]);
            return -1;
        }
    }

    // Posição da explosão na tela
    int explosion_x = SCREEN_WIDTH / 2;
    int explosion_y = SCREEN_HEIGHT / 2;

    while (explosion_active)
    {
        al_clear_to_color(al_map_rgb(0, 0, 0));

        // Desenhar o frame atual da explosão
        al_draw_bitmap(explosion_frames[current_frame], explosion_x, explosion_y, 0);

        al_flip_display();
        al_rest(FRAME_DURATION);

        // Avançar para o próximo frame
        current_frame++;
        if (current_frame >= FRAME_COUNT)
        {
            explosion_active = false; // Fim da animação
        }
    }

    // Liberar os recursos
    for (int i = 0; i < FRAME_COUNT; i++)
    {
        al_destroy_bitmap(explosion_frames[i]);
    }
    al_destroy_display(display);
    return 0;
}
