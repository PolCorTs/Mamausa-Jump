#ifndef __J1ENTITYMANAGER_H__
#define __J1ENTITYMANAGER_H__

#include "j1Module.h"
#include "p2Defs.h"
#include "p2List.h"
#include "p2Point.h"
//#include "j1Entity.h"

#define MAX_ENTITIES 100

class j1Entity;
class j1Player;

enum ENTITY_TYPES
{
	PLAYER,
	UNKNOWN
};

struct EntityInfo
{
	ENTITY_TYPES type = ENTITY_TYPES::UNKNOWN;
	iPoint position;
};

class j1EntityManager : public j1Module
{
public:
	// Constructor
	j1EntityManager();

	// Destructor
	~j1EntityManager();

	bool Awake(pugi::xml_node&);
	bool Start();

	// Called every frame
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	j1Entity* CreateEntity(ENTITY_TYPES type, int x = 0, int y = 0);

	/*void OnCollision(Collider* c1, Collider* c2);
	void CreatePlayer();
	void AddEnemy(int x, int y, ENTITY_TYPES type);
	void DestroyEntities();*/

public:

	p2List<j1Entity*>	entities;

	j1Player*			player = nullptr;

private:

	EntityInfo			queue[MAX_ENTITIES];

};

#endif // __J1ENTITYMANAGER_H__