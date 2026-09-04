#ifndef __ENEMY__
#define __ENEMY__

// Variáveis gerais dos inimigos
#define MAX_ENEMY 9
#define ENEMY_X_VEL_E 2
#define ENEMY_X_VEL_M 4
#define ENEMY_X_VEL_H 6
#define ENEMY_Y_VEL 16
#define ENEMY_ACC 2

// Tipos de inimigos
#define IMM 0
#define MOV 1
#define FLY 2
#define LAVA 3
#define SPK 4

// número de sprites para cada tipo
#define IMM_FRAMES 4
#define MOV_FRAMES 6
#define FLY_FRAMES 7

// Dimensões de cada tipo
#define IMM_HEIGHT 48
#define IMM_WIDTH 48
#define MOV_HEIGHT 48
#define MOV_WIDTH 48
#define FLY_HEIGHT 72
#define FLY_WIDTH 63
#define SPK_HEIGHT 64
#define SPK_WIDTH 64

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include "Player.h"

typedef struct {
	unsigned int width;
	unsigned int height;
	unsigned short x;
	unsigned short y;
	int yVelocity;
	int xMaxVelocity;
	int movementXDirection;
	int movementYDirection;
	unsigned short enemyType;
	int frameCount;
	int spriteFrame;
	int spriteCount;
} enemy;

enemy* enemyCreate(unsigned int width, unsigned int heigth, unsigned short x, unsigned short y, unsigned short enemyType, int gameDifficulty);

int enemyMove(enemy *element, char steps, unsigned short max_x, unsigned short max_y, long cameraDisplacement);

void enemyDestroy(enemy *element);

unsigned char collisionEnemyPlatform(enemy *entity, platform *platformEntity);

unsigned char collisionEnemy(player *entityCharacter, enemy *enemyEntity);

void renderEnemy(ALLEGRO_BITMAP *sprite, enemy *enemyCharacter);

#endif