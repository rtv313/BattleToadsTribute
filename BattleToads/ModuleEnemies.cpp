#include "ModuleEnemies.h"

Enemy::Enemy() 
{
	
}

Enemy::~Enemy() {}

update_status Enemy::Update() {
	return UPDATE_CONTINUE;
}

void Enemy::Walk() {
	
}

void Enemy::Attack() {

}

void Enemy::UnderAttack() {

}

void Enemy::Die() {

}

void Enemy::CheckLife() {
}

ModuleEnemies::ModuleEnemies(bool active):Module(active) {}


ModuleEnemies::~ModuleEnemies(){}


update_status ModuleEnemies::PreUpdate() 
{
	for (list<Enemy*>::iterator it = enemies.begin(); it != enemies.end();)
	{
		if ((*it)->to_delete == true)
		{
			RELEASE(*it);
			it = enemies.erase(it);
		}
		else
			++it;
	}
	
	return UPDATE_CONTINUE;
}

update_status ModuleEnemies::Update() 
{
	for (list<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); ++it) 
	{
		(*it)->Update();
	}

	return UPDATE_CONTINUE;
}

bool ModuleEnemies::CleanUp()
{
	for (list<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); ++it)
		RELEASE(*it);

	enemies.clear();

	return true;
}

void ModuleEnemies::DebugDraw()
{
	for (std::list<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); ++it)
	{
		App->renderer->DrawQuad((*it)->rect, 255, 0, 0, 80);
	}
}

bool ModuleEnemies::Start() 
{	
	graphics = App->textures->Load("rtype/BattletoadSprites/PsykoPig.png");
	return true;
}

Enemy* ModuleEnemies::AddEnemy() 
{
	Enemy *enemie = new Enemy();
	enemies.push_back(enemie);
	return enemie;
}