// [shared.c]
// you should define the shared variable declared in the header here.

#include "shared.h"
#include "utility.h"
#include "game.hpp"
// #include "scene_menu.hpp"

ALLEGRO_FONT* font_pirulen_32;
ALLEGRO_FONT* font_pirulen_24;
ALLEGRO_SAMPLE* themeMusic = NULL;
ALLEGRO_SAMPLE* endMusic = NULL;
ALLEGRO_SAMPLE* PACMAN_MOVESOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_DEATH_SOUND = NULL;
ALLEGRO_TIMER* power_up_timer= NULL;
ALLEGRO_FONT* menuFont = NULL;
ALLEGRO_TIMER* game_tick_timer = NULL;
Ghost** ghosts=NULL;
int score[50]={0};
int score_size=0;
bool check_color=0;
bool back=0;
bool tran=0;
int fontSize = 30;
float music_volume = 0.5;
float effect_volume = 0.5;
bool gameDone = false;
bool multiPlayer = false; // whether in multi-player mode

/*
	A way to accelerate is load assets once.
	And delete them at the end.
	This method does provide you better management of memory.
	
*/
void shared_init(void) {
	
	menuFont = load_font("Assets/Minecraft.ttf", fontSize);
	themeMusic = load_audio("Assets/Music/original_theme.ogg");
	endMusic = load_audio("Assets/Music/pacman_intermission.ogg");
	PACMAN_MOVESOUND = load_audio("Assets/Music/pacman-chomp.ogg");
	PACMAN_DEATH_SOUND = load_audio("Assets/Music/pacman_death.ogg");
}

void shared_destroy(void) {

	al_destroy_font(menuFont);
	al_destroy_sample(themeMusic);
	al_destroy_sample(endMusic);
	al_destroy_sample(PACMAN_MOVESOUND);
	al_destroy_sample(PACMAN_DEATH_SOUND);
}
