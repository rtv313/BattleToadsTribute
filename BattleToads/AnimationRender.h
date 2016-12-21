#ifndef __ANIMATION_RENDERH__
#define __ANIMATION_RENDERH__
#include "Animation.h"
#include "Point.h"
#include "Application.h"
#include "ModuleRender.h"

#define NUMBER_FRAMES_PER_SPRITE  9
#define VALID_RENDER_FRAMES 10

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
		
		if (animationCounter >= NUMBER_FRAMES_PER_SPRITE && actualOffset + 1 < animation->frames.size()) {
			++actualOffset;
			animationCounter = 0;
		}
		

		if (flipHorizontal) { // when looks left
			if (animationCounter < VALID_RENDER_FRAMES) // clean ghost
				App->renderer->Blit(graphics, position.x - offsetLeft[actualOffset].x, position.y - offsetLeft[actualOffset].y, &(animation->GetCurrentFrame()), 0.1f, flipHorizontal); // punche sprite
			else
				&(animation->GetCurrentFrame()); // move frames to finish but we not render to clean ghost
		}
		else { // if looks right
			if (animationCounter < VALID_RENDER_FRAMES) // clean ghost
				App->renderer->Blit(graphics, position.x, position.y - offsetRight[actualOffset].y, &(animation->GetCurrentFrame()), 0.1f, flipHorizontal);
			else
				&(animation->GetCurrentFrame()); // move frames to finish but we not render to clean ghost
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


