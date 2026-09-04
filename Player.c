#include <stdio.h>
#include <stdlib.h>
#include "Player.h"
#include "Joystick.h"

player* playerCreate(unsigned short x, unsigned short y){
	player *newPlayer = malloc(sizeof(player));
	if(!newPlayer)
		return NULL;
	newPlayer->width = PLAYER_WIDTH;
	newPlayer->height = PLAYER_HEIGHT;
	newPlayer->x = x + (PLAYER_WIDTH / 2);
	newPlayer->y = y + (PLAYER_HEIGHT / 2);
	newPlayer->xVelocity = 0;
	newPlayer->yVelocity = 0;
	newPlayer->jumpCount = 1;
	newPlayer->jumped = 0;
	newPlayer->control = joystick_create();
	newPlayer->hitPoints = 2;
	newPlayer->invincibility = 0;
	newPlayer->isClimbing = 0;
	newPlayer->crouching = 0;
	newPlayer->sprite = IDLE;
	newPlayer->leftLooking = 0;
	newPlayer->frameCount = 0;
	newPlayer->canStand = 1;

	return newPlayer;
}


void playerMove(player *entity, unsigned char steps, unsigned int trajectory, unsigned short max_x, unsigned short max_y, long cameraDisplacement){
	// O jogador agachado tem sua velocidade máxima limitada
	if(entity->crouching){
		if(entity->xVelocity < -MAX_VEL / 4)
			entity->xVelocity = -MAX_VEL / 4;

		else if(entity->xVelocity > MAX_VEL / 4)
			entity->xVelocity = MAX_VEL / 4;
	}


	// Caso a trajetória seja para a esquerda
	if(!trajectory){
		// Se o jogador está pulando, ou caindo, a movimentação é limitada
		if(entity->yVelocity != 0){
			if(entity->xVelocity > -MAX_VEL / 4)
				entity->xVelocity -= (0.5 * PLAYER_X_ACC);
		}
		// Caso contrário acelera para a esquerda.
		else{
			entity->leftLooking = 1;

			// Caso o jogador esteja agachado a aceleração é menor
			if(!entity->canStand){
				if(entity->xVelocity > -MAX_VEL / 4)
					entity->xVelocity -= (0.5 * PLAYER_X_ACC);
			}

			else{
				// Se for desaceleração, é mais rápido
				if(entity->xVelocity > 0)
					entity->xVelocity -= PLAYER_X_ACC;
				// Para a aceleração na direção é mais lento
				else if(entity->xVelocity > -MAX_VEL)
					entity->xVelocity -= 0.5 * PLAYER_X_ACC;
			}
		}

		// Atualiza a coordenada X do jogador.
		if((entity->x >= (steps * -entity->xVelocity) + entity->width / 2 + cameraDisplacement))
			entity->x += steps * entity->xVelocity;
		
	// Caso a trajetória seja para a direita
	} else if(trajectory == 1){
		// Se o jogador está pulando, ou caindo, a movimentação é limitada
		if(entity->yVelocity != 0){
			if(entity->xVelocity < MAX_VEL / 4)
				entity->xVelocity += (0.5 * PLAYER_X_ACC);
		}
		// Caso contrário acelera para a direita.
		else{
			entity->leftLooking = 0;

			// Caso o jogador esteja agachado a aceleração é menor
			if(!entity->canStand){	
				if(entity->xVelocity < MAX_VEL / 4)
					entity->xVelocity += (0.5 * PLAYER_X_ACC);
			}

			else{
				// Se for desaceleração, é mais rápido
				if(entity->xVelocity < 0)
					entity->xVelocity += PLAYER_X_ACC;
				// Para a aceleração na direção é mais lento
				else if(entity->xVelocity < MAX_VEL)
					entity->xVelocity += 0.5 * PLAYER_X_ACC;
			}
		}

		// Atualiza a coordenada X do jogador.
		if((entity->x >= (steps * -entity->xVelocity) + entity->width / 2 + cameraDisplacement))
			entity->x += steps * entity->xVelocity;


	// Caso a trajetória seja para cima
	} else if(trajectory == 2){
		// Se o jogador pode pular, velocidade máxima para cima
		if(entity->jumpCount)
			entity->yVelocity = -MAX_VEL;

		// Atualiza a coordenada Y
		if((entity->y >= (steps * entity->yVelocity) + entity->height / 2)){
			entity->y += steps * entity->yVelocity;
		}


	// Caso peça para agachar, mas ainda está tentando mover o personagem, sua movimentação será limitada
	} else if(trajectory == 4){
		// Caso queira se mover para a esquerda
		if(entity->control->left){
			entity->leftLooking = 1;
			if(entity->xVelocity > -MAX_VEL / 4)
				entity->xVelocity -= (0.5 * PLAYER_X_ACC);
		}

		// Caso queira se mover para a direita
		if(entity->control->right){
			entity->leftLooking = 0;
			if(entity->xVelocity < MAX_VEL / 4)
				entity->xVelocity += (0.5 * PLAYER_X_ACC);
		}

		// Atualiza a coordenada X do jogador.
		if((entity->x >= (steps * -entity->xVelocity) + entity->width / 2 + cameraDisplacement))
			entity->x += steps * entity->xVelocity;

	// Caso contrário, entramos no estado de inércia
	} else {
		// Se o jogador se move para a direita com velocidade diferente de 0, ela tende a diminiur, até chegar em 0.
		if(entity->xVelocity > 0){
			entity->xVelocity -= (0.5 * PLAYER_X_ACC);
			
			// Atualiza a coordenada X do jogador.
			entity->x += steps * entity->xVelocity;
		}
		// Se o jogador se move para a esquerda com velocidade diferente de 0, ela tende a aumentar, até chegar em 0.
		else if(entity->xVelocity < 0){
			entity->xVelocity += (0.5 * PLAYER_X_ACC);

			// Se o sprite do jogador, não ultrapassa o limite do começo da tela, move o sprite
			if((entity->x >= (steps * -entity->xVelocity) + entity->width / 2  + cameraDisplacement))
				entity->x += steps * entity->xVelocity;
			// Caso contrário, apenas atualiza as variáveis e trava ele no começo da tela
			else {
				entity->x = entity->width / 2 + cameraDisplacement;
				entity->xVelocity = 0;
			}
		}
	}

	// Efeito da gravidade
	if(!entity->isClimbing){
		if(entity->yVelocity < 0 && entity->jumped)
			entity->yVelocity += (0.8 * PLAYER_Y_ACC);
		else if(entity->yVelocity < MAX_VEL)
			entity->yVelocity += 2.5 * PLAYER_Y_ACC;

		entity->y += steps * entity->yVelocity;
	}
	// Movimentação para subir/descer escada, fixa para a velocidade máxima do jogador (sem aceleração)
	else{
		if(trajectory == 3){
			entity->yVelocity = 0;
			entity->y -= MAX_VEL / 3;
		}
		else if(trajectory == 4){
			entity->yVelocity = 0;
			entity->y += MAX_VEL / 3;
		}
	}
}


