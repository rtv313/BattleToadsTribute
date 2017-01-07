#include "ModuleEnemies.h"

Enemy::Enemy() {}

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

ModuleEnemies::ModuleEnemies()
{
}


ModuleEnemies::~ModuleEnemies()
{
}


update_status ModuleEnemies::PreUpdate() 
{
	
	return UPDATE_CONTINUE;
}

update_status ModuleEnemies::Update() 
{

	return UPDATE_CONTINUE;
}

bool ModuleEnemies::CleanUp()
{
	return true;
}

void ModuleEnemies::DebugDraw()
{

}

bool ModuleEnemies::Start() 
{
	return true;
}