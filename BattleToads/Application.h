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
class ModuleEnemies;
// Game modules ---
class ModulePlayer;
class ModuleSceneIntro;
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
	ModuleEnemies * enemies;
	// Game modules ---
	ModulePlayer* player;
	ModuleSceneIntro* scene_intro;
	ModuleStageOne * stageOne;

private:

	std::list<Module*> modules;
};

extern Application* App;

#endif // __APPLICATION_CPP__