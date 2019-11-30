#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2Point.h"
#include "p2animation.h"
#include "j1Entity.h"

struct SDL_Texture;
struct Collider;

class j1Player : public j1Entity
{

public:
	j1Player(int x, int y, ENTITY_TYPES type);

	virtual ~j1Player();

	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void OnCollision(Collider* c1, Collider* c2);

	void UpdateCameraPosition();
	void LoadPlayerProperties();

	// Animations player
	Animation idle;
	Animation run;
	Animation jump;
	Animation fall;
	Animation godmode;
	Animation death;

	// Sounds
	uint deathSound;
	uint jumpSound;
	uint playerHurt;
	uint attackSound;
	uint lifeup;

	fPoint initialPosition;
	fPoint position;

	fPoint player_position;

	iPoint playerSize;
	iPoint margin;

	uint maxJumps;
	uint colisionMargin;
	uint deathByFallColliderHeight;
	uint points = 0;
	uint score_points = 0;
	uint lives;
	uint initialJumps;

	Collider* attackCollider = nullptr;

	float speed;
	float godModeSpeed;
	float horizontalSpeed;
	float initialVerticalSpeed;
	float verticalSpeed;
	float fallingSpeed;
	float initialFallingSpeed;
	float gravity;

	bool onGround = false;
	bool wallInFront = false;
	bool wallBehind = false;
	bool wallAbove = false;

	bool facingRight = true;

	bool godMode = false;
	bool playerIdle = false;
	bool freefall = false;
	bool jumping = false;
	bool canDoubleJump = true;
	bool doubleJump = false;

	bool playerCanMove = false;
	bool player_start = false;
	bool loading = false;
	bool dead = false;

	bool loadedAudios = false;
}

#endif // __jPLAYER_H__