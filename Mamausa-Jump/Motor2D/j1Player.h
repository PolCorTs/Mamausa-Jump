//#ifndef __ModulePlayer_H__
//#define __ModulePlayer_H__
//
//#include "j1Module.h"
//#include "j1Animation.h"
//#include "j1Globals.h"
//#include "p2Point.h"
//
//struct SDL_Texture;
//struct Collider;
//
//class j1Player : public j1Module
//{
//public:
//	j1Player();
//	~j1Player();
//
//	bool Start();
//	update_status Update();
//	bool CleanUp();
//	void OnCollision(Collider* collider1, Collider* collider2);
//	void resetPlayer();
//
//public:
//	bool enable_movement = false;
//	int counter = 0;
//	int speed = 2;
//	SDL_Texture * graphics = nullptr;
//	j1Animation idle;
//	j1Animation forward;
//	j1Animation backward;
//	j1Animation upward;
//	j1Animation downward;
//	j1Animation upwardreturn;
//	j1Animation downwardreturn;
//	j1Animation* current_animation;
//	iPoint pos_map_layer;
//	iPoint camera_offset;
//	Uint32 start_time;
//	Uint32 aux_time;
//	Collider* collider;
//	bool dead = false;
//	bool godmode = false;
//	char _godmode[8] = "godmode";
//};
//
//#endif