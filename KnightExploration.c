#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>														//Biblioteca base do Allegro
#include <allegro5/allegro_font.h>													//Biblioteca de fontes do Allegro
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>

#include "Player.h"
#include "Enemy.h"
#include "Platform.h"
#include "Joystick.h"
#include "Movement.h"


#define MENU 1
#define GAME 2
#define DEATH 3
#define WIN 4

#define GAME_END 4126


void backgroundRender(ALLEGRO_BITMAP *sprite, int backgroundNumber){
	// Renderiza o background a quantidade de vezes pedida
	for(int i = 0; i < backgroundNumber; i++){
		al_draw_scaled_bitmap(
			sprite,
			0, 0,
			al_get_bitmap_width(sprite), al_get_bitmap_height(sprite),
			1382 * i, 0,
			1382, 777,
			0
		);
	}
}


int menu(ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *queue, int *gameDifficulty, long *cameraDisplacement){
	ALLEGRO_EVENT event;
	ALLEGRO_BITMAP *background;
	ALLEGRO_FONT *font;
	ALLEGRO_FONT *title;

	// Fontes
	font = al_load_font("./Fonts/LowresPixel-Regular.otf", 36, 0);
	title = al_load_font("./Fonts/LowresPixel-Regular.otf", 60, 0);

	// Fundo
	background = al_load_bitmap("./Tiles/Backgrounds/Background.png");
		if(!background){
		printf("Erro ao carregar o background\n");
		return 0;
	}

	// Resete da camera, caso saia do jogo para o menu
	*cameraDisplacement = 0;
	ALLEGRO_TRANSFORM camera_transform;
	al_identity_transform(&camera_transform);
	al_translate_transform(&camera_transform, *cameraDisplacement, 0);
	al_use_transform(&camera_transform);

	int menuState = 0;
	int menuOption = 0;
	while(1){
		al_wait_for_event(queue, &event);											//Função que captura eventos da fila, inserindo os mesmos na variável de eventos
		
		// evento timer apenas atualiza a tela
		if(event.type == 30){
			al_flip_display();
		}
		// Menu principal com as opções de começar/fechar jogo e o menu de opções
		if(!menuState){
			// Renderiza o background para o menu
			backgroundRender(background, 1);

			// Textos da tela de menu
			al_draw_text(title, al_map_rgb(0, 0, 0), X_SCREEN / 2, Y_SCREEN / 2 - 200, ALLEGRO_ALIGN_CENTRE, "KNIGHT'S EXPLORATION");
			al_draw_text(font, al_map_rgb(0, 0, 0), X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTRE, "Começar Jogo");
			al_draw_text(font, al_map_rgb(0, 0, 0), X_SCREEN / 2, Y_SCREEN / 2 + 60, ALLEGRO_ALIGN_CENTRE, "Opções");
			al_draw_text(font, al_map_rgb(0, 0, 0), X_SCREEN / 2, Y_SCREEN / 2 + 120, ALLEGRO_ALIGN_CENTRE, "Fechar Jogo");

			// Dependendo da opção atual, atualiza a flecha (triângulo) que mostra qual é a selecionada na tela
			if(!menuOption){
				al_draw_filled_triangle(
					X_SCREEN / 2 - 190, Y_SCREEN / 2,
					X_SCREEN / 2 - 190, Y_SCREEN / 2 + 40,
					X_SCREEN / 2 - 150, Y_SCREEN / 2 + 20,
					al_map_rgb(0, 0, 0)
				);
			}
			else if(menuOption == 1){
				al_draw_filled_triangle(
					X_SCREEN / 2 - 190, Y_SCREEN / 2 + 60,
					X_SCREEN / 2 - 190, Y_SCREEN / 2 + 100,
					X_SCREEN / 2 - 150, Y_SCREEN / 2 + 80,
					al_map_rgb(0, 0, 0)
				);
			}
			else{
				al_draw_filled_triangle(
					X_SCREEN / 2 - 190, Y_SCREEN / 2 + 120,
					X_SCREEN / 2 - 190, Y_SCREEN / 2 + 160,
					X_SCREEN / 2 - 150, Y_SCREEN / 2 + 140,
					al_map_rgb(0, 0, 0)
				);
			}

			// Detecta o clique do mouse
			if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
				
				// Se o botão pressionado é o esquerdo, testa o local que foi clicado
				if (event.mouse.button == 1) {
					if(event.mouse.x > X_SCREEN / 2 - 130 && event.mouse.x < X_SCREEN / 2 + 130 &&
					event.mouse.y > Y_SCREEN / 2 - 10 && event.mouse.y < Y_SCREEN / 2 + 46){
						al_destroy_bitmap(background);
						return 2;
					}

					else if(event.mouse.x > X_SCREEN / 2 - 130 && event.mouse.x < X_SCREEN / 2 + 130 &&
					event.mouse.y > Y_SCREEN / 2 + 50 && event.mouse.y < Y_SCREEN / 2 + 106){
						menuState = 1;
						menuOption = 0;
					}

					else if(event.mouse.x > X_SCREEN / 2 - 130 && event.mouse.x < X_SCREEN / 2 + 130 &&
					event.mouse.y > Y_SCREEN / 2 + 110 && event.mouse.y < Y_SCREEN / 2 + 166){
						al_destroy_bitmap(background);
						return 0;
					}
				}
			}
			
			// Detecta o movimento do mouse
			else if (event.type == ALLEGRO_EVENT_MOUSE_AXES){
					// Dependendo do local que o mouse passa atualiza qual opção de menu é a selecionada
					if(event.mouse.x > X_SCREEN / 2 - 130 && event.mouse.x < X_SCREEN / 2 + 130 &&
					event.mouse.y > Y_SCREEN / 2 - 10 && event.mouse.y < Y_SCREEN / 2 + 46)
						menuOption = 0;

					else if(event.mouse.x > X_SCREEN / 2 - 130 && event.mouse.x < X_SCREEN / 2 + 130 &&
					event.mouse.y > Y_SCREEN / 2 + 50 && event.mouse.y < Y_SCREEN / 2 + 106)
						menuOption = 1;

					else if(event.mouse.x > X_SCREEN / 2 - 130 && event.mouse.x < X_SCREEN / 2 + 130 &&
					event.mouse.y > Y_SCREEN / 2 + 110 && event.mouse.y < Y_SCREEN / 2 + 166)
						menuOption = 2;
			}

			// Se uma tecla foi clicada no teclado
			else if(event.type == ALLEGRO_EVENT_KEY_DOWN){
				// Se a tecla foi enter, atualiza dependendo de qual opção do menu é a selecionada
				if(event.keyboard.keycode == ALLEGRO_KEY_ENTER){
					if(!menuOption){
						al_destroy_bitmap(background);
						return GAME;
					}
					else if(menuOption == 1){
						menuState = 1;
						menuOption = 0;
					}
					else{
						al_destroy_bitmap(background);
						return 0;
					}
				}
				// W/seta para cima atualiza a opção do menu
				else if(event.keyboard.keycode == ALLEGRO_KEY_UP || event.keyboard.keycode == ALLEGRO_KEY_W){
					if(menuOption > 0)
						menuOption = (menuOption - 1) % 3;
					else
						menuOption = 2;
				}
				// S/seta para baixo atualiza a opção do menu
				else if(event.keyboard.keycode == ALLEGRO_KEY_DOWN || event.keyboard.keycode == ALLEGRO_KEY_S){
						menuOption = (menuOption + 1) % 3;
				}
			}
		}
		// Menu de opções com as dificuldades
		else{
			// Renderiza o background do jogo
			al_draw_scaled_bitmap(
				background,
				0, 0,
				al_get_bitmap_width(background), al_get_bitmap_height(background),
				0, 0,
				1382, 777,
				0
			);
			
			// Textos da tela de menu
			al_draw_text(title, al_map_rgb(0, 0, 0), X_SCREEN / 2, Y_SCREEN / 2 - 200, ALLEGRO_ALIGN_CENTRE, "DIFICULDADE");
			al_draw_text(font, al_map_rgb(0, 0, 0), X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTRE, "Fácil");
			al_draw_text(font, al_map_rgb(0, 0, 0), X_SCREEN / 2, Y_SCREEN / 2 + 60, ALLEGRO_ALIGN_CENTRE, "Médio");
			al_draw_text(font, al_map_rgb(0, 0, 0), X_SCREEN / 2, Y_SCREEN / 2 + 120, ALLEGRO_ALIGN_CENTRE, "Difícil");

			// Dependendo da opção atual, atualiza a flecha (triângulo) que mostra qual é a selecionada na tela
			if(!menuOption){
				al_draw_filled_triangle(
					X_SCREEN / 2 - 130, Y_SCREEN / 2,
					X_SCREEN / 2 - 130, Y_SCREEN / 2 + 40,
					X_SCREEN / 2 - 90, Y_SCREEN / 2 + 20,
					al_map_rgb(0, 0, 0)
				);
			}
			else if(menuOption == 1){
				al_draw_filled_triangle(
					X_SCREEN / 2 - 130, Y_SCREEN / 2 + 60,
					X_SCREEN / 2 - 130, Y_SCREEN / 2 + 100,
					X_SCREEN / 2 - 90, Y_SCREEN / 2 + 80,
					al_map_rgb(0, 0, 0)
				);
			}
			else{
				al_draw_filled_triangle(
					X_SCREEN / 2 - 130, Y_SCREEN / 2 + 120,
					X_SCREEN / 2 - 130, Y_SCREEN / 2 + 160,
					X_SCREEN / 2 - 90, Y_SCREEN / 2 + 140,
					al_map_rgb(0, 0, 0)
				);
			}

			// Detecta o clique do mouse
			if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){

				// Se o botão pressionado é o esquerdo, testa o local que foi clicado
				if (event.mouse.button == 1) {
					if(event.mouse.x > X_SCREEN / 2 - 70 && event.mouse.x < X_SCREEN / 2 + 70 &&
					event.mouse.y > Y_SCREEN / 2 - 10 && event.mouse.y < Y_SCREEN / 2 + 46)
						*gameDifficulty = 0;
					

					else if(event.mouse.x > X_SCREEN / 2 - 70 && event.mouse.x < X_SCREEN / 2 + 70 &&
					event.mouse.y > Y_SCREEN / 2 + 50 && event.mouse.y < Y_SCREEN / 2 + 106)
						*gameDifficulty = 1;
					

					else if(event.mouse.x > X_SCREEN / 2 - 70 && event.mouse.x < X_SCREEN / 2 + 70 &&
					event.mouse.y > Y_SCREEN / 2 + 110 && event.mouse.y < Y_SCREEN / 2 + 166)
						*gameDifficulty = 2;

					menuState = 0;
					menuOption = 0;
				}
			}

			// Detecta o movimento do mouse
			else if (event.type == ALLEGRO_EVENT_MOUSE_AXES){
					// Dependendo do local que o mouse passa atualiza qual opção de menu é a selecionada
					if(event.mouse.x > X_SCREEN / 2 - 130 && event.mouse.x < X_SCREEN / 2 + 130 &&
					event.mouse.y > Y_SCREEN / 2 - 10 && event.mouse.y < Y_SCREEN / 2 + 46)
						menuOption = 0;

					else if(event.mouse.x > X_SCREEN / 2 - 130 && event.mouse.x < X_SCREEN / 2 + 130 &&
					event.mouse.y > Y_SCREEN / 2 + 50 && event.mouse.y < Y_SCREEN / 2 + 106)
						menuOption = 1;

					else if(event.mouse.x > X_SCREEN / 2 - 130 && event.mouse.x < X_SCREEN / 2 + 130 &&
					event.mouse.y > Y_SCREEN / 2 + 110 && event.mouse.y < Y_SCREEN / 2 + 166)
						menuOption = 2;
			}

			// Se uma tecla foi clicada no teclado
			else if(event.type == ALLEGRO_EVENT_KEY_DOWN){
				// Se a tecla foi enter, atualiza a dificuldade para a selecionada
				if(event.keyboard.keycode == ALLEGRO_KEY_ENTER){
					*gameDifficulty = menuOption;
					menuState = 0;
					menuOption = 0;
				}
				// W/seta para cima atualiza a opção do menu
				else if(event.keyboard.keycode == ALLEGRO_KEY_UP || event.keyboard.keycode == ALLEGRO_KEY_W){
					if(menuOption > 0)
						menuOption = (menuOption - 1) % 3;
					else
						menuOption = 2;
				}
				// S/seta para baixo atualiza a opção do menu
				else if(event.keyboard.keycode == ALLEGRO_KEY_DOWN || event.keyboard.keycode == ALLEGRO_KEY_S){
						menuOption = (menuOption + 1) % 3;
				}
			}
		}
		// Clicar no "x" da tela fecha o display
		if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
			al_destroy_bitmap(background);
			return 0;
		}
	}

	al_destroy_font(font);															//Destrutor da fonte
	al_destroy_font(title);															//Destrutor da fonte
}


