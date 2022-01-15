// [HACKATHON 3-9]
// TODO: Create scene_settings.hpp and scene_settings.c.
// No need to do anything for this part. We've already done it for
// you, so this 2 files is like the default scene template.
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "scene_end.hpp"
#include "scene_menu_object.h"
#include "scene_settings.hpp"
#include "scene_game.hpp"
#include "scene_menu.hpp"
#include "shared.h"
ALLEGRO_SAMPLE_ID menuBGM;
// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */

// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.
extern ALLEGRO_FONT* font;
ALLEGRO_FONT* small_font;

void SceneSetting::draw() {
	al_clear_to_color(al_map_rgb(35, 187, 214));
	al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_W / 2, 30,
             ALLEGRO_ALIGN_CENTER,"Settings");
    al_draw_text(small_font, al_map_rgb(0, 0, 0), SCREEN_W / 2, 300,
             ALLEGRO_ALIGN_CENTER,"PRESS \"UP\"and\"DOWN\"to adjust volume");
    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_W / 2, 600,
             ALLEGRO_ALIGN_CENTER,"PRESS \"SPACE\"");
}

int max(int a,int b){
	return (a>b)?a:b; 
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
			music_volume=max(music_volume,0);
			effect_volume=max(effect_volume,0);
			break;
		default:
			break;
	}
}
// The only function that is shared across files.
SceneSetting::SceneSetting() {
	font=al_load_ttf_font("Assets/pirulen.ttf", 48, 0);
	small_font=al_load_ttf_font("Assets/pirulen.ttf", 24, 0);
	name = strdup("Settings");
	game_log("Settings scene created");
	// TODO: Register more event callback functions such as keyboard, mouse, ...
}
