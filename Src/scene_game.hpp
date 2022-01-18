#ifndef SCENE_GAME_H
#define SCENE_GAME_H
#include "game.hpp"

class SceneMain : public Scene
{
public:
    SceneMain();
    ~SceneMain();
    virtual void update();
	virtual void draw();
	virtual void on_key_down(int);
	virtual void on_mouse_down(int, int, int, int);
    void step();
    void checkItem();
    void status_update();
    void draw_hitboxes();
    void render_init_screen();
};

#endif
