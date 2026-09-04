#include <stdio.h>
#include <stdlib.h>
#include "Enemy.h"

enemy* enemyCreate(unsigned int width, unsigned int heigth, unsigned short x, unsigned short y, unsigned short enemyType, int gameDifficulty){
	enemy *newEnemy = malloc(sizeof(enemy));
	if(!newEnemy)
		return NULL;
	newEnemy->width = width;
	newEnemy->height = heigth;
	newEnemy->x = x;
	newEnemy->y = y;
	newEnemy->yVelocity = 0;
	newEnemy->movementXDirection = 0;
	newEnemy->movementYDirection = 0;
	newEnemy->enemyType = enemyType;
	newEnemy->frameCount = 0;
	newEnemy->spriteFrame = 0;
	newEnemy->spriteCount = 0;

	// Dependendo da dificuldade altera a velocidade máxima do inimigo
	if(!gameDifficulty)
		newEnemy->xMaxVelocity = ENEMY_X_VEL_E;
	else if(gameDifficulty == 1)
		newEnemy->xMaxVelocity = ENEMY_X_VEL_M;
	else
		newEnemy->xMaxVelocity = ENEMY_X_VEL_H;

	return newEnemy;
}


int enemyMove(enemy *entity, char steps, unsigned short max_x, unsigned short max_y, long cameraDisplacement){
	int movementXDirection = entity->movementXDirection;

	// Inimigos que se movem (não os que voam)
	if(entity->enemyType == MOV){
		// Se movimentam dependendo da orientação em X que estão no momento
		if(!movementXDirection){
			if(entity->x > (entity->xMaxVelocity + entity->width / 2))
				entity->x -= steps * entity->xMaxVelocity;
			else
				movementXDirection = 1;
		}
		else{
			if(entity->x + (entity->xMaxVelocity + entity->width / 2) < max_x + cameraDisplacement)
				entity->x += steps * entity->xMaxVelocity;
			else
				movementXDirection = 0;
		}

		// Efeito da gravidade para eles
		if(entity->yVelocity != ENEMY_Y_VEL)
			entity->yVelocity += ENEMY_ACC;
	}
	// Inimigos que voam
	else if(entity->enemyType == FLY){
		// Dependendo da orientação em Y que estão no momento
		if(!entity->movementYDirection){
			// Eles aceleram até 2X o valor máximo
			if(entity->yVelocity > 2 * -entity->xMaxVelocity)
				entity->yVelocity -= entity->xMaxVelocity / 2;
		}
		else{
			if(entity->yVelocity < 2 * entity->xMaxVelocity)
				entity->yVelocity += entity->xMaxVelocity / 2;
		}
		// Eles mantê, a direção de movimento pela mesma distância mesmo que a velocidade mude
		if(entity->frameCount >= 90 / (entity->xMaxVelocity / 2)){
			entity->movementYDirection = !entity->movementYDirection;
			entity->frameCount %= 90 / (entity->xMaxVelocity / 2);
		}
	}
	// Inimigo espinho
	else if(entity->enemyType == SPK){
		// O inimigo espinho tem um ciclo de 4 partes, que se altera dependendo da dificuldade
		if(entity->frameCount == 90 / (entity->xMaxVelocity / 2)){
			// Durante o 2° ciclo ele sobe
			if(entity->movementYDirection == 1){
				entity->yVelocity = -entity->xMaxVelocity / 2;
			}
			// Durante o 4° ciclo ele desce
			else if(entity->movementYDirection == 3){
				entity->yVelocity = entity->xMaxVelocity / 2;
				entity->movementYDirection = -1;
			}
			// Nos demais ciclos ele está oscioso.
			else{
				entity->yVelocity = 0;
			}

			entity->movementYDirection++;
			entity->frameCount = 0;
		}
	}
	// Atualiza a posição dele
	entity->y += steps * entity->yVelocity;

	return movementXDirection;
}


void enemyDestroy(enemy *entity){
	free(entity);
}


unsigned char collisionEnemy(player *playerCharacter, enemy *enemyEntity){
	int playerRight = playerCharacter->x + (playerCharacter->width / 2);
	int playerLeft = playerCharacter->x - (playerCharacter->width / 2);
	int playerTop = playerCharacter->y - (playerCharacter->height / 2);
	int playerBottom = playerCharacter->y + (playerCharacter->height / 2);

	int enemyRight = enemyEntity->x + (enemyEntity->width / 2);
	int enemyLeft = enemyEntity->x - (enemyEntity->width / 2);
	int enemyTop = enemyEntity->y - (enemyEntity->height / 2);
	int enemyBottom = enemyEntity->y + (enemyEntity->height / 2);

	// Caso as hitboxes do player ou do inimigo colidam retorna 1 avisando
	if ((((playerBottom >= enemyTop) && (enemyTop >= playerTop)) ||
	((enemyBottom >= playerTop) && (playerTop >= enemyTop))) &&
	(((playerRight >= enemyLeft) && (enemyLeft >= playerLeft)) ||
	((enemyRight >= playerLeft) && (playerLeft >= enemyLeft))))
		return 1;
	else
		return 0;
}


