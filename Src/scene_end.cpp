#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro.h>
#include "scene_end.hpp"
#include "scene_menu_object.hpp"
#include "scene_settings.hpp"
#include "scene_game.hpp"
#include "scene_menu.hpp"
#include "shared.h"
ALLEGRO_FONT* font;
extern int score[50];
extern int score_size;
extern ALLEGRO_SAMPLE* endMusic;
ALLEGRO_SAMPLE_ID endBGM;
ALLEGRO_BITMAP* ranklist;
ALLEGRO_BITMAP* bg;
void SceneEnd::draw() {
	al_clear_to_color(al_map_rgb(31, 21, 85));
	al_draw_scaled_bitmap(
		bg,
		0, 0,
		470, 579,
		0, 0,
		800, 840,
		0
	);
	al_draw_scaled_bitmap(
		ranklist,
		0, 0,
		288, 109,
		227, 20,
		346, 131,
		0
	);//800/2-346/2
    char str[20];
    for(int i=0;i<10;i++){
    	sprintf(str,"%d. :%10d",i+1,score[i]);
    	al_draw_text(font, al_map_rgb(255, 255, 255), 30, 130+i*55,
            ALLEGRO_ALIGN_LEFT,str);
	}
    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, 680,
             ALLEGRO_ALIGN_CENTER,"PRESS \"ENTER\"");
}
void SceneEnd::on_key_down(int keycode) {
	switch (keycode) {
		case ALLEGRO_KEY_ENTER:
			game_change_scene(new SceneMenu());
			break;
		default:
			break;
	}
}
SceneEnd::~SceneEnd(void) {
	stop_bgm(endBGM);
	al_destroy_font(font);
}

SceneEnd::SceneEnd() : Scene() {
	endBGM = play_bgm(endMusic,music_volume);
	ranklist = load_bitmap("Assets/ranklist.png");
	bg = load_bitmap("Assets/bg.png");
	font=al_load_ttf_font("Assets/Minecraft.ttf", 48, 0);
	name = strdup("End");
	game_log("End scene created");
}
