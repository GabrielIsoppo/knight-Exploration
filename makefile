TARGET = KE
SOURCES = KnightExploration.c Player.c Enemy.c Platform.c Joystick.c Movement.c
ALLEGRO_FLAGS = $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_image-5 allegro_primitives-5 allegro_ttf-5 --libs --cflags)

all:
	gcc $(SOURCES) -o $(TARGET) $(ALLEGRO_FLAGS)

clean:
	rm -rf $(TARGET)