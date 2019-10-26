#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1Input.h"
#include "j1Player.h"
#include "j1Render.h"
#include "j1FadeToBlack.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1Window.h"

j1Player::j1Player(int x, int y, ENTITY_TYPES type) : j1Entity(x, y, ENTITY_TYPES::PLAYER)
{
	animation = NULL;

	player_position = { (float)x, (float)y };

	idle.LoadAnimations("idle");
	run.LoadAnimations("run");
	jump.LoadAnimations("jump");
	fall.LoadAnimations("fall");
	godmode.LoadAnimations("godmode");
	death.LoadAnimations("death");
}

j1Player::~j1Player() {}

bool j1Player::Start() {

	LOG("Loading player textures");
	sprites = App->tex->Load("textures/AlienBlue.png");

	LoadPlayerProperties();

	collider = App->collision->AddCollider({ (int)player_position.x + margin.x, (int)player_position.y + margin.y, playerSize.x, playerSize.y }, COLLIDER_PLAYER, App->entity);

	player_start = true;

	speed = { 1, 1 };

	return true;
}

bool j1Player::PreUpdate() {

	return true;
}

bool j1Player::Update(float dt) {

	if (player_start)
	{

		if (GodMode) {

			animation = &idle;

			if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_REPEAT)
			{
				player_position.x += godModeSpeed * dt;
				facingRight = true;
			}

			if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_REPEAT)
			{
				player_position.x -= godModeSpeed * dt;
				facingRight = false;
			}

			if (App->input->GetKey(SDL_SCANCODE_W) == j1KeyState::KEY_REPEAT)
				player_position.y -= godModeSpeed * dt;

			if (App->input->GetKey(SDL_SCANCODE_S) == j1KeyState::KEY_REPEAT)
				player_position.y += godModeSpeed * dt;
		}
		else {

			if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_IDLE && App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_IDLE && attacking == false) {
				animation = &idle;
			}

			// Direction controls	
			if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_REPEAT && attacking == false) {
				if (wallInFront == false && dead == false) {
					player_position.x += speed.x;
					animation = &run;
				}
				else if (dead == true) {
					facingRight = true;
					animation = &idle;
				}
				else {
					animation = &idle;
				}
			}

			if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_REPEAT && attacking == false) {
				if (wallBehind == false && dead == false) {
					player_position.x -= speed.x;
					animation = &run;
				}
				else if (dead == true) {
					facingRight = false;
					animation = &idle;
				}
				else
					animation = &idle;
			}

			if (App->input->GetKey(SDL_SCANCODE_W) == j1KeyState::KEY_REPEAT && attacking == false) {
				if (wallInFront == false && dead == false) {
					player_position.y -= speed.y;
					animation = &run;
				}
				else if (dead == true) {
					facingRight = true;
					animation = &idle;
				}
				else {
					animation = &idle;
				}
			}

			if (App->input->GetKey(SDL_SCANCODE_S) == j1KeyState::KEY_REPEAT && attacking == false) {
				if (OnGround == false && dead == false) {
					player_position.y += speed.y;
					animation = &run;
				}
				else if (dead == true) {
					facingRight = false;
					animation = &idle;
				}
				else
					animation = &idle;
			}

			if (OnGround == false && jumping == false) {

				freefall = true;

				if (!attacking)
					animation = &fall;
			}

			if (App->input->GetKey(SDL_SCANCODE_SPACE) == j1KeyState::KEY_DOWN) {
				if ((currentJumps == initialJumps && freefall == true) || (currentJumps < maxJumps && freefall == false)) {
					jumping = true;
					verticalSpeed = initialVerticalSpeed;
					currentJumps++;

					if (freefall == true || (currentJumps > 1 && freefall == false))
						App->audio->PlayFx(jumpSound);
				}
			}

			// Reseting the jump every frame
			OnGround = false;
			if (dead && deathByFall == false)
				animation = &death;

			if (jumping == true && animation != &death) {
				//If the player touches a wall collider
				if (OnGround) {

					animation = &idle;
					jumping = false;
				}
				else {

					// While the player is falling
					if (!attacking) {
						if (verticalSpeed <= 0) {
							animation = &jump;
						}
						else if (verticalSpeed > 0) {
							animation = &fall;
						}
					}
				}
			}
		}

		// God mode
		if (App->input->GetKey(SDL_SCANCODE_F10) == j1KeyState::KEY_DOWN && dead == false)
		{
			GodMode = !GodMode;

			if (GodMode == true)
			{
				collider->type = COLLIDER_NONE;
				animation = &godmode;

			}
			else if (GodMode == false)
			{
				collider->type = COLLIDER_PLAYER;
			}
		}
		if (dead && App->fade->IsFading() == false)
			lives--;

		if (dead) {
			// Death animation is not shown if the player dies by falling
			if (!deathByFall)
				animation = &death;

			// Restarting the level in case the player dies
			if (App->fade->IsFading() == 0)
			{
				player_position.x = initialPosition.x;
				player_position.y = initialPosition.y;
				fallingSpeed = initialFallingSpeed;
				jumping = false;
				facingRight = true;
				deathByFall = false;
				playedSound = false;

				// Resetting the animation
				death.Reset();
				animation = &idle;

				dead = false;
			}
		}

		// Update collider position to player position

		if (collider != nullptr)
			collider->SetPos(player_position.x + margin.x, player_position.y + margin.y);

		// Blitting the player
		SDL_Rect r = animation->GetCurrentFrame(dt);

		if (facingRight) {
			Draw(r, false, player_position.x, player_position.y);
		}
		else {
			Draw(r, true, player_position.x, player_position.y);
		}

		return true;
	}
}


