 //#pragma once
// [game.hpp]
// declare shared variables and allegro5 related routines.

// Extern variables are "declare without defining". When
// a variable is defined, the compiler allocates memory for that
// variable and possibly also initializes its contents to some value.
// The declaration informs the compiler that a variable by that name
// and type exists, but the compiler does not need to allocate memory
// for it since it is allocated elsewhere.

// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

#ifndef GAME_H
#define GAME_H
#define _CRT_SECURE_NO_DEPRECATE
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>	
typedef struct Ghost Ghost;
typedef struct Map Map;
typedef struct Pacman Pacman;
#define LOG_ENABLED
static ALLEGRO_EVENT_QUEUE* game_event_queue;
typedef void(*func_ptr)(void);
typedef void(Ghost::*script_func_ptr)(Map*M, Pacman* pacman);
typedef void(*func_ptr_keyboard)(int keycode);
typedef void(*func_ptr_mouse)(int btn, int x, int y, int dz);
class Scene
{
public:
	char *name;
	Scene() {};
	virtual ~Scene() = default;
	virtual void update() {}
	virtual void draw() {}
	virtual void on_key_down(int) {}
	virtual void on_key_up(int) {}
	virtual void on_mouse_down(int, int, int, int) {}
	virtual void on_mouse_move(int, int, int, int) {}
	virtual void on_mouse_up(int, int, int, int) {}
	virtual void on_mouse_scroll(int, int, int, int) {}
};

extern const int FPS;
extern const int SCREEN_W;
extern const int SCREEN_H;
extern const int RESERVE_SAMPLES;

const uint32_t GAME_TICK_CD=64;
extern Scene *active_scene;
extern bool key_state[ALLEGRO_KEY_MAX];
extern bool* mouse_state;
extern int mouse_x, mouse_y;

void game_create(void);
void game_change_scene(Scene *next_scene);
void game_abort(const char* format, ...);
void game_log(const char* format, ...);
extern void shared_init(void);
extern void shared_destroy(void);
#endif
