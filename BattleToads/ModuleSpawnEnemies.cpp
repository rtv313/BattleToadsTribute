#include "ModuleSpawnEnemies.h"
#include "Application.h"
#include "ModulePlayer.h"


ModuleSpawnTriggers::ModuleSpawnTriggers(){}
ModuleSpawnTriggers::~ModuleSpawnTriggers(){}


update_status ModuleSpawnTriggers::PreUpdate() 
{
	for (std::list<SpawnTrigger*>::iterator it = spawnTriggers.begin(); it != spawnTriggers.end();) 
	{
		if ((*it)->to_delete == true)
		{
			RELEASE(*it);
			it = spawnTriggers.erase(it);
		}
		else
			++it;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleSpawnTriggers::Update() 
{
	for (std::list<SpawnTrigger*>::iterator it = spawnTriggers.begin(); it != spawnTriggers.end();++it)
	{
		if ((*it)->CheckCollision(App->player->collider->rect)) 
		{
			(*it)->CreateEnemies(); // start animations for spawn points
			(*it)->to_delete = true;
		}
	}

	if (debug == true)
		DebugDraw();
	return UPDATE_CONTINUE;
}

void ModuleSpawnTriggers::DebugDraw() 
{
	for (std::list<SpawnTrigger*>::iterator it = spawnTriggers.begin(); it != spawnTriggers.end(); ++it)
	{
		App->renderer->DrawQuad((*it)->rect, 0, 0, 255, 80);
	}
}

SpawnTrigger* ModuleSpawnTriggers::AddSpawnTrigger(const int  x, const int  y , const int  h, const int  w)
{
	SDL_Rect triggerRec;
	triggerRec.h = h;
	triggerRec.w = w;
	triggerRec.x = x;
	triggerRec.y = y;

	SpawnTrigger *trigger = new SpawnTrigger(triggerRec);
	spawnTriggers.push_back(trigger);
	return trigger;
}

bool ModuleSpawnTriggers::CleanUp() 
{
	for (list<SpawnTrigger*>::iterator it = spawnTriggers.begin(); it != spawnTriggers.end(); ++it)
		RELEASE(*it);

	spawnTriggers.clear();

	return true;
}




ModuleSpawnZones::ModuleSpawnZones() {}
ModuleSpawnZones::~ModuleSpawnZones() {}

update_status ModuleSpawnZones::PreUpdate()
{
	for (std::list<SpawnZone*>::iterator it = spawnZones.begin(); it != spawnZones.end();)
	{
		if ((*it)->to_delete == true)
		{
			RELEASE(*it);
			it = spawnZones.erase(it);
		}
		else
			++it;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleSpawnZones::Update()
{
	for (std::list<SpawnZone*>::iterator it = spawnZones.begin(); it != spawnZones.end(); ++it)
	{
		if((*it)->activate)
			App->renderer->Blit(spawnSprite, (*it)->rect.x, (*it)->rect.y, &(spawnAnimationOne.GetCurrentFrame()), 1.0f);
	}

	if (debug == true)
		DebugDraw();

	return UPDATE_CONTINUE;
}

SpawnZone * ModuleSpawnZones::AddSpawnZone(const int  x, const int  y, const int  h, const int  w)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.h = h;
	rect.w = w;
	SpawnZone *spawnZone = new SpawnZone(rect);
	spawnZones.push_back(spawnZone);
	return spawnZone;
}

void ModuleSpawnZones::DebugDraw()
{	
	for (std::list<SpawnZone*>::iterator it = spawnZones.begin(); it != spawnZones.end(); ++it)
	{	if((*it)->activate)
			App->renderer->DrawQuad((*it)->rect, 255, 0, 0, 80);
	}
}


bool ModuleSpawnZones::CleanUp() 
{
	return true;
}

bool ModuleSpawnZones::Start() 
{
	spawnSprite = App->textures->Load("rtype/BattletoadSprites/PsykoPig.png");
	spawnAnimationOne;
	spawnAnimationOne.frames.push_back({ 290,173, 30, 53 });
	spawnAnimationOne.speed = 0.1;
	spawnAnimationOne.loop = false;
	return true;
}
// SPAWNTRIGGER

SpawnTrigger::SpawnTrigger(const SDL_Rect& rectangle):rect(rectangle)
{

}

bool SpawnTrigger::CheckCollision(const SDL_Rect& r)const
{
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	leftA = rect.x;
	rightA = rect.x + rect.w;
	topA = rect.y;
	bottomA = rect.y + rect.h;

	leftB = r.x;
	rightB = r.x + r.w;
	topB = r.y;
	bottomB = r.y + r.h;

	if (bottomA <= topB) {
		return false;
	}

	if (topA >= bottomB) {
		return false;
	}

	if (rightA <= leftB) {
		return false;
	}

	if (leftA >= rightB) {
		return false;
	}

	return true;
}

void SpawnTrigger::CreateEnemies() 
{
	//send signal to activate animations for spawn
	for (std::list<SpawnZone*>::iterator it = spawnZones.begin(); it != spawnZones.end(); ++it)
	{
		(*it)->activate = true;
	}
}

void SpawnTrigger::AddSpawnZone(const int  x, const int  y, const int  h, const int  w)
{
	spawnZones.push_back(App->spawnZones->AddSpawnZone(x,y,h,w));
}

