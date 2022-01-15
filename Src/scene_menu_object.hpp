#ifndef MENU_OBJECT_H
#define MENU_OBJECT_H

#include "utility.h"
#include "shared.h"
#include "game.hpp"

class Button
{
public:
	RecArea body;
	ALLEGRO_BITMAP *default_img;
	ALLEGRO_BITMAP *hovered_img;
	bool hovered;
	Button() {}
	Button(float, float, float, float, const char*, const char*);
	void draw();
	bool Hover(int, int);
};

#endif
