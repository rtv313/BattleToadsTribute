#include "ModuleEnemies.h"
#include "ModulePlayer.h"
Enemy::Enemy() 
{
	
}

Enemy::Enemy(int x, int y)
{
	position.x = x;
	position.y = y;
	
	state = WALK_ENEMY;
	animation.frames.push_back({ 13, 55, 36, 31 });

	SDL_Rect bodyRect = { x,y,30,30 };
	body = App->collision->AddCollider(bodyRect);
	body->colliderType = ENEMY;

	bodyRect = {x-30,y,30,30};
	sensorLeft = App->collision->AddCollider(bodyRect);
	sensorLeft->colliderType = SENSOR;

	bodyRect = { x+30,y,30,30 };
	sensorRight = App->collision->AddCollider(bodyRect);
	sensorRight->colliderType = SENSOR;
}

Enemy::~Enemy() {}

void Enemy::Update() {

	switch (state) {
		
	case WALK_ENEMY:
		Walk();
		break;
	}
	
}

void Enemy::Walk() {
	iPoint playerPosition = App->player->position;

	if (position.x >= playerPosition.x) 
	{
		position.x -= speed;
	}
	else 
	{
		position.x += speed;
	}
	
}

void Enemy::Attack() {

}

void Enemy::UnderAttack() {

}

void Enemy::Die() {

}

void Enemy::CheckLife() {
}

void Enemy::UpdateCollidersPosition() {
}

void Enemy::onNotify(GameEvent event) {

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
		
		App->renderer->Blit(graphics,(*it)->position.x, (*it)->position.y, &((*it)->animation.GetCurrentFrame()), 1.0f, (*it)->flipHorizontal);

	}

	if (debug == true) 
	{
		DebugDraw();
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
		//App->renderer->DrawQuad((*it)->rect, 255, 0, 0, 80);
	}
}

bool ModuleEnemies::Start() 
{	
	graphics = App->textures->Load("rtype/BattletoadSprites/BT_PsykoPig.gif");
	return true;
}

Enemy* ModuleEnemies::AddEnemy(int x , int y) 
{
	Enemy *enemie = new Enemy(x,y);
	enemies.push_back(enemie);
	return enemie;
}