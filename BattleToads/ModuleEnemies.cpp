#include "ModuleEnemies.h"
#include "ModulePlayer.h"
#include "ModuleAudio.h"
Enemy::Enemy() 
{
	
}

Enemy::Enemy(int x, int y)
{
	position.x = x;
	position.y = y;
	life = 1000;
	state = WALK_ENEMY;

	animationRender = AnimationRender();

	animationWalk.frames.push_back({ 14, 55, 32, 30 });
	animationWalk.frames.push_back({ 54, 56, 33, 30 });
	animationWalk.frames.push_back({ 94, 60, 32, 29 });
	animationWalk.speed = 0.1;
	offsetLeftWalk = { {1,0},{1,0},{1,0} };
	offsetRightWalk = { {0,0},{0,0},{0,0} };

	animationAttack.frames.push_back({ 50, 15, 34, 31 });
	animationAttack.frames.push_back({ 91, 17, 40, 28 });
	animationAttack.frames.push_back({ 91, 17, 40, 28 });
	animationAttack.speed = 0.1;
	animationAttack.loop = false;
	offsetLeftAttack = { {0,0},{16,0},{16,0} };
	offsetRightAttack = { {3,0},{-2,0},{0,0} };

	animationDead.frames.push_back({145,64,32,35});
	animationDead.frames.push_back({ 14,100,32,24 });
	animationDead.speed = 0.1;
	animationDead.loop = false;
	offsetLeftDead = { {0,0},{0,0,},{0,0} };
	offsetRightDead = { { 0,0 },{ 0,0, },{ 0,0 } };

	SDL_Rect bodyRect = { x,y,30,30 };
	body = App->collision->AddCollider(bodyRect);
	body->colliderType = ENEMY;
	body->addObserver(this);

	bodyRect = {x-5,y,5,5};
	sensor = App->collision->AddCollider(bodyRect);
	sensor->colliderType = SENSOR;
	sensor->addObserver(this);
}

Enemy::~Enemy() {}

void Enemy::Update() {

	if (life <= 0) {
		state = DIE_ENEMY;
	}

	switch (state) {
		
	case WALK_ENEMY:
		Walk();
		break;
	case ATTACK_ENEMY:
		Attack();
		break;
	case MOVE_OTHER_SIDE:
		MoveOtherSide();
		break;
	case ENEMY_IDLE:
		Idle();
		break;
	case DIE_ENEMY:
		Die();
		break;

	default:
		break;
	}
	//App->renderer->Blit(App->enemies->graphics, position.x, position.y, &(currentAnimation->GetCurrentFrame()), 1.0f,flipHorizontal);
	UpdateCollidersPosition();
}

void Enemy :: MoveOtherSide() 
{
	
	currentAnimation = &animationWalk;
	if (flipHorizontal == true && position.x > targetPositionAttack)
	{
		position.x -= speed;

		if (position.x <= targetPositionAttack) {
			state = WALK_ENEMY;
			return;
		}
	
	}
	else if(flipHorizontal == false && position.x < targetPositionAttack)
	{
		position.x += speed;
		if (position.x >= targetPositionAttack) {
			state = WALK_ENEMY;
			return;
		}
	}

	animationRender.Update(App, App->enemies->graphics, currentAnimation, flipHorizontal, position, offsetLeftWalk, offsetRightWalk);

}

void Enemy::Walk() 
{
	iPoint playerPosition = App->player->position;
	currentAnimation = &animationWalk;
	if (position.x == playerPosition.x) {
		return;
	}

	if (position.x > playerPosition.x) 
	{
		position.x -= speed;
		flipHorizontal = true;
	}
	else if(position.x < playerPosition.x)
	{
		position.x += speed;
		flipHorizontal = false;
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
		if (position.y <= wallPositionTarget) {
			position.y += speed;
			if (position.y >= wallPositionTarget) {
				position.y = wallPositionTarget;
			}
		}
	}

	animationRender.Update(App, App->enemies->graphics, currentAnimation, flipHorizontal, position, offsetLeftWalk, offsetRightWalk);
}

