#ifndef __APPLICATION_CPP__
#define __APPLICATION_CPP__

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModuleFadeToBlack;
class ModuleCollision;
class ModuleSpawnTriggers;
class ModuleSpawnZones;
class ModuleParticles;

// Game modules ---
class ModulePlayer;
class ModuleSceneIntro;
class ModuleSceneSpace;
class ModuleStageOne;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleFadeToBlack* fade;
	ModuleCollision* collision;
	ModuleSpawnTriggers* spawnTriggers;
	ModuleSpawnZones* spawnZones;
	ModuleParticles* particles;

	// Game modules ---
	ModulePlayer* player;
	ModuleSceneIntro* scene_intro;
	ModuleSceneSpace* scene_space;
	ModuleStageOne * stageOne;

private:

	std::list<Module*> modules;
};

extern Application* App;

#endif // __APPLICATION_CPP__