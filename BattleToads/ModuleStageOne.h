
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
	Collider* CreateCollider(int x, int y, int w, int h, ColliderType colliderType);
	
public :
	SDL_Texture* levelOne = nullptr;
	Animation  background; 
	Animation  waterfall;
private:
	list<Collider *> colliders;
	
};
#endif // _MODULESCENESTAGEONE_H_
