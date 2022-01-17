
#ifndef SCENE_SHARED_H
#define SCENE_SHARED_H
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include "ghost.hpp"
extern ALLEGRO_FONT* font_pirulen_32;
extern ALLEGRO_FONT* font_pirulen_24;
extern ALLEGRO_SAMPLE* themeMusic;
extern ALLEGRO_SAMPLE* PACMAN_MOVESOUND;
extern ALLEGRO_SAMPLE* endMusic;
extern ALLEGRO_SAMPLE* PACMAN_DEATH_SOUND;
extern ALLEGRO_FONT* menuFont;
extern ALLEGRO_TIMER* power_up_timer;
extern ALLEGRO_TIMER* game_tick_timer;
extern int score[50];
extern int score_size;
extern int fontSize;
extern float music_volume;
extern float effect_volume;
extern bool gameDone;
extern bool check_color;
extern bool back;
extern bool tran;
extern bool multiPlayer;
extern Ghost** ghosts;
void shared_init(void);
void shared_destroy(void);

#endif
