#include "boss.h"

void init_boss(Boss *boss)
{
    boss->x = SCREEN_WIDTH;      // Inicia fora da tela
    boss->y = SCREEN_HEIGHT / 2; // Posição vertical inicial
    boss->width = 200;           // Largura do chefe
    boss->height = 200;          // Altura do chefe
    boss->active = 0;            // Inicialmente inativo
    boss->speed = 2;             // Velocidade do movimento vertical
    boss->health = 20;
    boss->last_shot_time = 0;
    boss->last_special_attack_time = 0;
    boss->special_attack_active = 0;
    ;
}

void move_boss(Boss *boss, int game_phase)
{

    if (game_phase == 1)
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
    else if (game_phase == 2)
    {

        if (boss->active)
        {
            // Movimento vertical rápido
            boss->y += boss->speed * 2; // Velocidade aumentada no eixo vertical

            // Inverte a direção vertical ao atingir o topo ou fundo da tela
            if (boss->y <= 0 || boss->y >= SCREEN_HEIGHT - boss->height)
            {
                boss->speed = -boss->speed; // Inverte a direção vertical
            }

        }
    }
}

void shoot_boss_special_attack(Boss *boss, BossBullet boss_bullets[], int *boss_bullet_count)
{
    double current_time = al_get_time();

    // Verifica se é hora de iniciar o ataque especial
    if (!boss->special_attack_active && current_time - boss->last_special_attack_time >= 2.0)
    {
        boss->special_attack_active = 1; // Ativa o ataque especial
        boss->last_special_attack_time = current_time;

        // Dispara 6 balas em posições espaçadas verticalmente
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < MAX_BOSS_BULLETS; j++)
            {
                if (!boss_bullets[j].active)
                {
                    boss_bullets[j].x = boss->x;
                    boss_bullets[j].y = boss->y - 10 + i * 40; // Posições espaçadas verticalmente
                    boss_bullets[j].active = 1;
                    boss_bullets[j].speed = BULLET_SPEED * 2; // Velocidade aumentada
                    (*boss_bullet_count)++;
                    break;
                }
            }
        }
    }

    // Desativa o ataque especial após o intervalo
    if (boss->special_attack_active && current_time - boss->last_special_attack_time >= 1)
    {
        boss->special_attack_active = 0; // Desativa após meio segundo
    }
}

void shoot_boss_special_attack_2(Boss *boss, BossBullet boss_bullets[], int *boss_bullet_count)
{
    double current_time = al_get_time();

    // Verifica se é hora de iniciar o ataque especial
    if (!boss->special_attack_active && current_time - boss->last_special_attack_time >= 3.0)
    {
        boss->special_attack_active = 1; // Ativa o ataque especial
        boss->last_special_attack_time = current_time;

        // Dispara 6 balas em posições espaçadas verticalmente
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < MAX_BOSS_BULLETS; j++)
            {
                if (!boss_bullets[j].active)
                {
                    boss_bullets[j].x = boss->x;
                    boss_bullets[j].y = boss->y - 10 + i * 100; // Posições espaçadas verticalmente
                    boss_bullets[j].active = 1;
                    boss_bullets[j].speed = BULLET_SPEED * 1.5; // Velocidade aumentada
                    (*boss_bullet_count)++;
                    break;
                }
            }
        }
    }

    // Desativa o ataque especial após o intervalo
    if (boss->special_attack_active && current_time - boss->last_special_attack_time >= 0.5)
    {
        boss->special_attack_active = 0; // Desativa após meio segundo
    }
}

void shoot_boss_bullet(Boss *boss, BossBullet boss_bullets[], int *boss_bullet_count, int game_phase)
{
    double current_time = al_get_time();

    // Disparo normal
    if (current_time - boss->last_shot_time >= BOSS_SHOT_INTERVAL && !boss->special_attack_active)
    {
        for (int i = 0; i < MAX_BOSS_BULLETS; i++)
        {
            if (!boss_bullets[i].active)
            {
                boss_bullets[i].x = boss->x;
                boss_bullets[i].y = boss->y + boss->height / 2 - 20;
                boss_bullets[i].active = 1;
                boss_bullets[i].speed = BULLET_SPEED;
                boss->last_shot_time = current_time;
                (*boss_bullet_count)++;
                break;
            }
        }
    }

    // Ataque especial
    if (game_phase == 1)
    {
        shoot_boss_special_attack_2(boss, boss_bullets, boss_bullet_count);
    } else if (game_phase == 2)
    {
        shoot_boss_special_attack(boss, boss_bullets, boss_bullet_count);
    }
    
}

void check_and_activate_boss(Boss *boss, double *boss_start_time, double current_time, 
                             double *boss_shoot_start_time, bool *boss_waiting, int remaining_time, 
                             int screen_width) {
    // Verifica se o boss deve começar a esperar
    if (remaining_time <= 0 && !(*boss_waiting) && !(boss->active)) {
        *boss_waiting = true;           // Marca que estamos aguardando
        *boss_start_time = current_time; // Registra o tempo inicial da espera
    }

    // Aguarda os 3 segundos antes de ativar o boss
    if (*boss_waiting && (current_time - *boss_start_time >= 2.0)) {
        *boss_waiting = false;            // Reseta o estado de espera
        boss->active = 1;                 // Ativa o boss
        boss->x = screen_width - boss->width; // Posiciona o boss
        *boss_shoot_start_time = current_time; // Define o tempo inicial para começar a disparar
    }
}
