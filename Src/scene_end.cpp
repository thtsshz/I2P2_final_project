#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro.h>
#include "scene_end.h"
#include "scene_menu_object.h"
#include "scene_settings.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "shared.h"
ALLEGRO_FONT* font;
extern int score[50];
extern int score_size;
extern ALLEGRO_SAMPLE* endMusic;
ALLEGRO_SAMPLE_ID endBGM;
ALLEGRO_BITMAP* ranklist;
static void init(){
	endBGM=play_bgm(endMusic,music_volume);
	ranklist=load_bitmap("Assets/ranklist.png");
}
static void draw(){
	al_clear_to_color(al_map_rgb(102, 238, 108));
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
    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_W / 2, 680,
             ALLEGRO_ALIGN_CENTER,"PRESS \"ENTER\"");
}
static void on_key_down(int keycode) {
	switch (keycode) {
		case ALLEGRO_KEY_ENTER:
			game_change_scene(scene_menu_create());
			break;
		default:
			break;
	}
}
static void destroy(void) {
	stop_bgm(endBGM);
	al_destroy_font(font);
}
Scene scene_end_create(){
	font=al_load_ttf_font("Assets/Minecraft.ttf", 48, 0);
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.initialize=&init;
	scene.name = strdup("End");
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down=&on_key_down;
	game_log("End scene created");
	return scene;
}