void Enemy::Attack() {

	currentAnimation = &animationAttack;

	if (flagAudio == true)
	{
		App->audio->PlayFx(App->enemies->audioIDpunch);
		flagAudio = false;
	}
	App->player->life -= 10;

	animationRender.Update(App, App->enemies->graphics, currentAnimation, flipHorizontal, position, offsetLeftAttack, offsetRightAttack);


	if (currentAnimation->Finished()) {
		state = WALK_ENEMY;
		
		flagAudio = true;
		animationAttack.Reset();
	}
}

void Enemy::Idle() {
	if (tiempoIdle < 90) { // for testing
		tiempoIdle++;
	}

	if (tiempoIdle >= 90) {
		state = WALK_ENEMY;
		tiempoIdle = 0;
	}

	animationRender.Update(App, App->enemies->graphics, currentAnimation, flipHorizontal, position, offsetLeftAttack, offsetRightAttack);


}

void Enemy::UnderAttack() {

}

void Enemy::Die() {
	currentAnimation = &animationDead;

	if (flagAudio == true)
	{
		App->audio->PlayFx(App->enemies->audioIDdamage);
		flagAudio = false;
	}

	animationRender.Update(App, App->enemies->graphics, currentAnimation, flipHorizontal, position, offsetLeftDead, offsetRightDead);
}

void Enemy::CheckLife() {
}

void Enemy::UpdateCollidersPosition() {

	body->rect.x = position.x;
	body->rect.y = position.y;
	if(flipHorizontal == true)
		sensor->rect.x = position.x-5;
	else
		sensor->rect.x = position.x+30;
	sensor->rect.y = position.y;
	
}

void Enemy::onNotify(GameEvent event) {
	if ((state == MOVE_OTHER_SIDE || state == ATTACK_ENEMY) &&  event != ENEMY_DAMAGE)  { //||  state == ATTACK_ENEMY
		return; 
	}
	switch (event) {
	
		case NO_COLLISION:
			go_down = false;
			state = WALK_ENEMY;
			break;

		case PLAYER_COLLISION:
			state = ATTACK_ENEMY;
			break;

		case ENEMY_DAMAGE:
			life -= 10;
			break;

		default:
			break;
	}
}

void Enemy::onNotify(GameEvent event,int position) {

	if (state == MOVE_OTHER_SIDE || state == ATTACK_ENEMY ) { // || state == ATTACK_ENEMY
		return;
	}

	switch (event) {

	case WALL_COLLISION:
		go_down = true;
		wallPositionTarget= position-1;
		break;
	
	case ENEMY_COLLISION:
		if (state != ATTACK_ENEMY && PlayerInYourDirection()) {
			state = MOVE_OTHER_SIDE;
			if(flipHorizontal==true)
				targetPositionAttack = position - 60;
			else
				targetPositionAttack = position + 60;
		}	
		break;
	default:
		break;
	}
}

bool Enemy::PlayerInYourDirection() {
	if (flipHorizontal == false && App->player->position.x > position.x)
	{
		return true;
	}

	if (flipHorizontal == true && App->player->position.x < position.x) {
		return true;
	}

	return false;
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
	bool twoMovingSides = false;

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



bool ModuleEnemies::Start() 
{	
	graphics = App->textures->Load("rtype/BattletoadSprites/BT_PsykoPig.gif");
	audioIDpunch = App->audio->LoadFx("rtype/Music/SoundsFX/stickHit.ogg");
	audioIDdamage = App->audio->LoadFx("rtype/Music/SoundsFX/toadFall.ogg");
	return true;
}

Enemy* ModuleEnemies::AddEnemy(int x , int y) 
{
	Enemy *enemie = new Enemy(x,y);
	enemies.push_back(enemie);
	return enemie;
}

bool ModuleEnemies::EnemiesAlive() {
	
	for (list<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); ++it) {
		if ((*it)->state != DIE_ENEMY) {
			return true;
		}
	}
	return false;

}