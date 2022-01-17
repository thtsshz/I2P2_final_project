#ifndef PACMAN_OBJ_H
#define PACMAN_OBJ_H

#include "utility.h"
#include "game.hpp"

typedef struct Map Map;

class Pacman
{
private:
	bitmapdata imgdata;
	func_ptr _move;
	int color;
	bool powerUp;
	ALLEGRO_BITMAP* move_sprite;
	ALLEGRO_BITMAP* die_sprite;
	bool movable(Map *, Directions);

public:
	object objData;
	int speed;
	ALLEGRO_TIMER* death_anim_counter;
	Pacman();
	~Pacman();
	void draw();
	void move(Map* M);
	void eatItem(const char Item);
	void NextMove(Directions next);
	void die();
};

#endif 
