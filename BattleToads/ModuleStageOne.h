
#ifndef  _MODULESCENESTAGEONE_H_
#define  _MODULESCENESTAGEONE_H_

#include "Module.h"

struct SDL_Texture;

class ModuleStageOne :public Module
{
public:
	ModuleStageOne(bool active = true);
	~ModuleStageOne();
	bool Start();
	update_status Update();
	bool CleanUp();
public :
	SDL_Texture* levelOne = nullptr;
	Animation  background; 
	Animation  waterfall;
private:
	Collider* floorOne;
};
#endif // _MODULESCENESTAGEONE_H_
