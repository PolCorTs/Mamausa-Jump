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

	// Destructor
	virtual ~j1Player();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Called to check collisions
	void OnCollision(Collider* c1, Collider* c2);

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void LoadPlayerProperties();
	void Jump();

public:

	// Animations of the player
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
	fPoint player_position;
	fPoint speed;

	iPoint playerSize;
	iPoint margin;

	uint maxJumps;
	uint colisionMargin;
	uint deathByFallColliderHeight;
	uint points = 0;
	uint score_points = 0;
	uint lives;
	uint currentJumps;
	uint initialJumps;

	Collider* attackCollider = nullptr;

	float godModeSpeed;
	float horizontalSpeed;
	//Jumping speed
	float initialVerticalSpeed;
	float verticalSpeed;
	// Free fall speed
	float fallingSpeed;
	float initialFallingSpeed;
	// "Gravity"
	float Gravity;

	// It tells you wether the player has landed, has a wall in front, a wall behind or a wall above
	bool OnGround = false;
	bool wallInFront = false;
	bool wallBehind = false;
	bool wallAbove = false;

	bool facingRight = true;

	bool GodMode = false;
	bool playerIdle = false;
	bool jumping = false;
	bool doubleJump = false;
	bool freefall = false;

	bool playerCanMove = false;
	bool player_start = false;
	bool loading = false;
	bool dead = false;

	int cameraLimit;

private:
	int playerLimit;

	bool loadedAudios = false;
};

#endif // __jPLAYER_H__