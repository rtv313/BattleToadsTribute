#ifndef __MODULE_LEVEL_INTRO_H__
#define __MODULE_LEVEL_INTRO_H__

#include "Module.h"
#include "Animation.h";


struct SDL_Texture;
class ModuleLevelIntro :public Module
{
public:
	ModuleLevelIntro(bool active);
	~ModuleLevelIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
public:
	SDL_Texture* background = nullptr;
	Animation  backSelection;
};

#endif
