#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro.h>
#include <string.h>
#include <stdio.h>
#include "game.hpp"
#include "shared.h"
#include "utility.h"
#include "scene_game.hpp"
#include "scene_menu.hpp"
#include "scene_settings.hpp"
#include "pacman_obj.hpp"
#include "scene_end.hpp"
#include "ghost.hpp"
#include "map.hpp"
// [HACKATHON 2-0]
// Just modify the GHOST_NUM to 1
#define GHOST_NUM 4
/* global variables*/
//extern const uint32_t GAME_TICK_CD;
extern uint32_t GAME_TICK;
extern int score[50];
extern ALLEGRO_TIMER* power_up_timer;
extern ALLEGRO_TIMER* game_tick_timer;
int game_main_Score = 0;
bool game_over = false;

/* Internal variables*/

static const int power_up_duration = 10;
static Pacman* pman;
static Map* basic_map;
static Ghost** ghosts;
extern ALLEGRO_FONT* font;
bool debug_mode = false;
bool cheat_mode = false;
bool check_P=0;
/* Declare static function prototypes */

void SceneMain::step(void) {
	if (pman->objData.moveCD > 0)
		pman->objData.moveCD -= pman->speed;
	for (int i = 0; i < GHOST_NUM; i++) {
		// important for movement
		if (ghosts[i]->objData.moveCD > 0)
			ghosts[i]->objData.moveCD -= ghosts[i]->speed;
	}
}
void SceneMain::checkItem(void) {
	check_P=0;
	int Grid_x = pman->objData.Coord.x, Grid_y = pman->objData.Coord.y;
	if (Grid_y >= basic_map->row_num - 1 || Grid_y <= 0 || Grid_x >= basic_map->col_num - 1 || Grid_x <= 0)
		return;
	// [HACKATHON 1-3]
	// TODO: check which item you are going to eat and use `pacman_eatItem` to deal with it.
	switch (basic_map->map[Grid_y][Grid_x])
	{
	case '.':
		pman->eatItem('.');
		basic_map->beansNum--;
		game_main_Score+=100;
		break;
	case 'P':
		pman->eatItem('P');
		basic_map->beansNum--;
		game_main_Score+=555;
		check_P=1;
		break;
	case 'F':
		pman->eatItem('F');
		game_main_Score+=326;
		break;
	case 'S':
		pman->eatItem('S');
		game_main_Score+=326;
		break;
	default:
		break;
	}
	// [HACKATHON 1-4]
	// erase the item you eat from map
	// becareful no erasing the wall block.
	if(basic_map->map[Grid_y][Grid_x]!='#'&&basic_map->map[Grid_y][Grid_x]!='B')
		basic_map->map[Grid_y][Grid_x]=' ';
}

void SceneMain::status_update(void) {
	if(back){
		back=0;
		for (int i = 0; i < GHOST_NUM; i++) {
			if(ghosts[i]->status==GhostStatus::FREEDOM){
				ghosts[i]->status=GhostStatus::GO_IN;
				ghosts[i]->speed=4;
			}
		}
		return;
	}
	if(check_P){
		al_stop_timer(power_up_timer);
		al_set_timer_count(power_up_timer,0);
		al_start_timer(power_up_timer);
		for(int i=0;i<GHOST_NUM;i++)
			ghosts[i]->toggle_FLEE(1);
		return;
	}
	for (int i = 0; i < GHOST_NUM; i++) {
		if (ghosts[i]->status == GhostStatus::GO_IN)
			continue;
		// [TODO]
		// use `getDrawArea(..., GAME_TICK_CD)` and `RecAreaOverlap(..., GAME_TICK_CD)` functions to detect
		// if pacman and ghosts collide with each other.
		// And perform corresponding operations.
		// [NOTE]
		// You should have some branch here if you want to implement power bean mode.
		// Uncomment Following Code
		if(!basic_map->beansNum){
			game_log("Win");
			game_over = true;
			break;
		}
		if(!cheat_mode&&RecAreaOverlap(getDrawArea(pman->objData,GAME_TICK_CD),getDrawArea(ghosts[i]->objData, GAME_TICK_CD))){
			if(ghosts[i]->status==GhostStatus::FLEE){
				game_main_Score+=1111;
				ghosts[i]->status=GhostStatus::GO_IN;
				ghosts[i]->speed=4;
				ghosts[i]->previous_timer_val=al_get_timer_count(game_tick_timer);
				continue;
			}
			if(ghosts[i]->status==GhostStatus::FREEDOM){
				pman->die();
				game_over = true;
				break;
			} 
		}
//		printf("%d\n",al_get_timer_count(power_up_timer));
		if(al_get_timer_count(power_up_timer)>9){
			al_stop_timer(power_up_timer);
			al_set_timer_count(power_up_timer,0);
			for(i=0;i<GHOST_NUM;i++){
				if(ghosts[i]->status==GhostStatus::FLEE){
					ghosts[i]->speed=2;
					ghosts[i]->status=GhostStatus::FREEDOM;
				}
			}
			break;
		}
//		if(check_P){
//			al_stop_timer(power_up_timer);
//			al_set_timer_count(power_up_timer,0);
//			al_start_timer(power_up_timer);
//			ghost_toggle_FLEE(ghosts[i],1);
//		}
	}
}

