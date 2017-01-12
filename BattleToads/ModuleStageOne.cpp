#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "ModuleSpawnEnemies.h"
#include "ModuleStageOne.h"
#include "ModuleEnemies.h"
#include "ModuleFadeToBlack.h"
#define END_LEVEL 1174
ModuleStageOne::ModuleStageOne(bool active):Module(active)
{
}


ModuleStageOne::~ModuleStageOne()
{
}

bool ModuleStageOne::Start() {

	LOG("Loading space scene");

	levelOne = App->textures->Load("Assets/BattletoadSprites/level1.png");
	background = Animation();
	waterfall =  Animation();
	bridge = Animation();

	App->player->Enable();
	App->collision->Enable();
	App->enemies->Enable();
	App->spawnTriggers->Enable();
	App->spawnZones->Enable();
	App->audio->PlayMusic("Assets/Music/firstBackgroundMusic.ogg", 1.0f);
	App->renderer->camera.x = App->renderer->camera.y = 0;

	background.frames.push_back({ 518, 7, 1404, 357 });
	background.speed = 0.08f;

	bridge.frames.push_back({583,288,114,23});

	waterfall.frames.push_back({1703,380,32,112});
	waterfall.frames.push_back({1736,380,32,112});
	waterfall.frames.push_back({1769,380,32,112});
	waterfall.frames.push_back({1802,380,32,112});
	waterfall.frames.push_back({1835,380,32,112});
	waterfall.speed = 0.1f;


	colliders.push_back(CreateCollider(-20, 80, 240, 35,WALL));
	colliders.push_back(CreateCollider(0,310,47,632, WALL));
	colliders.push_back(CreateCollider(0, 150, 32, 92, WALL));
	colliders.push_back(CreateCollider(90, 160, 33, 29, WALL));
	colliders.push_back(CreateCollider(119, 180, 33, 29, WALL));
	colliders.push_back(CreateCollider(150, 160, 33, 29, WALL));
	colliders.push_back(CreateCollider(185, 150, 32, 72, WALL));
	colliders.push_back(CreateCollider(255, 160, 33, 29, WALL));
	colliders.push_back(CreateCollider(281, 180, 33, 29, WALL));
	colliders.push_back(CreateCollider(310, 160, 33, 29, WALL));
	colliders.push_back(CreateCollider(350, 150, 32, 100, WALL));
	colliders.push_back(CreateCollider(450, 160, 33, 29, WALL));
	colliders.push_back(CreateCollider(480, 180, 33, 29, WALL));
	colliders.push_back(CreateCollider(510, 200, 33, 29, WALL));
	colliders.push_back(CreateCollider(540, 210, 33, 29, WALL));
	colliders.push_back(CreateCollider(570, 210, 33, 29, WALL));
	colliders.push_back(CreateCollider(610, 190, 33, 20, WALL));
	colliders.push_back(CreateCollider(635, 170, 33, 20, WALL));
	colliders.push_back(CreateCollider(670, 150, 33, 25, WALL));
	colliders.push_back(CreateCollider(700, 140, 33, 29, WALL));
	colliders.push_back(CreateCollider(740, 120, 33, 29, WALL));
	colliders.push_back(CreateCollider(775, 120, 33, 29, WALL));
	colliders.push_back(CreateCollider(795, 150, 33, 320, WALL));
	colliders.push_back(CreateCollider(640, 345, 33, 130, WALL));
	colliders.push_back(CreateCollider(775, 330, 10, 10, WALL));
	colliders.push_back(CreateCollider(780, 310, 20, 110, WALL));
	colliders.push_back(CreateCollider(910, 280, 20, 20, WALL));
	colliders.push_back(CreateCollider(940, 250, 20, 120, WALL));
	colliders.push_back(CreateCollider(1050, 280, 20, 120, WALL));
	colliders.push_back(CreateCollider(1125, 150, 20, 20, WALL));
	colliders.push_back(CreateCollider(1150, 130, 20, 40, WALL));
	colliders.push_back(CreateCollider(1200, 150, 90, 20, WALL));
	colliders.push_back(CreateCollider(1185, 270, 30, 20, WALL));

	
	////////////////////////////////////////
	
	SpawnTrigger *trigger = App->spawnTriggers->AddSpawnTrigger(190,180,200,30);
	SpawnTrigger *triggerTwo = App->spawnTriggers->AddSpawnTrigger(388, 180, 200, 30);
	SpawnTrigger *triggerThree = App->spawnTriggers->AddSpawnTrigger(686, 180, 250, 30);
	SpawnTrigger *triggerFour = App->spawnTriggers->AddSpawnTrigger(986, 220, 200, 30);

	///////////////////////////////////////////
	
	
	trigger->AddSpawnZone(32, 206, 30, 30);
	trigger->AddSpawnZone(200, 248, 30, 30);

	triggerTwo->AddSpawnZone(378, 210, 30, 30);
	triggerTwo->AddSpawnZone(456, 250, 30, 30);

	triggerThree->AddSpawnZone(708, 252, 30, 30);
	triggerThree->AddSpawnZone(766, 258, 30, 30);
	triggerThree->AddSpawnZone(866, 210, 30, 30);

	triggerFour->AddSpawnZone(1070, 184, 30, 30);
	triggerFour->AddSpawnZone(1088, 226, 30, 30);

	return true;
}

update_status ModuleStageOne::Update()
{

	if (App->player->position.x >= END_LEVEL && App->fade->isFading() == false) { // restart game
		App->player->Disable();
		App->collision->CleanUp();
		App->enemies->CleanUp();
		App->player->CleanUp();
		App->spawnTriggers->CleanUp();
		App->spawnZones->CleanUp();
		App->fade->FadeToBlack((Module*)App->scene_intro, this, 1);

	}
	App->renderer->Blit(levelOne, 0, 0, &(background.GetCurrentFrame()),1.0f);
	App->renderer->Blit(levelOne, 986,115, &(waterfall.GetCurrentFrame()), 1.0f);
	App->renderer->Blit(levelOne, 986,227, &(waterfall.GetCurrentFrame()), 1.0f);
	App->renderer->Blit(levelOne, 950,225, &(bridge.GetCurrentFrame()),1.0f);
	return UPDATE_CONTINUE;
}

bool ModuleStageOne::CleanUp()
{
	LOG("Unloading battletoad stage one scene");

	
	App->textures->Unload(levelOne);

	return true;
}

Collider* ModuleStageOne::CreateCollider(int x, int y , int w, int h, ColliderType colliderType)
{
	Collider* collider;
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.h = w;
	rect.w = h;
	collider = App->collision->AddCollider(rect);
	collider->colliderType = colliderType;
	return collider;
}