#define _CRT_SECURE_NO_WARNINGS

#include "scene_menu_object.hpp"
#include "utility.h"
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Button::Button(float x, float y, float w, float h, const char* default_image_path, const char* hovered_image_path)
{
	default_img = load_bitmap(default_image_path);
	if (hovered_image_path) {
		hovered_img = load_bitmap(hovered_image_path);
	}

	if (!default_img) {
		game_log("failed loading button image!");
	}

	body.x = x;
	body.y = y;
	body.w = w;
	body.h = h;
}

void Button::draw() {
	ALLEGRO_BITMAP* _img = hovered_img ? hovered ? hovered_img : default_img : default_img;
	al_draw_scaled_bitmap(
		_img,
		0, 0,
		al_get_bitmap_width(_img), al_get_bitmap_height(_img),
		body.x, body.y,
		body.w, body.h, 0
	);
}

bool Button::Hover(int mouse_x, int mouse_y) {
	//	[HACKATHON 3-6]
	//	TODO: Check if mouse is hovering on the button
	//	Uncomment and fill the code below
	return pnt_in_rect(mouse_x, mouse_y, body);
	//return false;
}
