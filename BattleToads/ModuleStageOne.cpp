#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleStageOne.h"



ModuleStageOne::ModuleStageOne(bool active):Module(active)
{
}


ModuleStageOne::~ModuleStageOne()
{
}

bool ModuleStageOne::Start() {

	LOG("Loading space scene");

	levelOne = App->textures->Load("rtype/BattletoadSprites/level1.png");
	background = new Animation();
	waterfall = Animation();
	App->player->Enable();
	App->particles->Enable();
	App->collision->Enable();
	//App->audio->PlayMusic("rtype/stage1.ogg", 1.0f);
	App->renderer->camera.x = App->renderer->camera.y = 0;

	background->frames.push_back({ 518, 7, 1404, 357 });
	background->speed = 0.08f;

	waterfall.frames.push_back({1703,380,32,112});
	waterfall.frames.push_back({1736,380,32,112});
	waterfall.frames.push_back({1769,380,32,112});
	waterfall.frames.push_back({1802,380,32,112});
	waterfall.frames.push_back({1835,380,32,112});
	waterfall.speed = 0.1f;

	return true;
}

update_status ModuleStageOne::Update()
{
	App->renderer->Blit(levelOne, 0, 0, &(background->GetCurrentFrame()),1.0f);
	App->renderer->Blit(levelOne, 986,115, &(waterfall.GetCurrentFrame()), 1.0f);
	App->renderer->Blit(levelOne, 986,227, &(waterfall.GetCurrentFrame()), 1.0f);

	return UPDATE_CONTINUE;
}

bool ModuleStageOne::CleanUp()
{
	LOG("Unloading battletoad stage one scene");

	
	delete background;

	return true;
}