int game(ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *queue, int gameDifficulty, long *cameraDisplacement){
	// Variáveis gerais
	int frameCount = 0;
	int frameWidth = 64;
	int frameHeight = 96;
	int pause = 0;
	int pausePressed = 0;


	// Jogador
	int playerXPosition = 0;
	int playerYPosition = 572;
	player *playerCharacter;
	ALLEGRO_BITMAP *idle;
	ALLEGRO_BITMAP *running;
	ALLEGRO_BITMAP *climbing;
	ALLEGRO_BITMAP *jumping;
	ALLEGRO_BITMAP *dying;
	ALLEGRO_BITMAP *crouching;
	ALLEGRO_BITMAP *crouchWalk;


	// Inimigos
	int enemyWidth[MAX_ENEMY] = {MOV_WIDTH, SPK_WIDTH, IMM_WIDTH, IMM_WIDTH, MOV_WIDTH, FLY_WIDTH, MOV_WIDTH, FLY_WIDTH, 330};
	int enemyHeight[MAX_ENEMY] = {MOV_HEIGHT, SPK_HEIGHT, IMM_HEIGHT, IMM_HEIGHT, MOV_HEIGHT, FLY_HEIGHT, MOV_HEIGHT, FLY_HEIGHT, 150};
	int enemyXPosition[MAX_ENEMY] = {700, 2200, 400, 3040, 650, 2840, 1800, 3240, 1050};
	int enemyYPosition[MAX_ENEMY] = {636, 490, 475, 525, 347, 560, 636, 647, 760};
	unsigned short enemyMovementType[MAX_ENEMY] = {MOV, SPK, IMM, IMM, MOV, FLY, MOV, FLY, LAVA};
	enemy **enemyArray;
	ALLEGRO_BITMAP *lava;
	ALLEGRO_BITMAP *movable;
	ALLEGRO_BITMAP *flying;
	ALLEGRO_BITMAP *spike;
	ALLEGRO_BITMAP *immovable;


	// Plataformas (flutuantes/chão)
	int platformHeight[MAX_PLAT] = {300, 30, 30, 350, 300, 30, 400, 30, 30, 300, 380, 236};
	int platformWidth[MAX_PLAT] = {900, 300, 300, 50, 900, 300, 400, 300, 300, 1030, 200, 50};
	int platformXPosition[MAX_PLAT] = {0, 300, 500, 850, 1230, 1550, 2000, 2480, 2890, 3300, 2100, 1500};
	int platformYPosition[MAX_PLAT] = {636, 500, 370, 586, 636, 400, 430, 450, 550, 636, 0, 400};
	unsigned short platformType[MAX_PLAT] = {PLAT, PLAT, PLAT, PLAT, PLAT, PLAT, PLAT, PLAT, PLAT, PLAT, PLAT, LADR};
	platform **platformArray;
	ALLEGRO_BITMAP *ground;
	ALLEGRO_BITMAP *ladder;

	// Câmera
	ALLEGRO_BITMAP *background;
	ALLEGRO_TRANSFORM camera_transform;
	

	ALLEGRO_EVENT event;
	ALLEGRO_FONT *font; // Fonte para a tela

	font = al_load_font("./Fonts/LowresPixel-Regular.otf", 36, 0);


	// Fundo da tela
	background = al_load_bitmap("./Tiles/Backgrounds/Background.png");
		if(!background){
		printf("Erro ao carregar o background\n");
		return 0;
	}

	// Jogador e suas sprites
	playerCharacter = playerCreate(playerXPosition, playerYPosition);
	idle = al_load_bitmap("./Sprites/Player/Idle.png");
	if(!idle){
		printf("Erro ao carregar os sprites do personagem\n");
		return 0;
	}
	running = al_load_bitmap("./Sprites/Player/Run.png");
	if(!running){
		printf("Erro ao carregar os sprites do personagem\n");
		return 0;
	}
	jumping = al_load_bitmap("./Sprites/Player/Jump.png");
	if(!jumping){
		printf("Erro ao carregar as sprites do personagem.\n");
		return 0;
	}
	climbing = al_load_bitmap("./Sprites/Player/Climb.png");
	if(!climbing){
		printf("Erro ao carregar as sprites do personagem.\n");
		return 0;
	}
	crouching = al_load_bitmap("./Sprites/Player/Crouch.png");
	if(!crouching){
		printf("Erro ao carregar as sprites do personagem.\n");
		return 0;
	}
	crouchWalk = al_load_bitmap("./Sprites/Player/CrouchWalk.png");
	if(!crouchWalk){
		printf("Erro ao carregar as sprites do personagem.\n");
		return 0;
	}


	// Vetor de inimigos e suas sprites
	enemyArray = malloc(MAX_ENEMY * sizeof(enemy));
	if(!enemyArray){
		printf("Erro ao criar o vetor de inimigos.\n");
		return 0;
	}
	for(int i = 0; i < MAX_ENEMY; i++){
		*(enemyArray + i) = enemyCreate(enemyWidth[i], enemyHeight[i], enemyXPosition[i], enemyYPosition[i], enemyMovementType[i], gameDifficulty);
	}
	lava = al_load_bitmap("./Sprites/Enemies/Lava.png");
	if(!lava){
		printf("Erro ao carregar as sprites do inimigo.\n");
		return 0;
	}
	movable = al_load_bitmap("./Sprites/Enemies/Movable.png");
	if(!movable){
		printf("Erro ao carregar as sprites do inimigo.\n");
		return 0;
	}
	flying = al_load_bitmap("./Sprites/Enemies/Flying.png");
	if(!flying){
		printf("Erro ao carregar as sprites do inimigo.\n");
		return 0;
	}
	spike = al_load_bitmap("./Sprites/Enemies/Spike.png");
	if(!spike){
		printf("Erro ao carregar as sprites do inimigo.\n");
		return 0;
	}
	immovable = al_load_bitmap("./Sprites/Enemies/Immovable.png");
	if(!immovable){
		printf("Erro ao carregar as sprites do inimigo.\n");
		return 0;
	}


	// Vetor de plataformas e suas sprites
	platformArray = malloc(MAX_PLAT * sizeof(platform));
	if(!platformArray){
		printf("Erro ao criar o vetor de plataformas.\n");
		return 0;
	}
	for(int i = 0; i < MAX_PLAT; i++){
		*(platformArray + i) = platformCreate(platformWidth[i], platformHeight[i], platformXPosition[i], platformYPosition[i], platformType[i]);
	}
	ground = al_load_bitmap("./Tiles/Tileset/Platform.png");
	if(!ground){
		printf("Erro ao carregar as sprites da plataforma.\n");
		return 0;
	}
	ladder = al_load_bitmap("./Tiles/Tileset/Ladder.png");
	if(!ladder){
		printf("Erro ao carregar as sprites da plataforma.\n");
		return 0;
	}


	// Loop do jogo
	while(1){
		al_wait_for_event(queue, &event);											//Função que captura eventos da fila, inserindo os mesmos na variável de eventos

		// Renderiza o background do jogo
		backgroundRender(background, 3);

		// Renderiza os inimigos e suas hitboxs
		enemy *currentEnemy;
		for(int i = 0; i < MAX_ENEMY; i++){
			currentEnemy = *(enemyArray + i);				
			
			// Dependendo o tipo do inimigo renderiza a sprite certa
			switch(currentEnemy->enemyType){
				case IMM:
					renderEnemy(immovable, currentEnemy);
					break;
				case MOV:
					renderEnemy(movable, currentEnemy);
					break;
				case FLY:
					renderEnemy(flying, currentEnemy);
					break;
				case SPK:
					renderEnemy(spike, currentEnemy);
					break;
				case LAVA:
					renderEnemy(lava, currentEnemy);
					break;
			}
		}



		// Renderiza o chão da fase
		platform *currentPlatform;
		for(int i = 0; i < MAX_PLAT; i++){
			currentPlatform = *(platformArray + i);
			switch(currentPlatform->platformType){
			case PLAT:
				platformRender(ground, currentPlatform);
				break;
			case LADR:
				platformRender(ladder, currentPlatform);
				break;
			}
		}
		
		
		// Renderiza o player e a sprite correta
		switch(playerCharacter->sprite){
			case RUNNING:
				playerRender(running, playerCharacter);
				break;
			case JUMPING:
				playerCharacter->frameCount = 2;
				playerRender(jumping, playerCharacter);
				break;
			case CLIMBING:
				playerRender(climbing, playerCharacter);
				break;
			case CROUCHING:
				playerCharacter->frameCount = 2;
				playerRender(crouching, playerCharacter);
				break;
			case CROUCHWALK:
				playerRender(crouchWalk, playerCharacter);
				break;
			default:
				playerRender(idle, playerCharacter);
		}


		// HUD (hp no canto da tela)
		al_draw_textf(font, al_map_rgb(0, 0, 0), *cameraDisplacement + 60, 20, ALLEGRO_ALIGN_CENTRE, "HP: %d", playerCharacter->hitPoints);
		
		// Se o jogo não está pausado e o jogador tem hp
		if(!pause && playerCharacter->hitPoints){		
			if (event.type == ALLEGRO_EVENT_TIMER){		//O evento tipo 30 indica um evento de relógio, ou seja, verificação se a tela deve ser atualizada (conceito de FPS)
				// Apenas quando o evento é do timer que as posições são atualizadas
				updatePosition(playerCharacter, enemyArray, platformArray, *cameraDisplacement);

				// Atualiza o contador de frames
				frameCount++;
				// As sprites do jogador são atualizadas a cada 5 frames
				if(frameCount == 5){
					frameCount %= 5;
					spriteFrameUpdate(playerCharacter);
				}

				// Se não está pausado atualiza os sprites do inimigo e do player
				if(!pause){
					for(int i = 0; i < MAX_ENEMY; i++){
						currentEnemy = *(enemyArray + i);
						currentEnemy->frameCount++;
						currentEnemy->spriteFrame++;
					}
					updatePlayerSprite(playerCharacter);
				}

				// Caso o jogador morra, sai do jogo e vai para a tela de morte
				if(!playerCharacter->hitPoints || playerCharacter->y > Y_SCREEN){
					return DEATH;
				}

				// Caso o jogador chegue ao final da fase, sai do jogo e vai para a tela de vitória
				if(playerCharacter->x >= GAME_END - 50)
					return WIN;

				/* A câmera é atualizada todas vez que o jogador passa da metade da tela e está com velocidade positiva
				A tela não volta para trás após se mover para frente */
				if(playerCharacter->xVelocity > 0 &&
				playerCharacter->x + (playerCharacter->width / 2) >= (X_SCREEN / 2) + *cameraDisplacement &&
				playerCharacter->x + (playerCharacter->width / 2) < GAME_END - (X_SCREEN / 2)){
					*cameraDisplacement += playerCharacter->xVelocity;
				}

				// Realiza a movimentação em si da tela
				al_identity_transform(&camera_transform);
				al_translate_transform(&camera_transform, -(*cameraDisplacement), 0);
				al_use_transform(&camera_transform);

				al_flip_display();	//Insere as modificações realizadas nos buffers de tela
			}

			// Atualiza os controles se uma das teclas clicadas for uma dos controles
			else if(event.type == ALLEGRO_EVENT_KEY_DOWN){		
				if(event.keyboard.keycode == ALLEGRO_KEY_A || event.keyboard.keycode == ALLEGRO_KEY_LEFT) joystick_left(playerCharacter->control, 1);
				else if(event.keyboard.keycode == ALLEGRO_KEY_D || event.keyboard.keycode == ALLEGRO_KEY_RIGHT) joystick_right(playerCharacter->control, 1);
				else if(event.keyboard.keycode == ALLEGRO_KEY_W || event.keyboard.keycode == ALLEGRO_KEY_UP) joystick_up(playerCharacter->control, 1);
				else if(event.keyboard.keycode == ALLEGRO_KEY_S || event.keyboard.keycode == ALLEGRO_KEY_DOWN) joystick_down(playerCharacter->control, 1);
				else if(event.keyboard.keycode == ALLEGRO_KEY_SPACE) joystick_jump(playerCharacter->control, 1);

				// O escape pausa o jogo
				else if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE && !pausePressed){
					pause = 1;
					pausePressed = 1;
				}
			}

			// Atualiza os controles se uma das teclas soltas for uma dos controles
			else if(event.type == ALLEGRO_EVENT_KEY_UP){
				if(event.keyboard.keycode == ALLEGRO_KEY_A || event.keyboard.keycode == ALLEGRO_KEY_LEFT) 
					joystick_left(playerCharacter->control, 0);
				else if(event.keyboard.keycode == ALLEGRO_KEY_D || event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
					joystick_right(playerCharacter->control, 0);
				else if(event.keyboard.keycode == ALLEGRO_KEY_W || event.keyboard.keycode == ALLEGRO_KEY_UP)
					joystick_up(playerCharacter->control, 0);
				else if(event.keyboard.keycode == ALLEGRO_KEY_S || event.keyboard.keycode == ALLEGRO_KEY_DOWN)
					joystick_down(playerCharacter->control, 0);
				else if(event.keyboard.keycode == ALLEGRO_KEY_SPACE)
					joystick_jump(playerCharacter->control, 0);
				// A possibilidade de despausar ou pausar só é possível quando a tecla é solta
				else if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
					pausePressed = 0;
			}
		}
		else if(pause){
			if(event.type == ALLEGRO_EVENT_TIMER){
				al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
				al_draw_filled_rectangle(0, 0, X_SCREEN + *cameraDisplacement, Y_SCREEN, al_map_rgba(0, 0, 0, 150));

				al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2 + *cameraDisplacement, Y_SCREEN / 2 - 70, ALLEGRO_ALIGN_CENTRE, "JOGO PAUSADO");
				al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2 + *cameraDisplacement, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTRE, "Pressione 'ESC' para Voltar");
				al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2 + *cameraDisplacement, Y_SCREEN / 2 + 50, ALLEGRO_ALIGN_CENTRE, "Pressione 'Q' para Voltar para o menu");

				al_flip_display();
			}
			
			// Durante o pause existe apenas duas opções
			else if(event.type == ALLEGRO_EVENT_KEY_DOWN){
				// Despausar pressionando escape novamente
				if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE && !pausePressed){
					pause = 0;
					pausePressed = 1;
				}
				// Voltar ao menu principal pressionando a tecla "Q"
				else if(event.keyboard.keycode == ALLEGRO_KEY_Q){
					return MENU;
				}
			}
			// A possibilidade de despausar ou pausar só é possível quando a tecla é solta
			else if(event.type == ALLEGRO_EVENT_KEY_UP){
				if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
					pausePressed = 0;
			}
		}
		if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) 
			break;											//Evento de clique no "X" de fechamento da tela. Encerra o programa graciosamente.
	}

	al_destroy_font(font);
	al_destroy_bitmap(background);
	playerDestroy(playerCharacter);

	return 0;
}


