#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#include<list>
#include "Module.h"
#include "Observer.h"
// TODO 9: Create a matrix of game specific types of collision for early discard
// Example: lasers should not collide with lasers but should collider with walls
// enemy shots will collide with other enemies ? and against walls ?

enum ColliderType {
	PLAYER,
	PLAYER_LASER,
	ENEMY_LASER,
	WALL,
	ENEMY,
};



class Collider
{
private:
	Observer* observers_[2]; //use array just for educational puropose use list in production
	int numObservers_;

	

public:
	SDL_Rect rect = { 0,0,0,0 };
	bool to_delete = false;
	ColliderType colliderType;
	// TODO 10: Add a way to notify other classes that a collision happened
	
	void ValidCollision(Collider * collider);
	void addObserver(Observer* observer);
	void removeObserver(Observer* observer);
	bool CollisionMatrix[5][5] = { { false,false,true,true,true },
									{ false,false,false,true,true },
									{ true,false,false,true,false },
									{ true,true,true,false,false },
									{ true,true,false,true,false } };

	Collider(SDL_Rect rectangle) : // expand this call if you need to
		rect(rectangle)
	{
		numObservers_ = 0;
	}

	Collider() {
	}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const SDL_Rect& r) const;
};

class ModuleCollision : public Module
{
public:

	ModuleCollision();
	~ModuleCollision();

	update_status PreUpdate();
	update_status Update();

	bool CleanUp();

	Collider* AddCollider(const SDL_Rect& rect);
	void DebugDraw();



private:
	std::list<Collider*> colliders;
	bool debug = false;
};

#endif // __ModuleCollision_H__