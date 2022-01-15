#ifndef SCENE_SETTINGS_H
#define SCENE_SETTINGS_H
#include "game.hpp"

class SceneSetting : public Scene
{
public:
    SceneSetting();
	virtual void draw();
	virtual void on_key_down(int);
};

#endif
