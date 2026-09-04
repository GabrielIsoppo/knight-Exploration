#include <stdio.h>
#include <stdlib.h>
#include "Movement.h"
#include "Player.h"
#include "Enemy.h"

void playerMovement(player *playerCharacter, enemy **enemyArray, platform **platformArray, long cameraMovement){
	unsigned int movementDirection = 0;
	int moved = 0;
	int enemyCollision = 0;
	enemy *currentEnemy;

	// Caso o jogador pressione apenas para andar para a esquerda, atualiza o movimento
	if(playerCharacter->control->left && !playerCharacter->control->right){
		movementDirection = 0;
		moved = 1;
	}

	// Caso o jogador pressione apenas para andar para a direita, atualiza o movimento
	if(playerCharacter->control->right && !playerCharacter->control->left){
		movementDirection = 1;
		moved = 1;
	}

	// Enquanto o jogador pressionar o botão de pular.
	if(playerCharacter->control->jump && !playerCharacter->isClimbing) {
		// Caso ele não tenha pulado (ou seja, segurado o botão), e tiver pulos disponíveis, o jogador pula
		if(playerCharacter->jumpCount && !playerCharacter->jumped && playerCharacter->canStand){
			if(playerCharacter->height != PLAYER_HEIGHT)
				playerCharacter->height = PLAYER_HEIGHT;

			playerMove(playerCharacter, 1, 2, X_SCREEN / 2, Y_SCREEN, cameraMovement);
			playerCharacter->canStand = 1;
			playerCharacter->jumpCount--;
			playerCharacter->jumped = 1;

			movementDirection = 5;
			moved = 1;
		}
	// Caso contrário reseta o valor de pulo, para ele poder pular novamente.
	} else 
		playerCharacter->jumped = 0;

	// O jogador só escala se pode e está pressionando o controle up
	if(playerCharacter->control->up && playerCharacter->canClimb){
		movementDirection = 3;
		moved = 1;
	}

	// Tratamento de controle down
	if (playerCharacter->control->down){
		// Caso o jogador não possa escalar, não esteja agachado e não tenha pulado
		if (!playerCharacter->canClimb && playerCharacter->height == PLAYER_HEIGHT && playerCharacter->jumpCount){
			// O jogador agacha
			playerCharacter->height = (3 * PLAYER_HEIGHT) / 4;
			playerCharacter->y += PLAYER_HEIGHT / 8;
			playerCharacter->crouching = 1;
		}
		// Caso contrário anda agachado
		else if((playerCharacter->control->right || playerCharacter->control->left) || (playerCharacter->isClimbing))
			moved = 1;
		movementDirection = 4;
	}
	else {
		// Se ele não está tentando agachar/descer, está agachado e pode levantar
		if (playerCharacter->crouching && playerCharacter->canStand){
			// Levanta o jogador
			playerCharacter->height = PLAYER_HEIGHT;
			playerCharacter->y -= PLAYER_HEIGHT / 8;
			playerCharacter->crouching = 0;
		}
	}

	// Caso o jogador se moveu e não foi pulando, realiza a movimentação.
	if(moved && movementDirection < 5)
		playerMove(playerCharacter, 1, movementDirection, X_SCREEN / 2, Y_SCREEN, cameraMovement);

	// Caso contrário, entra no caso de inércia.
	else
		playerMove(playerCharacter, 1, -1, X_SCREEN / 2, Y_SCREEN, cameraMovement);

	// Tratamento de colisão com inimigos
	for(int i = 0; i < MAX_ENEMY; i++){
		currentEnemy = *(enemyArray + i);
		enemyCollision = collisionEnemy(playerCharacter, currentEnemy);
		// Ao tratar a colisão de cada inimigo, garante que o jogador possa receber dano
		if(enemyCollision && !playerCharacter->invincibility){
			// Caso o dano seja causado pela lava, o jogador morre
			if(currentEnemy->enemyType == LAVA){
				playerCharacter->hitPoints = 0;
				return;
			}
			// Para outros inimigos ele recebe 1 de dano e ganha invencibilidade
			else{
				playerCharacter->hitPoints--;
				playerCharacter->invincibility = PLAYER_INV;
			}
		}
	}
	

	// Trata a colisão com o chão, para que o jogador não passe por dentro dele.
	// Além disso, trata a recuperação dos pulos, que só se dá ao colidir com o chão.
	platform *currentPlatform;
	
	playerCharacter->canStand = 1;
	for(int i = 0; i < MAX_PLAT; i++){
		currentPlatform = *(platformArray + i);
		collisionPlayerPlatform(playerCharacter, currentPlatform);
	}

	return;
}


void enemyMovement(player *playerCharacter, enemy **enemyArray, platform **platformArray, long cameraMovement){
	int playerColision = 0;

	// Atualiza a movimetação do inimigo. A função trata a direção da movimentação e a retorna.
	enemy *currentEnemy;
	for(int i = 0; i < MAX_ENEMY; i++){
		currentEnemy = *(enemyArray + i);
		// Atualiza a posição de cada inimigo
		if(currentEnemy->enemyType != IMM && currentEnemy->enemyType != LAVA)
			currentEnemy->movementXDirection = enemyMove(currentEnemy, 1, X_SCREEN, Y_SCREEN, cameraMovement);
		// Ao tratar a colisão de cada inimigo, garante que o jogador possa receber dano
		playerColision = collisionEnemy(playerCharacter, currentEnemy);
		if(playerColision && !playerCharacter->invincibility){
			if(currentEnemy->enemyType == LAVA){
				playerCharacter->hitPoints = 0;
				return;
			}
			// Para outros inimigos ele recebe 1 de dano e ganha invencibilidade
			else{
				playerCharacter->hitPoints--;
				playerCharacter->invincibility = PLAYER_INV;
			}
		}
	}

	// Trata a colisão com o chão, para que o inimigo não passe por dentro dele.
	platform *currentPlatform;
	for(int i = 0; i < MAX_PLAT; i++){
		currentPlatform = *(platformArray + i);
		for(int j = 0; j < MAX_ENEMY; j++){
			currentEnemy = *(enemyArray + j);
			// O único inimigo que pode entrar nas plataformas é o espinho
			if(currentEnemy->enemyType != SPK)
				collisionEnemyPlatform(currentEnemy, currentPlatform);
		}
	}

	return;
}


// Função geral que chama as demais
void updatePosition(player *playerCharacter, enemy **enemyArray, platform **platformArray, long cameraMovement){
	// Movimenta o jogador
	playerMovement(playerCharacter, enemyArray, platformArray, cameraMovement);
	// Se o jogador não tem pontos de vida, as funções apenas retornam
	if(!playerCharacter->hitPoints)
		return;

	// Movimenta os inimigos
	enemyMovement(playerCharacter, enemyArray, platformArray, cameraMovement);

	// Atualiza os frames de invencibilidade do jogador
	if(playerCharacter->invincibility)
		playerCharacter->invincibility--;
}