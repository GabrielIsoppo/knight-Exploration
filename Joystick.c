#include <stdlib.h>
#include "Joystick.h"

joystick* joystick_create(){														//Implementação da função "joystick_create" (!)

	joystick *element = (joystick*) malloc (sizeof(joystick));						//Aloca memória na heap para um novo controle (!)
	element->right = 0;																//Insere o estado de desligado para o botão de movimentação à esquerda (!)
	element->left = 0;																//Insere o estado de desligado para o botão de movimentação à direita (!)
	element->up = 0;																//Insere o estado de desligado para o botão de movimentação para cima (!)
	element->down = 0;																//Insere o estado de desligado para o botão de movimentação para baixo (!)
	element->jump = 0;
	return element;																	//Retorna o novo controle (!)
}

void joystick_left(joystick *element, int status){
	element->left = element->left ^ 1;
}

void joystick_right(joystick *element, int status){
	element->right = element->right ^ 1;
}

void joystick_up(joystick *element, int status){
	element->up = element->up ^ 1;
}

void joystick_down(joystick *element, int status){
	element->down = element->down ^ 1;
}

void joystick_jump(joystick *element, int status){
	element->jump = element->jump ^ 1;
}

void joystick_destroy(joystick *element){
	free(element);
}