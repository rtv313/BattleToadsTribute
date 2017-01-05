#include "ModuleSpawnEnemies.h"
#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
ModuleSpawnEnemies::ModuleSpawnEnemies()
{
}


ModuleSpawnEnemies::~ModuleSpawnEnemies()
{
}


update_status ModuleSpawnEnemies::PreUpdate() 
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
}

update_status ModuleSpawnEnemies::Update() 
{
	for (std::list<SpawnTrigger*>::iterator it = spawnTriggers.begin(); it != spawnTriggers.end();++it)
	{
		if ((*it)->CheckCollision(App->player->collider->rect)) 
		{
			(*it)->CreateEnemies(); // start animations for spawn points
		}
	}

	if (debug == true)
		DebugDraw();

}

void ModuleSpawnEnemies::DebugDraw() 
{
	for (std::list<SpawnTrigger*>::iterator it = spawnTriggers.begin(); it != spawnTriggers.end(); ++it)
	{
		App->renderer->DrawQuad((*it)->rect, 0, 0, 255, 80);
	}
}

SpawnTrigger* ModuleSpawnEnemies::AddSpawnTrigger(const SDL_Rect& rect)
{
	SpawnTrigger *trigger = new SpawnTrigger(rect);
	spawnTriggers.push_back(trigger);
	return trigger;
}