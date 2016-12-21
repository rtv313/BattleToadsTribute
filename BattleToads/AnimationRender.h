#ifndef __ANIMATION_RENDERH__
#define __ANIMATION_RENDERH__
#include "Animation.h"
#include "Point.h"
#include "Application.h"
#include "ModuleRender.h"

class AnimationRender {

public:
	vector<iPoint> offsetLeft;
	vector<iPoint> offsetRight;
	AnimationRender() {}
	AnimationRender(vector<iPoint> offsetLeft, vector<iPoint> offsetRight):
		offsetLeft(offsetLeft), 
		offsetRight(offsetRight)
	{}

	~AnimationRender(){}

	void Play(Application* const App, SDL_Texture* const graphics, Animation* const animation,bool const flipHorizontal,iPoint const position)
	{
		
		if (animationCounter == 9 && actualOffset + 1 < animation->frames.size()) {
			++actualOffset;
			animationCounter = 0;
		}
		

		if (flipHorizontal) { // when looks left
			App->renderer->Blit(graphics, position.x - offsetLeft[actualOffset].x, position.y - offsetLeft[actualOffset].y, &(animation->GetCurrentFrame()), 0.1f, flipHorizontal); // punche sprite
		}
		else { // if looks right
			
			App->renderer->Blit(graphics, position.x, position.y- offsetRight[actualOffset].y, &(animation->GetCurrentFrame()), 0.1f, flipHorizontal);
		}
		
		++animationCounter; 

		if (animation->Finished()) {
			animationCounter = 0;
			actualOffset = 0;
			return;
		}
	}
	
private:
	int animationCounter = 0; 
	int actualOffset = 0;

};




#endif


