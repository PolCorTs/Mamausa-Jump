#ifndef __j1FadeToBlack_H__
#define __j1FadeToBlack_H__

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"



class j1FadeToBlack : public j1Module
{
public:
	j1FadeToBlack();
	~j1FadeToBlack();

	bool Start();
	update_status Update();
	bool FadeToBlack(j1Module* module_off, j1Module* module_on, float time = 2.0f);
	bool FadeToBlack(float time = 1.0f);
	bool FadeonlyBlack(float time = 1.0f);
	void resetStep();
	bool isBlack = false;
	bool isfadding = false;

private:
	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black,
		fade_only_black
	}current_step;
	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Rect screen;
	j1Module* module_off;
	j1Module* module_on;
	bool switch_scene = false;
};

#endif //__j1FadeToBlack_H__