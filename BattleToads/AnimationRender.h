#ifndef __ANIMATION_RENDERH__
#define __ANIMATION_RENDERH__
#include "Animation.h"
#include "Point.h"
#include "Application.h"
#include "ModuleRender.h"


class AnimationRender {

public:
	
	AnimationRender() {}
	~AnimationRender(){}

	void Update(Application* const App, SDL_Texture* const graphics, Animation* const animation, bool const flipHorizontal, iPoint const position, vector<iPoint> &offsetLeft, vector<iPoint> &offsetRight)
	{

		if (flipHorizontal)  // when looks left
			Render(App, graphics, animation, flipHorizontal, position, offsetLeft, offsetRight);
		else  // if looks right
			Render(App, graphics, animation, flipHorizontal, position, offsetLeft, offsetRight);
	}
	
private:

	
	int actualOffset = 0;

	void Render(Application* const App, SDL_Texture* const graphics, Animation* const animation, bool const flipHorizontal, iPoint const position, vector<iPoint> &offsetLeft, vector<iPoint> &offsetRight) {
		SDL_Rect * sprite = &(animation->GetCurrentFrame());
		actualOffset = (int)(animation->current_frame);
		if (flipHorizontal == true)
			App->renderer->Blit(graphics, position.x - offsetLeft[actualOffset].x, position.y - offsetLeft[actualOffset].y, sprite, 0.1f, flipHorizontal);
		else
			App->renderer->Blit(graphics, position.x - offsetRight[actualOffset].x, position.y - offsetRight[actualOffset].y,sprite, 0.1f, flipHorizontal);
		}
};




#endif