void collisionPlayerPlatform(player *entityCharacter, platform *platformEntity){
	int entityRight = entityCharacter->x + (entityCharacter->width / 2);
	int entityLeft = entityCharacter->x - (entityCharacter->width / 2);
	int entityTop = entityCharacter->y - (entityCharacter->height / 2);
	int entityBottom = entityCharacter->y + (entityCharacter->height / 2);


	int platformRight = platformEntity->x + platformEntity->width;
	int platformLeft = platformEntity->x;
	int platformTop = platformEntity->y;
	int platformBottom = platformEntity->y + platformEntity->height;


	// Colisão com escadas
	if(platformEntity->platformType == LADR){
		entityCharacter->canClimb = 0;	

		// Caso o jogador colida com a escada ele pode escalar
		if((platformRight > entityLeft) && (entityRight > platformLeft))
			if(entityBottom >= platformTop && entityBottom <= platformBottom + 3)
				entityCharacter->canClimb = 1;
		
		// Se ele não pode escalar, ou pulou, sai do estado de escalando
		if(!entityCharacter->canClimb || entityCharacter->control->jump)
			entityCharacter->isClimbing = 0;

		// Caso o jogador tente descer para além da base da escada, ele é impedido
		if(entityCharacter->control->down && entityBottom >= platformBottom){
			entityCharacter->isClimbing = 0;
			entityCharacter->y = platformBottom - entityCharacter->height / 2;
			entityCharacter->yVelocity = 0;
		}


		// Colisão por cima da escada
		if(!entityCharacter->control->down &&
		((platformRight + 2) > entityLeft) && (entityRight - 2 > platformLeft) && 
		(entityBottom - entityCharacter->yVelocity) <= platformTop && entityBottom > platformTop &&
		entityCharacter->yVelocity > 0){
			entityCharacter->y = platformTop - entityCharacter->height / 2;
			entityCharacter->yVelocity = 0;
			entityCharacter->jumpCount = 1;

			return;
		}
		// Caso contrário
		else{
			// Se o jogador tentar subir a escada, ele é travado em X ao centro da escada
			if(entityCharacter->canClimb && entityCharacter->control->up){
				entityCharacter->isClimbing = 1;
				entityCharacter->xVelocity = 0;
				entityCharacter->x = platformEntity->x + platformEntity->width / 2;
			}
			// Se o jogador tentar descer a escada, ele é travado em X ao centro da escada
			else if(entityCharacter->canClimb && entityCharacter->control->down &&
			entityBottom < platformBottom){
				entityCharacter->isClimbing = 1;
				entityCharacter->xVelocity = 0;
				entityCharacter->x = platformEntity->x + platformEntity->width / 2;
			}
		}
	}

	// Colisão com plataformas normais
	else{
		// Colisão por cima
		if(((platformRight - 4) > entityLeft) && (entityRight > platformLeft)){
			if(entityBottom - entityCharacter->yVelocity <= (platformTop + 2) && entityBottom > platformTop &&
			entityCharacter->yVelocity >= 0){
				entityCharacter->y = platformTop - entityCharacter->height / 2;
				entityCharacter->yVelocity = 0;
				entityCharacter->jumpCount = 1;

				return;
			}
		}

		// Colisão pela esquerda da plataforma
		if((entityRight >= platformLeft) && ((entityRight - entityCharacter->xVelocity) <= platformLeft) &&
		(entityBottom > platformTop) &&  (entityTop < platformBottom) &&
		entityCharacter->xVelocity > 0){
			entityCharacter->xVelocity = 0;
			entityCharacter->x = platformEntity->x - entityCharacter->width / 2;

			return;
		}

		// Colisão pela direita da plataforma
		else if((entityLeft <= platformRight) && ((entityLeft - entityCharacter->xVelocity) >= platformRight) &&
		(entityBottom > platformTop) &&  (entityTop < platformBottom) &&
		entityCharacter->xVelocity < 0){
			entityCharacter->xVelocity = 0;
			entityCharacter->x = platformRight + entityCharacter->width / 2;

			return;
		}

		// Colisão por baixo da plataforma
		if((platformRight > entityLeft) && (entityRight > platformLeft)){
			if(entityTop < platformBottom && (entityBottom > platformBottom) &&
			entityCharacter->yVelocity < 0){
				entityCharacter->y = platformBottom + entityCharacter->height / 2;
				entityCharacter->yVelocity = 0;

				return;
			}
			// Caso a colisão por baixo seja causada pelo jogador tentando levantar, mas não pode, não o deixa
			else if(entityTop < platformBottom + entityCharacter->height / 2 && (entityBottom > platformBottom) &&
			entityCharacter->yVelocity == 0)
				entityCharacter->canStand = 0;
		}
	}
	return;
}


