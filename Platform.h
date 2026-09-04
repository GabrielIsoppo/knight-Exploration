#ifndef __PLATFORM__ 																												//Guardas de inclusão (!)
#define __PLATFORM__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

#define MAX_PLAT 12
#define PLAT 0
#define LADR 1

typedef struct {																												//Definição da estrutura de um quadrado (!)
	unsigned int width;
	unsigned int height;																											//Tamanmho da lateral de um quadrado (!)
	unsigned short x;																											//Posição X do centro do quadrado (!)
	unsigned short y;																											//Posição Y do centro do quadrado (!)
	unsigned short platformType;
} platform;

platform* platformCreate(unsigned int width, unsigned int heigth, unsigned short x, unsigned short y, unsigned short platformType);		//Protótipo da função de criação de um quadrado (!)

void platformRender(ALLEGRO_BITMAP *sprite, platform *platform);

void platformDestroy(platform *element);

#endif