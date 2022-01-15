#ifndef PACMAN_OBJ_H
#define PACMAN_OBJ_H

#include "utility.h"
#include "game.hpp"

// <Forward Declaration>
// forward declarations can be useful when you need to have looping struct declarations,
// but you want to (should) prevent mutually include dependency.
// Only applicable when you only need the declaration but no need the definition in you sturct,
// which means that you only used the struct as pointer(no access to struct data), param, return type in this file.
// If you have to access the data in struct ex. `A->data`, 
// you should include the corresponding header file for its definition.
// Advance reading : https://stackoverflow.com/questions/18658438/what-is-forward-declaration-and-the-difference-between-typedef-struct-x-and

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

#endif // !PACMAN_OBJ_H
