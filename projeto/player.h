#ifndef PLAYER_H
#define PLAYER_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include <time.h>

#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "collision.h"
#include "boss.h"
#include "structs.h"
#include "defines.h"

void init_player(Player *player);
void move_player(Player *player);

#endif // PLAYER_H
