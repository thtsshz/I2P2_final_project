#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <math.h>
#include "scene_menu_object.hpp"
#include "scene_settings.hpp"
#include "scene_game.hpp"
#include "scene_menu.hpp"
#include "utility.h"
#include "shared.h"
static ALLEGRO_BITMAP* gameTitle = NULL;
static ALLEGRO_SAMPLE_ID menuBGM;
static int gameTitleW ;
static int gameTitleH ;
extern bool check_color;
static Button btnSettings;

void SceneMenu::draw() {

	al_clear_to_color(al_map_rgb(0, 0, 0));

	const float scale = 0.7;
	const float offset_w = (SCREEN_W >> 1) - 0.5 * scale * gameTitleW;
	const float offset_h = (SCREEN_H >> 1) - 0.5 * scale * gameTitleH;
	al_draw_scaled_bitmap(
		gameTitle,
		0, 0,
		gameTitleW, gameTitleH,
		offset_w, offset_h-120,
		gameTitleW * scale, gameTitleH * scale,
		0
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W/2,
		offset_h+80,
		ALLEGRO_ALIGN_CENTER,
		"PRESS \" B \"to change the color"
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W/2,
		SCREEN_H - 150,
		ALLEGRO_ALIGN_CENTER,
		"PRESS \"ENTER\""
	);
	btnSettings.draw();
}

void SceneMenu::on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
	btnSettings.hovered = pnt_in_rect(mouse_x, mouse_y,btnSettings.body);
}

void SceneMenu::on_mouse_down(int a, int b, int c, int d) {
	if (btnSettings.hovered)
		game_change_scene(new SceneSetting());
}


SceneMenu::~SceneMenu() {
	stop_bgm(menuBGM);
	al_destroy_bitmap(gameTitle);
	al_destroy_bitmap(btnSettings.default_img);
	al_destroy_bitmap(btnSettings.hovered_img);
}

void SceneMenu::on_key_down(int keycode) {

	switch (keycode) {
		case ALLEGRO_KEY_ENTER:
			game_change_scene(new SceneMain());
			break;
		case ALLEGRO_KEY_B:
			check_color^=1;
			break;
		default:
			break;
	}
}
SceneMenu::SceneMenu(void) : Scene() {
	btnSettings = Button(730, 20, 50, 50, "Assets/settings.png", "Assets/settings2.png");
	gameTitle = load_bitmap("Assets/title.png");
	gameTitleW = al_get_bitmap_width(gameTitle);
	gameTitleH = al_get_bitmap_height(gameTitle);
	stop_bgm(menuBGM);
	menuBGM = play_bgm(themeMusic, music_volume);
	name = strdup("Menu");
	game_log("Menu scene created");
}
