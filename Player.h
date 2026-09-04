#ifndef __PLAYER__ 																												
#define __PLAYER__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include "Joystick.h"
#include "Platform.h"

#define MAX_VEL 16
#define PLAYER_X_ACC 2
#define PLAYER_Y_ACC 1
#define PLAYER_INV 45
#define PLAYER_WIDTH 40
#define PLAYER_HEIGHT 64
#define FRAME_WIDTH 100

#define RUN_FRAMES 7
#define JUMP_FRAMES 6
#define IDLE_FRAMES 4
#define DEAD_FRAMES 5
#define CLIMB_FRAMES 4
#define CROUCH_FRAMES 3
#define CROUCHWALK_FRAMES 4

typedef enum {
	RUNNING,
	JUMPING,
	IDLE,
	CLIMBING,
	CROUCHING,
	CROUCHWALK,
} playerSprite;

typedef struct {
	int hitPoints;
	unsigned int width;
	unsigned int height;
	unsigned short x;	
	unsigned short y;
	float xVelocity;
	float yVelocity;
	short jumpCount;
	short jumped;
	joystick *control;
	int invincibility;
	unsigned short canClimb;
	unsigned short isClimbing;
	unsigned short crouching;
	unsigned short leftLooking;
	unsigned short canStand;
	unsigned int frameCount;
	playerSprite sprite;
} player;

player* playerCreate(unsigned short x, unsigned short y);

void playerMove(player *element, unsigned char steps, unsigned int trajectory, unsigned short max_x, unsigned short max_y, long cameraDisplacement);	

void collisionPlayerPlatform(player *entityCharacter, platform *platformEntity);

void updatePlayerSprite(player *character);

void spriteFrameUpdate(player *character);

void playerRender(ALLEGRO_BITMAP *sprite, player *character);

void playerDestroy(player *element);																						

#endif																															