unsigned char collisionEnemyPlatform(enemy *entity, platform *platformEntity){
	int entityRight = entity->x + (entity->width / 2);
	int entityLeft = entity->x - (entity->width / 2);
	int entityTop = entity->y - (entity->height / 2);
	int entityBottom = entity->y + (entity->height / 2);


	int platformRight = platformEntity->x + platformEntity->width;
	int platformLeft = platformEntity->x;
	int platformTop = platformEntity->y;
	int platformBottom = platformEntity->y + platformEntity->height;


	// Colisão por cima
	if((platformRight > entityLeft) && (entityRight > platformLeft)){
		if(entityBottom > platformTop && entityTop < platformTop &&
		entity->yVelocity > 0){
			entity->y = platformTop - entity->height / 2;
			entity->yVelocity = 0;

			return 1;
		}
	}

	// Colisão pela esquerda da plataforma
	if((entityRight >= platformLeft) && (entityLeft < (platformLeft + 4)) &&
	(entityBottom > platformTop) &&  (entityTop < platformBottom) &&
	entity->movementXDirection){
		entity->movementXDirection = !entity->movementXDirection;
		entity->x = platformEntity->x - entity->width / 2;

		return 1;
	}

	// Colisão pela direita da plataforma
	else if((entityRight >= (platformRight - 4)) && (entityLeft < platformRight) &&
	(entityBottom > platformTop) &&  (entityTop < platformBottom) &&
	!entity->movementXDirection){
		entity->movementXDirection = !entity->movementXDirection;
		entity->x = platformRight + entity->width / 2;

		return 1;
	}

	// Colisão por baixo da plataforma
	if((platformRight > entityLeft) && (entityRight > platformLeft)){
		if(entityTop < platformBottom && (entityBottom > platformBottom) &&
		entity->yVelocity < 0){
			entity->y = platformBottom + entity->height / 2;
			entity->yVelocity = 0;

			return 1;
		}
	}

	return 0;
}


void updateSpriteFrame(enemy *enemyCharacter){
	// Os sprites são atualizados dependendo da dificuldade, para dar a sensação de aceleração
	if(enemyCharacter->spriteFrame == (12 - enemyCharacter->xMaxVelocity) / 2){
		enemyCharacter->spriteFrame %= (12 - enemyCharacter->xMaxVelocity) / 2;
		enemyCharacter->spriteCount++;
	}

	// Para cada inimigo, suas sprites tem número de frames diferentes
	switch(enemyCharacter->enemyType){
		case IMM:
			enemyCharacter->spriteCount %= IMM_FRAMES;
			break;
		case MOV:
			enemyCharacter->spriteCount %= MOV_FRAMES;
			break;
		case FLY:
			enemyCharacter->spriteCount %= FLY_FRAMES;
			break;
	}
}


void renderEnemy(ALLEGRO_BITMAP *sprite, enemy *enemyCharacter){
	int flags = 0;
	int spriteFrame = 0;
	// Condição para o inimigo sempre estar orientado a direção que se move
	if(!enemyCharacter->movementXDirection)
		flags = ALLEGRO_FLIP_HORIZONTAL;

	// Apenas os inimigos que tem animações precisam ser atualizados
	switch(enemyCharacter->enemyType){
		case IMM:
			updateSpriteFrame(enemyCharacter);
			spriteFrame = enemyCharacter->spriteCount;
			break;
		case MOV:
			updateSpriteFrame(enemyCharacter);
			spriteFrame = enemyCharacter->spriteCount;
			break;
		case FLY:
			updateSpriteFrame(enemyCharacter);
			spriteFrame = enemyCharacter->spriteCount;
			break;
		default:
			spriteFrame = 0;

	}

	// Após a atualização desenha o inimigo na tela
	al_draw_bitmap_region(
		sprite,
		spriteFrame * enemyCharacter->width, 0,
		enemyCharacter->width, enemyCharacter->height,
		enemyCharacter->x - (enemyCharacter->width / 2), enemyCharacter->y - (enemyCharacter->height / 2),	
		flags
	);
}