void SceneMain::update(void) {

	if (game_over) {
		//	[TODO] start pman->death_anim_counter and schedule a game-over event (e.g change scene to menu) after Pacman's death animation finished
		al_start_timer(pman->death_anim_counter);
		if(al_get_timer_count(pman->death_anim_counter)>192)
			game_change_scene(new SceneEnd());
		return;
	}
	step();
	checkItem();
	status_update();
	pman->move(basic_map);
	if (multiPlayer && (ghosts[0]->status == GhostStatus::FREEDOM || ghosts[0]->status == GhostStatus::FLEE))
		ghosts[0]->move(basic_map);
	for (int i = multiPlayer && (ghosts[0]->status == GhostStatus::FREEDOM || ghosts[0]->status == GhostStatus::FLEE); i < GHOST_NUM; i++) 
		ghosts[i]->move_script(basic_map, pman);
}

void SceneMain::draw(void) {

	al_clear_to_color(al_map_rgb(0, 0, 0));

	
	//	[TODO]
	//	Draw scoreboard, something your may need is sprinf();
	char str[20];
	sprintf(str,"score:%d",game_main_Score);
	al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, 21,
            ALLEGRO_ALIGN_CENTER,str);
	basic_map->draw();

	pman->draw();
	if (game_over)
		return;
	// no drawing below when game over
	for (int i = 0; i < GHOST_NUM; i++)
		if (ghosts[i])
			ghosts[i]->draw();
	
	//debugging mode
	if (debug_mode) {
		draw_hitboxes();
	}

}

void SceneMain::draw_hitboxes(void) {
	RecArea pmanHB = getDrawArea(pman->objData, GAME_TICK_CD);
	al_draw_rectangle(
		pmanHB.x, pmanHB.y,
		pmanHB.x + pmanHB.w, pmanHB.y + pmanHB.h,
		al_map_rgb_f(1.0, 0.0, 0.0), 2
	);

	for (int i = 0; i < GHOST_NUM; i++) {
		RecArea ghostHB = getDrawArea(ghosts[i]->objData, GAME_TICK_CD);
		al_draw_rectangle(
			ghostHB.x, ghostHB.y,
			ghostHB.x + ghostHB.w, ghostHB.y + ghostHB.h,
			al_map_rgb_f(1.0, 0.0, 0.0), 2
		);
	}

}

static void printinfo(void) {
	game_log("pacman:\n");
	game_log("coord: %d, %d\n", pman->objData.Coord.x, pman->objData.Coord.y);
	game_log("PreMove: %d\n", pman->objData.preMove);
	game_log("NextTryMove: %d\n", pman->objData.nextTryMove);
	game_log("Speed: %f\n", pman->speed);
}

int cmp(const void *a,const void *b){
	if(*((int*)a)<*((int*)b))
		return 1;
	return -1;
}

SceneMain::~SceneMain(void) {
	/*
		[TODO]
		free map array, Pacman and ghosts
	*/
	score[score_size++]=game_main_Score;
	qsort(score,score_size,sizeof(int),cmp);
	al_destroy_timer(power_up_timer);
	free(basic_map);
	free(pman);
	for(int i=0;i<GHOST_NUM;i++)
		delete ghosts[i];
	free(ghosts);
}

