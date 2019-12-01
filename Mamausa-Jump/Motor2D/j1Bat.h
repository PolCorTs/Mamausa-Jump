#ifndef __j1Bat_H__
#define __j1Bat_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2Point.h"
#include "p2Animation.h"
#include "p2DynArray.h"
#include "j1PathFinding.h"
#include "j1Entity.h"

#define DETECTION_RANGE 100

struct SDL_Texture;
//struct Collider;

class j1Bat : public j1Entity
{
public:
	j1Bat(int x, int y, ENTITY_TYPES type);

	// Destructor
	virtual ~j1Bat();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt, bool do_logic);

	// Called before quitting
	bool CleanUp();

	// Called to check collisions
	void OnCollision(Collider* col_1, Collider* col_2);

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void LoadBatProperties();
	void Move(p2DynArray<iPoint>& path, float dt);

public:

	Animation idle;
	Animation move;

public:
	fPoint initialPosition;

	iPoint margin;
	iPoint colliderSize;

	float speed;

	Movement direction;

	p2DynArray<iPoint>* path;

	bool path_created = false;
	bool dead = false;

};
#endif // __j1Bat_H__
