#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleFadeToBlack.h"
#include "ModuleLevelIntro.h"
#include "ModulePlayer.h"



ModuleLevelIntro::ModuleLevelIntro(bool active) : Module(active)
{
}


ModuleLevelIntro::~ModuleLevelIntro()
{
}

bool ModuleLevelIntro::Start()
{
	background = App->textures->Load("Assets/BattletoadSprites/level1.png");
	backSelection.frames.push_back({ 259,8,242,218 });
	App->audio->PlayMusic("Assets/Music/firstBackgroundMusic.ogg", 1.0f);
	App->renderer->camera.x = App->renderer->camera.y = 0;

	return true;
}

bool ModuleLevelIntro::CleanUp() 
{
	App->textures->Unload(background);

	return true;
}

update_status ModuleLevelIntro::Update() 
{
	App->renderer->Blit(background, SCREEN_WIDTH / 2 - 120, 15, &(backSelection.GetCurrentFrame()), 1.0f);

	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && App->fade->isFading() == false)
	{
		
		App->fade->FadeToBlack((Module*)App->stageOne, this);

	}

	return UPDATE_CONTINUE;

}