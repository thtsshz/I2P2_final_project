#ifndef SCENE_UTILITY_H
#define SCENE_UTILITY_H
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
ALLEGRO_SAMPLE* load_audio(const char* filename);
ALLEGRO_SAMPLE_ID play_audio(ALLEGRO_SAMPLE* sample, float volume);
ALLEGRO_SAMPLE_ID play_bgm(ALLEGRO_SAMPLE* sample, float volume);
void stop_bgm(ALLEGRO_SAMPLE_ID sample);
ALLEGRO_FONT* load_font(const char* filename, int size);
ALLEGRO_BITMAP* load_bitmap(const char* filename);
ALLEGRO_BITMAP* load_bitmap_resized(const char* filename, int w, int h);
typedef struct RecArea{
	float x, y, w, h;
} RecArea;
typedef struct Pair_IntInt {
	int x;
	int y;
} Pair_IntInt;
typedef enum class Directions{
	NONE = 0,
	UP = 1,
	LEFT = 2,
	RIGHT = 3,
	DOWN = 4,
	UP_DOWN = 5,
	LEFT_RIGHT = 6,
	UP_LEFT = 7,
	DOWN_LEFT = 8,
	DOWN_RIGHT = 9,
	UP_RIGHT = 10
} Directions;
typedef struct object {
	Pair_IntInt Coord; 
	Pair_IntInt Size; // x for width, y for height
	Directions facing;
	Directions preMove;
	Directions nextTryMove;
	uint32_t moveCD;				
} object;

typedef struct bitmapdata{
	int bitmap_x;
	int bitmap_y;
	int bitmap_w;
	int bitmap_h;
} bitmapdata;

void setRecArea(RecArea* RA, float x, float y, float w, float h);
RecArea getDrawArea(object obj, uint32_t TICK);
bool RecAreaOverlap(const RecArea RA,const RecArea RB);
void printRecAreaInfo(const RecArea* RA);
void printDirection(const Directions a);
bool movetime(int speed);
uint32_t generateRandomNumber(uint32_t a, uint32_t b);
double generateRandomFloat(void);
bool bernoulliTrail(double p); 
bool pnt_in_rect(int px, int py, RecArea field);
#endif