int death(ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *queue, long *cameraDisplacement){
	ALLEGRO_EVENT event;
	ALLEGRO_FONT *font;
	ALLEGRO_FONT *title;

	font = al_load_font("./Fonts/LowresPixel-Regular.otf", 36, 0);
	title = al_load_font("./Fonts/LowresPixel-Regular.otf", 60, 0);

	// Tela de GameOver
	al_draw_filled_rectangle(0, 0, X_SCREEN + *cameraDisplacement, Y_SCREEN, al_map_rgb(0, 0, 0));
	while(1){
		al_wait_for_event(queue, &event);

		if(event.type == ALLEGRO_EVENT_TIMER){
			al_draw_text(title, al_map_rgb(255, 0, 0), X_SCREEN / 2 + *cameraDisplacement, Y_SCREEN / 2 - 90, ALLEGRO_ALIGN_CENTRE, "VOCE MORREU");
			al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2 + *cameraDisplacement, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTRE, "Pressione 'ESC' para Voltar para o Menu");

			al_flip_display();
		}
		// Quando escape for pressionado volta para o menu
		else if(event.type == ALLEGRO_EVENT_KEY_DOWN){
			if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				return MENU;
		}
		// O display pode ser fechado clicando no 'x' da janela
		else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) 
			break;	
	}
}


