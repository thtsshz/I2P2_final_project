#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "scene_end.hpp"
#include "scene_menu_object.hpp"
#include "scene_settings.hpp"
#include "scene_game.hpp"
#include "scene_menu.hpp"
#include "shared.h"
#include <string>
using namespace std;
ALLEGRO_SAMPLE_ID menuBGM;
extern ALLEGRO_FONT* font;
ALLEGRO_FONT* small_font;

void SceneSetting::draw() {
	al_clear_to_color(al_map_rgb(35, 187, 214));
	al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_W / 2, 30,
             ALLEGRO_ALIGN_CENTER,"Settings");
    al_draw_text(small_font, al_map_rgb(0, 0, 0), SCREEN_W / 2, 300,
             ALLEGRO_ALIGN_CENTER, ("PRESS \"UP\"and\"DOWN\" to adjust volume (Now: " + to_string(int(music_volume * 10)) + ')').c_str());
    al_draw_text(small_font, al_map_rgb(0, 0, 0), SCREEN_W / 2, 450,
             ALLEGRO_ALIGN_CENTER, ("PRESS \"TAB\" to toggle multiplayer mode (Now: " + string(multiPlayer ? "On" : "Off") + ')').c_str());
	al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_W / 2, 600,
             ALLEGRO_ALIGN_CENTER,"PRESS \"SPACE\"");
}

void SceneSetting::on_key_down(int keycode) {
	switch (keycode) {
		case ALLEGRO_KEY_SPACE:
			game_change_scene(new SceneMenu());
			break;
		case ALLEGRO_KEY_UP:
			music_volume+=0.1;
			effect_volume+=0.1;
			break; 
		case ALLEGRO_KEY_DOWN:
			music_volume-=0.1;
			effect_volume-=0.1;
			music_volume = max(music_volume, .0f);
			effect_volume = max(effect_volume, .0f);
			break;
		case ALLEGRO_KEY_TAB:
			multiPlayer ^= 1; 
			break;
	}
}
SceneSetting::SceneSetting() {
	font=al_load_ttf_font("Assets/pirulen.ttf", 48, 0);
	small_font = al_load_ttf_font("Assets/pirulen.ttf", 18, 0);
	name = strdup("Settings");
	game_log("Settings scene created");
}
