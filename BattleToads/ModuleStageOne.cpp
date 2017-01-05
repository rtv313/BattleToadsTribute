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
	background = Animation();
	waterfall =  Animation();
	App->player->Enable();
	App->particles->Enable();
	App->collision->Enable();
	//App->audio->PlayMusic("rtype/stage1.ogg", 1.0f);
	App->renderer->camera.x = App->renderer->camera.y = 0;

	background.frames.push_back({ 518, 7, 1404, 357 });
	background.speed = 0.08f;

	waterfall.frames.push_back({1703,380,32,112});
	waterfall.frames.push_back({1736,380,32,112});
	waterfall.frames.push_back({1769,380,32,112});
	waterfall.frames.push_back({1802,380,32,112});
	waterfall.frames.push_back({1835,380,32,112});
	waterfall.speed = 0.1f;


	colliders.push_back(CreateCollider(-20, 80, 240, 35));
	colliders.push_back(CreateCollider(0,310,47,632));
	colliders.push_back(CreateCollider(0, 150, 32, 92));
	colliders.push_back(CreateCollider(90, 160, 33, 29));
	colliders.push_back(CreateCollider(119, 180, 33, 29));
	colliders.push_back(CreateCollider(150, 160, 33, 29));
	colliders.push_back(CreateCollider(185, 150, 32, 72));
	colliders.push_back(CreateCollider(255, 160, 33, 29));
	colliders.push_back(CreateCollider(281, 180, 33, 29));
	colliders.push_back(CreateCollider(310, 160, 33, 29));
	colliders.push_back(CreateCollider(350, 150, 32, 100));
	colliders.push_back(CreateCollider(450, 160, 33, 29));
	colliders.push_back(CreateCollider(480, 180, 33, 29));
	colliders.push_back(CreateCollider(510, 200, 33, 29));

	return true;
}

update_status ModuleStageOne::Update()
{
	App->renderer->Blit(levelOne, 0, 0, &(background.GetCurrentFrame()),1.0f);
	App->renderer->Blit(levelOne, 986,115, &(waterfall.GetCurrentFrame()), 1.0f);
	App->renderer->Blit(levelOne, 986,227, &(waterfall.GetCurrentFrame()), 1.0f);

	return UPDATE_CONTINUE;
}

bool ModuleStageOne::CleanUp()
{
	LOG("Unloading battletoad stage one scene");

	


	return true;
}

Collider* ModuleStageOne::CreateCollider(int x, int y , int w, int h)
{
	Collider* collider;
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.h = w;
	rect.w = h;
	collider = App->collision->AddCollider(rect);
	collider->colliderType = WALL;
	return collider;
}