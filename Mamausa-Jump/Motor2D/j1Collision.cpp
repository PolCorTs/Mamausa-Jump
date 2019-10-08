//#include "j1App.h"
//#include "j1Input.h"
//#include "j1Render.h"
//#include "j1Player.h"
//#include "j1Collision.h"
//#include "p2Log.h"
//#include "j1Globals.h"
//
//
//j1Collision::j1Collision()
//{
//	for (uint i = 0; i < MAX_COLLIDERS; ++i)
//		colliders[i] = nullptr;
//
//	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
//	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = true;
//	matrix[COLLIDER_WALL][COLLIDER_ENEMY] = true;
//	matrix[COLLIDER_WALL][COLLIDER_POWERUP] = false;
//
//	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
//	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
//	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY] = true;
//	matrix[COLLIDER_PLAYER][COLLIDER_POWERUP] = true;
//
//	matrix[COLLIDER_ENEMY][COLLIDER_WALL] = true;
//	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER] = true;
//	matrix[COLLIDER_ENEMY][COLLIDER_ENEMY] = false;
//	matrix[COLLIDER_ENEMY][COLLIDER_POWERUP] = false;
//
//	matrix[COLLIDER_POWERUP][COLLIDER_WALL] = false;
//	matrix[COLLIDER_POWERUP][COLLIDER_PLAYER] = true;
//	matrix[COLLIDER_POWERUP][COLLIDER_ENEMY] = false;
//	matrix[COLLIDER_POWERUP][COLLIDER_POWERUP] = false;
//}
//
//// Destructor
//j1Collision::~j1Collision()
//{}
//
//update_status j1Collision::PreUpdate()
//{
//	// Remove all colliders scheduled for deletion
//	for (uint i = 0; i < MAX_COLLIDERS; ++i)
//	{
//		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
//		{
//			delete colliders[i];
//			colliders[i] = nullptr;
//		}
//	}
//
//	// Calculate collisions
//	Collider* c1;
//	Collider* c2;
//
//	for (uint i = 0; i < MAX_COLLIDERS; ++i)
//	{
//		// skip empty colliders
//		if (colliders[i] == nullptr)
//			continue;
//
//		c1 = colliders[i];
//
//		// avoid checking collisions already checked
//		for (int k = i + 1; k < MAX_COLLIDERS; ++k)
//		{
//			// skip empty colliders
//			if (colliders[k] == nullptr)
//				continue;
//
//			c2 = colliders[k];
//			if (c2 != nullptr) {
//
//				if (c1->CheckCollision(c2->rect) == true)
//				{
//					if (matrix[c1->type][c2->type] && c1->callback)
//						c1->callback->OnCollision(c1, c2);
//
//					if (matrix[c2->type][c1->type] && c2->callback)
//						c2->callback->OnCollision(c2, c1);
//				}
//			}
//		}
//	}
//
//	return UPDATE_CONTINUE;
//}
//
//// Called before render is available
//update_status j1Collision::Update()
//{
//
//	DebugDraw();
//
//	return UPDATE_CONTINUE;
//}
//
//
//
//// Called before quitting
//bool j1Collision::CleanUp()
//{
//	LOG("Freeing all colliders");
//
//	for (uint i = 0; i < MAX_COLLIDERS; ++i)
//	{
//		if (colliders[i] != nullptr)
//		{
//			delete colliders[i];
//			colliders[i] = nullptr;
//		}
//	}
//
//	return true;
//}
//
//Collider* j1Collision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
//{
//	Collider* ret = nullptr;
//
//	for (int i = 0; i < MAX_COLLIDERS; ++i)
//	{
//		if (colliders[i] == nullptr)
//		{
//			ret = colliders[i] = new Collider(rect, type, callback);
//			break;
//		}
//	}
//
//	return ret;
//}
//
//// -----------------------------------------------------
//
//bool Collider::CheckCollision(const SDL_Rect& r) const
//{
//	if (r.y + r.h <= rect.y || r.y >= rect.y + rect.h || r.x + r.w <= rect.x || r.x >= rect.x + rect.w)
//		return false;
//	else
//		return true;
//}