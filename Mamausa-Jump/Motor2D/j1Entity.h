#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "j1EntityManager.h"
#include "p2Point.h"
#include "p2DynArray.h"

struct Collider;
class Animation;
struct SDL_Texture;

enum ENTITY_TYPES;

class j1Entity
{
public:
	j1Entity(int x, int y, ENTITY_TYPES type);

	~j1Entity() {}

	virtual bool Start() {
		return true;
	};
	virtual bool Update(float dt) {
		return true;
	};
	virtual bool PostUpdate() {
		return true;
	};
	virtual bool PreUpdate() {
		return true;
	};
	virtual bool CleanUp() {
		return true;
	};

	virtual bool Load(pugi::xml_node&) { return true; };
	virtual bool Save(pugi::xml_node&) { return true; };

	virtual void Draw(SDL_Rect r, bool flip = false, float x = 0, float y = 0);
	virtual void OnCollision(Collider* c1, Collider* c2) {};

	ENTITY_TYPES type;

	fPoint position;

	Animation* animation = nullptr;
	Collider* collider = nullptr;
	SDL_Texture* sprites = nullptr;

}
#endif // __ENTITY_H__