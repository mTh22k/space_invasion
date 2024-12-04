#include "enemy.h" // Inclui o cabeçalho com as definições e declarações para gerenciamento de inimigos

// Inicializa um array de inimigos básicos
void init_enemies(Enemy enemies[], int count)
{
    // Configura cada inimigo do array
    for (int i = 0; i < count; i++)
    {
        enemies[i].active = 0;                                 // Inimigo começa inativo
        enemies[i].health = 2;                                 // Define a saúde inicial
        enemies[i].vertical_speed = 1.0;                       // Velocidade vertical constante
        enemies[i].original_y = rand() % (SCREEN_HEIGHT - 30); // Posição inicial aleatória
        enemies[i].y = enemies[i].original_y;                  // Define a posição vertical
        enemies[i].moving_up = 1;                              // Começa movendo para cima
        enemies[i].exploding = 0;                              // Não está explodindo
        enemies[i].explosion_time = 0;                         // Tempo da explosão inicial
    }
}

// Atualiza o movimento dos inimigos básicos de acordo com a fase do jogo
void move_enemies(Enemy enemies[], int count, int game_phase)
{
    if (game_phase == 1) // Lógica de movimento para a fase 1
    {
        for (int i = 0; i < count; i++)
        {
            if (enemies[i].active)
            {
                enemies[i].x -= ENEMY_SPEED;               // Move horizontalmente para a esquerda
                enemies[i].y += enemies[i].vertical_speed; // Move verticalmente

                // Inverte a direção vertical ao atingir as bordas da tela
                if (enemies[i].y <= 0 || enemies[i].y >= SCREEN_HEIGHT - enemies[i].height)
                {
                    enemies[i].vertical_speed *= -1; // Inverte a direção vertical
                }

                // Reposiciona inimigos que saíram da tela pela esquerda
                if (enemies[i].x < -enemies[i].width)
                {
                    enemies[i].x = SCREEN_WIDTH + rand() % 100;
                    enemies[i].y = rand() % (SCREEN_HEIGHT - enemies[i].height);
                    enemies[i].vertical_speed = (rand() % 3 + 1) * (rand() % 2 == 0 ? 1 : -1);
                }
            }
        }
    }
    else if (game_phase == 2) // Lógica de movimento para a fase 2
    {
        for (int i = 0; i < count; i++)
        {
            if (enemies[i].active)
            {
                enemies[i].x -= 8;                         // Movimento horizontal mais lento
                enemies[i].y += enemies[i].vertical_speed; // Movimento vertical

                // Inverte a direção vertical ao atingir as bordas da tela
                if (enemies[i].y <= 0 || enemies[i].y >= SCREEN_HEIGHT - enemies[i].height)
                {
                    enemies[i].vertical_speed *= -1;
                }

                // Reposiciona inimigos que saíram da tela pela esquerda
                if (enemies[i].x < -enemies[i].width)
                {
                    enemies[i].x = SCREEN_WIDTH + rand() % 50;
                    enemies[i].y = rand() % (SCREEN_HEIGHT - enemies[i].height);
                    enemies[i].vertical_speed = rand() % 2 + 1; // Velocidade vertical reduzida
                }
            }
        }
    }
}

// Inicializa um inimigo que atira
void init_shooting_enemies(ShootingEnemy *enemy, int game_phase)
{
    enemy->x = SCREEN_WIDTH + rand() % 100;   // Posição inicial à direita da tela
    enemy->y = rand() % (SCREEN_HEIGHT - 30); // Posição vertical aleatória
    enemy->width = 100;                       // Largura do inimigo
    enemy->height = 50;                       // Altura do inimigo
    enemy->active = 1;                        // Ativa o inimigo
    enemy->health = 2;                        // Define a saúde inicial
    enemy->vertical_speed = 0.5;              // Velocidade vertical
    enemy->last_shot_time = 0;                // Tempo desde o último tiro
    enemy->moving_up = 1;                     // Começa movendo para cima
    enemy->explosion_time = 0;                // Tempo da explosão inicial
    enemy->exploding = 0;                     // Não está explodindo

    // Inicializa os projéteis do inimigo
    for (int i = 0; i < 4; i++)
    {
        enemy->bullets[i].active = 0;  // Inicia os projéteis como inativos
        enemy->bullets[i].width = 10;  // Define a largura dos projéteis
        enemy->bullets[i].height = 10; // Define a altura dos projéteis
        enemy->bullets[i].speed = 8;   // Define a velocidade dos projéteis
    }
}

// Atualiza o movimento de um inimigo que atira
void move_shooting_enemy(ShootingEnemy *enemy)
{
    if (enemy->active)
    {
        enemy->x -= ENEMY_SPEED_SHOOTING; // Movimento horizontal para a esquerda

        // Movimento vertical
        if (enemy->moving_up)
        {
            enemy->y -= enemy->vertical_speed; // Move para cima
            if (enemy->y <= 0)
                enemy->moving_up = 0; // Troca para mover para baixo
        }
        else
        {
            enemy->y += enemy->vertical_speed; // Move para baixo
            if (enemy->y >= SCREEN_HEIGHT - enemy->height)
                enemy->moving_up = 1; // Troca para mover para cima
        }

        // Reposiciona o inimigo que saiu da tela pela esquerda
        if (enemy->x < -enemy->width)
        {
            enemy->x = SCREEN_WIDTH + rand() % 100;
            enemy->y = rand() % (SCREEN_HEIGHT - enemy->height);
        }
    }
}

// Gera um inimigo básico em uma posição inicial válida
void generate_enemy(Enemy enemies[], int count, int player_width, int player_height)
{
    for (int i = 0; i < count; i++)
    {
        if (!enemies[i].active)
        {
            enemies[i].x = SCREEN_WIDTH + rand() % 100; // Posição inicial fora da tela
            enemies[i].y = rand() % SCREEN_HEIGHT;      // Posição vertical aleatória
            enemies[i].width = 70;                      // Largura do inimigo
            enemies[i].height = 40;                     // Altura do inimigo
            enemies[i].active = 1;                      // Ativa o inimigo
            enemies[i].health = 2;                      // Define a saúde inicial
            break;                                      // Gera apenas um inimigo por chamada
        }
    }
}

// Gera um inimigo que atira
void generate_shooting_enemy(ShootingEnemy enemies[], int count, int game_phase)
{
    for (int i = 0; i < count; i++)
    {
        if (!enemies[i].active)
        {
            init_shooting_enemies(&enemies[i], game_phase); // Inicializa o inimigo
            break;                                          // Gera apenas um inimigo por vez
        }
    }
}
