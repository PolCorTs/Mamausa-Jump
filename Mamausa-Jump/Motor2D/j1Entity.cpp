#include "j1Entity.h"
#include "j1App.h"
#include "j1Render.h"

j1Entity::j1Entity(int x, int y, ENTITY_TYPES type) : position(x, y), type(type)
{
}

void j1Entity::Draw(SDL_Rect r, bool flip, float x, float y)
{
	if (flip == true)
	{
		App->render->Blit(sprites, x + 2, y + 417, &r, SDL_FLIP_NONE);
	}
	else
	{
		App->render->Blit(sprites, x + 2, y + 417, &r, SDL_FLIP_HORIZONTAL);
	}
}