int win(ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *queue, long *cameraDisplacement){
	ALLEGRO_EVENT event;
	ALLEGRO_FONT *font;
	ALLEGRO_FONT *title;

	font = al_load_font("./Fonts/LowresPixel-Regular.otf", 36, 0);
	title = al_load_font("./Fonts/LowresPixel-Regular.otf", 60, 0);

	// Tela de vitória
	al_draw_filled_rectangle(0, 0, X_SCREEN + *cameraDisplacement, Y_SCREEN, al_map_rgb(0, 0, 0));
	while(1){
		al_wait_for_event(queue, &event);

		if(event.type == ALLEGRO_EVENT_TIMER){
			al_draw_text(title, al_map_rgb(0, 255, 0), X_SCREEN / 2 + *cameraDisplacement, Y_SCREEN / 2 - 70, ALLEGRO_ALIGN_CENTRE, "VOCE VENCEU");
			al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2 + *cameraDisplacement, Y_SCREEN / 2 + 20, ALLEGRO_ALIGN_CENTRE, "Pressione 'ESC' para Voltar para o Menu");

			al_flip_display();
		}
		// Quando escape for pressionado volta para o menu
		else if(event.type == ALLEGRO_EVENT_KEY_DOWN){
			if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				return MENU;
		}
		// O display pode ser fechado clicando no 'x' da janela
		else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) 
			break;	
	}
}


