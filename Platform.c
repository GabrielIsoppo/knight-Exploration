#include <stdio.h>
#include <stdlib.h>
#include "Platform.h"

platform* platformCreate(unsigned int width, unsigned int heigth, unsigned short x, unsigned short y, unsigned short platformType){
	platform *newPlatform = malloc(sizeof(platform));
	if(!newPlatform)
		return NULL;
	newPlatform->width = width;
	newPlatform->height = heigth;
	newPlatform->x = x;
	newPlatform->y = y;
	newPlatform->platformType = platformType;

	return newPlatform;
}

void platformRender(ALLEGRO_BITMAP *sprite, platform *platform){
	al_draw_bitmap_region(
		sprite,
		0, 0,
		platform->width, platform->height,
		platform->x, platform->y,	
		0
	);
}

void platformDestroy(platform *entity){
	free(entity);
}