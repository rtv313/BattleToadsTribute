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
	body->addObserver(this);

	bodyRect = {x-30,y,30,30};
	sensorLeft = App->collision->AddCollider(bodyRect);
	sensorLeft->colliderType = SENSOR;
	sensorLeft->addObserver(this);
	

	/*bodyRect = { x+30,y,30,30 };
	sensorRight = App->collision->AddCollider(bodyRect);
	sensorRight->colliderType = SENSOR;
	sensorRight->addObserver(this);*/
}

Enemy::~Enemy() {}

void Enemy::Update() {

	switch (state) {
		
	case WALK_ENEMY:
		Walk();
		break;

	default:
		break;
	}
	UpdateCollidersPosition();
}

void Enemy::Walk() {
	iPoint playerPosition = App->player->position;
	
	if (position.x == playerPosition.x) {
		return;
	}

	if (position.x > playerPosition.x) 
	{
		position.x -= speed;
	}
	else if(position.x < playerPosition.x)
	{
		position.x += speed;
	}
	if (go_down == false) {
		if (position.y > playerPosition.y)
		{
			position.y -= speed;
		}
		else if (position.y < playerPosition.y)
		{
			position.y += speed;
		}
	}
	else {
		//position.y += speed + go_down_offset;
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

	body->rect.x = position.x;
	body->rect.y = position.y;
	sensorLeft->rect.x = position.x-30;
	sensorLeft->rect.y = position.y;
	//sensorRight->rect.x = position.x+30;
	//sensorRight->rect.y = position.y;
}

void Enemy::onNotify(GameEvent event) {
	switch (event) {
		case WALL_COLLISION:
			//go_down = true;
			break;
		case NO_COLLISION:
			go_down = false;
			go_down_offset = 0;
			break;
		default:
			break;
	}
}

void Enemy::onNotify(GameEvent event,int downPosition) {
	switch (event) {
	case WALL_COLLISION:
		//go_down = true;
		position.y = downPosition;
		break;
	default:
		break;
	}
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