void updatePlayerSprite(player *character){
	playerSprite oldSprite = character->sprite;

	// Se o jogador pulou e não está escalando
	if(!character->jumpCount && !character->isClimbing)
		character->sprite = JUMPING;
	
	// Escalando
	else if(character->isClimbing)
		character->sprite = CLIMBING;

	// Agachado
	else if(character->crouching){
		// Andando
		if(character->xVelocity != 0)
			character->sprite = CROUCHWALK;
		// Apenas agachado
		else
			character->sprite = CROUCHING;
	}
	else{
		// Correndo	
		if(character->xVelocity != 0)
			character->sprite = RUNNING;
		// Idle
		else{
			character->sprite = IDLE;
		}
	}

	// Caso o sprite troque, reseta o contador, para que a animação começe do zeros
	if(oldSprite != character->sprite)
		character->frameCount = 0;
}


void spriteFrameUpdate(player *character){
	// Toda vez que essa função é chamada atualiza o sprite do jogador
	character->frameCount++;
	// Como cada animação tem seu número de sprites, o módulo depende disso
	switch(character->sprite){
		case RUNNING:
			character->frameCount %= RUN_FRAMES;
			break;
		case CROUCHING:
			character->frameCount %= CROUCH_FRAMES;
			break;
		case CROUCHWALK:
			character->frameCount %= CROUCHWALK_FRAMES;
			break;
		case CLIMBING:
			character->frameCount %= CLIMB_FRAMES;
			break;
		default:
			character->frameCount %= IDLE_FRAMES;
	}
}


void playerRender(ALLEGRO_BITMAP *sprite, player *character){
	int flags = 0;
	// Atualiza para que o jogador esteja orientado a direção que se move
	if(character->leftLooking)
		flags = ALLEGRO_FLIP_HORIZONTAL;

	// Essa condição é para garantir que a animação "pisque" a cada 3 frames
	if(character->invincibility % 3 != 2){
		al_draw_bitmap_region(
			sprite,
			30 + character->frameCount * FRAME_WIDTH, PLAYER_HEIGHT - character->height,
			PLAYER_WIDTH, PLAYER_HEIGHT,
			character->x - (character->width / 2), character->y - (character->height / 2),	
			flags
		);
	}
}


void playerDestroy(player *entity){
	joystick_destroy(entity->control);
	free(entity);
}