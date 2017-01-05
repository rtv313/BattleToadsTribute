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
}

update_status ModuleSpawnEnemies::Update() 
{
	for (std::list<SpawnZone*>::iterator it = spawnZones.begin(); it != spawnZones.end();++it)
	{
		if ((*it)->CheckCollision(App->player->collider->rect)) 
		{

		}
	}
}