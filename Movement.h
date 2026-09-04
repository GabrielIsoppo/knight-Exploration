#ifndef __MOVEMENT__
#define __MOVEMENT__

#include "Player.h"
#include "Enemy.h"
#include "Platform.h"

// Dimensões da tela
#define X_SCREEN 1024
#define Y_SCREEN 768

void playerMovement(player *playerCharacter, enemy **enemyCharacter, platform **platformArray, long cameraMovement);

void enemyMovement(player *playerCharacter, enemy **enemyCharacter, platform **platformArray, long cameraMovement);

void updatePosition(player *playerCharacter, enemy **enemyArray, platform **platformArray, long cameraMovement);

#endif