void SceneMain::on_key_down(int key_code) {
	switch (key_code)
	{
		// [HACKATHON 1-1]	
		// TODO: Use allegro pre-defined enum ALLEGRO_KEY_<KEYNAME> to controll pacman movement
		// we provided you a function `pman->NextMove` to set the pacman's next move direction.
		case ALLEGRO_KEY_W:{
			pman->NextMove(Directions::UP);
			break;
		}
		case ALLEGRO_KEY_A:
			pman->NextMove(Directions::LEFT);
			break;
		case ALLEGRO_KEY_S:
			pman->NextMove(Directions::DOWN);
			break;
		case ALLEGRO_KEY_D:
			pman->NextMove(Directions::RIGHT);
			break;
		case ALLEGRO_KEY_UP:
			if (multiPlayer && (ghosts[0]->status == GhostStatus::FREEDOM || ghosts[0]->status == GhostStatus::FLEE))
				ghosts[0]->NextMove(Directions::UP);
			break;
		case ALLEGRO_KEY_LEFT:
			if (multiPlayer && (ghosts[0]->status == GhostStatus::FREEDOM || ghosts[0]->status == GhostStatus::FLEE))
				ghosts[0]->NextMove(Directions::LEFT);
			break;
		case ALLEGRO_KEY_DOWN:
			if (multiPlayer && (ghosts[0]->status == GhostStatus::FREEDOM || ghosts[0]->status == GhostStatus::FLEE))
				ghosts[0]->NextMove(Directions::DOWN);
			break;
		case ALLEGRO_KEY_RIGHT:
			if (multiPlayer && (ghosts[0]->status == GhostStatus::FREEDOM || ghosts[0]->status == GhostStatus::FLEE))
				ghosts[0]->NextMove(Directions::RIGHT);
			break;
		case ALLEGRO_KEY_C:
			cheat_mode = !cheat_mode;
			if (cheat_mode)
				printf("cheat mode on\n");
			else 
				printf("cheat mode off\n");
			break;
		case ALLEGRO_KEY_G:
			debug_mode = !debug_mode;
			break;
	default:
		break;
	}

}

void SceneMain::on_mouse_down(int, int, int, int) {
	// nothing here

}

void SceneMain::render_init_screen(void) {
	al_clear_to_color(al_map_rgb(0, 0, 0));

	basic_map->draw();
	pman->draw();
	for (int i = 0; i < GHOST_NUM; i++)
		if (ghosts[i])
			ghosts[i]->draw();

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 0),
		400, 400,
		ALLEGRO_ALIGN_CENTER,
		"READY!"
	);

	al_flip_display();
	al_rest(2.0);

}
// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
SceneMain::SceneMain(void) : Scene() {

	game_over = false;
	game_main_Score = 0;
	// create map
	basic_map = new Map(nullptr);
	font = al_load_ttf_font("Assets/pirulen.ttf", 24, 0);
	// [TODO]
	// Create map from .txt file and design your own map !!
	//basic_map = create_map("Assets/map_nthu.txt");
	if (!basic_map) {
		game_abort("error on creating map");
	}	
	// create pacman
	pman = new Pacman();
	if (!pman) {
		game_abort("error on creating pacamn\n");
	}
	
	// allocate ghost memory
	// [HACKATHON 2-1]
	// TODO: Allocate dynamic memory for ghosts array.
	ghosts = (Ghost**)malloc(sizeof(Ghost*) * GHOST_NUM);
//	else {
		// [HACKATHON 2-2]
		// TODO: create a ghost.
		// Try to look the definition of ghost_create and figure out what should be placed here.
		for (int i = 0; i < GHOST_NUM; i++) {
			game_log("creating ghost %d\n", i);
			if(!i) 
				ghosts[i] = new GhostRed(); 
			else if(i==1)
				ghosts[i] = new GhostPink();
			else if(i==2)
				ghosts[i] = new GhostBlue();
			else
				ghosts[i] = new GhostOrange();
			if (!ghosts[i])
				game_abort("error creating ghost\n");
		}
//	}
	GAME_TICK = 0;

	render_init_screen();
	power_up_timer = al_create_timer(1.0f); // 1 tick / sec
	if (!power_up_timer)
		game_abort("Error on create timer\n");

	name = strdup("Start");
	//scene.on_mouse_down = &on_mouse_down;
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Start scene created");
}