int main(){	
	// Inicialização Allegro
	al_init();																		//Faz a preparação de requisitos da biblioteca Allegro
	al_install_keyboard();															//Habilita a entrada via teclado (eventos de teclado), no programa
	al_init_image_addon();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	al_install_keyboard();
	al_install_mouse();

	// Variáveis Allegro
	ALLEGRO_TIMER* timer;															//Cria o relógio do jogo; isso indica quantas atualizações serão realizadas por segundo (30, neste caso)
	ALLEGRO_EVENT_QUEUE* queue;														//Cria a fila de eventos; todos os eventos (programação orientada a eventos) 
	ALLEGRO_FONT* font;																//Carrega uma fonte padrão para escrever na tela (é bitmap, mas também suporta adicionar fontes ttf)
	ALLEGRO_DISPLAY* disp;															//Cria uma janela para o programa, define a largura (x) e a altura (y) da tela em píxeis (320x320, neste caso)
	ALLEGRO_EVENT event;															//Variável que guarda um evento capturado, sua estrutura é definida em: https://www.allegro.cc/manual/5/ALLEGRO_EVENT

	// Inicialização das variáveis
	timer = al_create_timer(1.0 / 30.0);
	queue = al_create_event_queue();
	disp = al_create_display(X_SCREEN, Y_SCREEN);

	al_set_window_position(disp, 400, 150);
	al_set_window_title(disp, "Knight's Exploration");

	al_register_event_source(queue, al_get_keyboard_event_source());				//Indica que eventos de teclado serão inseridos na nossa fila de eventos
	al_register_event_source(queue, al_get_display_event_source(disp));				//Indica que eventos de tela serão inseridos na nossa fila de eventos
	al_register_event_source(queue, al_get_timer_event_source(timer));				//Indica que eventos de relógio serão inseridos na nossa fila de eventos
	al_register_event_source(queue, al_get_mouse_event_source());
	al_flush_event_queue(queue);

	al_start_timer(timer);															//Função que inicializa o relógio do programa


	int gameState = 1;
	int gameDifficulty = 0;
	long cameraDisplacement = 0;
	while(gameState){																		//Laço principal do programa
		switch(gameState){
			case MENU:
				gameState = menu(timer, queue, &gameDifficulty, &cameraDisplacement);
				break;
			case GAME:
				gameState = game(timer, queue, gameDifficulty, &cameraDisplacement);
				break;
			case DEATH:
				gameState = death(timer, queue, &cameraDisplacement);
				break;
			case WIN:
				gameState = win(timer, queue, &cameraDisplacement);
				break;
		}
	}

	al_destroy_display(disp);														//Destrutor da tela
	al_destroy_timer(timer);														//Destrutor do relógio
	al_destroy_event_queue(queue);													//Destrutor da fila

	return 0;
}