// Call modules after each loop iteration
bool j1Player::PostUpdate() {

	loading = false;

	// Resetting the jump if touched the "ceiling"
	wallAbove = false;

	// Resetting the movement
	wallInFront = false;
	wallBehind = false;

	return true;
}

// Load game state
bool j1Player::Load(pugi::xml_node& data) {

	player_position.x = data.child("player").child("position").attribute("x").as_int();
	player_position.y = data.child("player").child("position").attribute("y").as_int();

	GodMode = data.child("player").child("godmode").attribute("value").as_bool();

	lives = data.child("player").child("lives").attribute("value").as_uint();

	loading = true;

	if (GodMode == true)
	{
		collider->type = COLLIDER_NONE;
		animation = &godmode;
	}
	else if (GodMode == false)
	{
		collider->type = COLLIDER_PLAYER;
	}


	return true;
}

// Save game state
bool j1Player::Save(pugi::xml_node& data) const {

	pugi::xml_node pos = data.append_child("position");

	pos.append_attribute("x") = player_position.x;
	pos.append_attribute("y") = player_position.y;

	pugi::xml_node godmode = data.append_child("godmode");

	pugi::xml_node life = data.append_child("lives");
	life.append_attribute("value") = lives;

	godmode.append_attribute("value") = GodMode;



	return true;
}

// Called before quitting
bool j1Player::CleanUp() {

	// Remove all memory leaks
	LOG("Unloading the player");
	App->tex->UnLoad(sprites);

	if (collider != nullptr)
		collider->to_delete = true;

	return true;
}

// Detects Collisions

void j1Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == COLLIDER_PLAYER)
	{
		if (c2->type == COLLIDER_WALL)
		{
			// Right & Left Collisions
			if (c1->rect.y <= c2->rect.y + c2->rect.h && c1->rect.y + c1->rect.h - 5 >= c2->rect.y)
			{
				// right
				if (c1->rect.x + c1->rect.w >= c2->rect.x && c1->rect.x <= c2->rect.x)
				{
					ColRight = true;
					ColLeft = false;
				}
				// left
				else if (c1->rect.x <= c2->rect.x + c2->rect.w && c1->rect.x + c1->rect.w >= c2->rect.x + c2->rect.w)
				{
					ColLeft = true;
					ColRight = false;
				}
			}

			// Up & Down Collisions
			if (c1->rect.x + c1->rect.w >= c2->rect.x + 4 && c1->rect.x + 4 <= c2->rect.x + c2->rect.w)
			{
				// down
				if (c1->rect.y + c1->rect.h >= c2->rect.y && c1->rect.y < c2->rect.y) {

					OnGround = true;
					jumping = false;

					player_position.y = c2->rect.y - c1->rect.h + 1;

					speed.y = 0;
					doubleJump = 2;

					ColDown = true;
					ColUp = false;
					playerCanMove = true;
				}
				// up
				else if (c1->rect.y <= c2->rect.y + c2->rect.h && c1->rect.y > c2->rect.y) {

					OnGround = false;

					player_position.y = c2->rect.y + c2->rect.h;

					ColDown = false;
					ColUp = true;

					LOG("TOUCHING UP");
				}
			}
		}

		//Death

		/*if (c2->type == COLLIDER_DEATH) {
			if (!playedFx) {
				App->audio->PlayFx(App->audio->deathFx);
				playedFx = true;
			}

			animation = &deathAnim;
			isDead = true;
		}*/

		//Win

		/*if (c2->type == COLLIDER_WIN) {
			touchingWin = true;
			playerCanMove = false;
			c2->to_delete = true;
		}*/
	}
}

void j1Player::LoadPlayerProperties() {

	pugi::xml_document config_file;
	config_file.load_file("config.xml");

	pugi::xml_node config;
	config = config_file.child("config");

	pugi::xml_node player;
	player = config.child("player");

	// Copying the size of the player
	playerSize.x = player.child("size").attribute("width").as_int();
	playerSize.y = player.child("size").attribute("height").as_int();
	margin.x = player.child("margin").attribute("x").as_int();
	margin.y = player.child("margin").attribute("y").as_int();

	// Copying values of the speed
	pugi::xml_node speed = player.child("speed");

	//Speed

	//Jump

	cameraLimit = config.child("scene1").child("camera").attribute("cameraLimit").as_int();

	deathByFallColliderHeight = player.child("deathByFallCollider").attribute("h").as_uint();
}

