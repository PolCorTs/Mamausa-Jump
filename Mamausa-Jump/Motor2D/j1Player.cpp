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

bool j1Player::Start()
{
	LOG("Loading player textures");
	sprites = App->tex->Load("textures/AlienBlue.png");

	LoadPlayerProperties();

	collider = App->collision->AddCollider({ (int)player_position.x, (int)player_position.y, playerSize.x, playerSize.y }, COLLIDER_PLAYER, App->entity);

	player_start = true;

	fallingSpeed = 0.0f;
	initialFallingSpeed = 0.0f;
	gravity = 0.01f; //0.02
	verticalSpeed = 0.0f;
	initialVerticalSpeed = -2.0f; //-2.5f
	maxJumps = 2;

	godModeSpeed = 1; //2
	speed = 0.8; //1.3

	return true;
}

bool j1Player::PreUpdate()
{
	return true;
}

bool j1Player::Update(float dt)
{
	if (player_start)
	{
		if (godMode)
		{
			animation = &godmode;

			if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_REPEAT)
			{
				player_position.x += godModeSpeed;
			}

			if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_REPEAT)
			{
				player_position.x -= godModeSpeed;
			}

			if (App->input->GetKey(SDL_SCANCODE_W) == j1KeyState::KEY_REPEAT)
			{
				player_position.y -= godModeSpeed;
			}

			if (App->input->GetKey(SDL_SCANCODE_S) == j1KeyState::KEY_REPEAT)
			{
				player_position.y += godModeSpeed;
			}
		}
		else {

			if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_IDLE && App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_IDLE)
			{
				animation = &idle;
			}

			if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_REPEAT)
			{
				facingRight = true;

				if (wallInFront == false)
				{
					player_position.x += speed;
					animation = &run;
				}
				else
				{
					animation = &idle;
				}
			}

			if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_REPEAT)
			{
				facingRight = false;

				if (wallBehind == false)
				{
					player_position.x -= speed;
					animation = &run;
				}
				else
				{
					animation = &idle;
				}
			}

			if (onGround == false && jumping == false)
			{
				freefall = true;
			}

			if (freefall == true)
			{
				player_position.y += fallingSpeed;
				fallingSpeed += gravity;
				animation = &fall;
				canDoubleJump = false;
			}

			if (onGround)
			{
				jumping = false;
				doubleJump = false;
				canDoubleJump = true;

				fallingSpeed = initialFallingSpeed;
				verticalSpeed = initialVerticalSpeed;
			}

			if (App->input->GetKey(SDL_SCANCODE_SPACE) == j1KeyState::KEY_DOWN && onGround)
			{
				freefall = false;
				jumping = true;

				LOG("Jump");
			}

			else if (App->input->GetKey(SDL_SCANCODE_SPACE) == j1KeyState::KEY_DOWN && !onGround && canDoubleJump)
			{
				verticalSpeed = initialVerticalSpeed;
				canDoubleJump = false;
				doubleJump = true;

				LOG("DoubleJump");
			}

			if (jumping == true || doubleJump == true)
			{
				player_position.y += verticalSpeed;
				verticalSpeed += gravity;
			}
		}

		// God mode
		if (App->input->GetKey(SDL_SCANCODE_F10) == j1KeyState::KEY_DOWN)
		{
			godMode = !godMode;

			if (godMode == true)
			{
				collider->type = COLLIDER_NONE;
				animation = &godmode;

			}
			else if (godMode == false)
			{
				collider->type = COLLIDER_PLAYER;
			}
		}

		// Update collider position to player position

		if (collider != nullptr)
		{
			collider->SetPos(player_position.x + margin.x, player_position.y + margin.y);
		}

		// Blitting the player

<<<<<<< HEAD
		Draw(animation->GetCurrentFrame(dt), facingRight, position.x, position.y);
=======
		if (facingRight) 
		{
			Draw(r, false, player_position.x, player_position.y);
		}
		
		else if (!facingRight)
		{
			Draw(r, true, player_position.x, player_position.y);
		}
>>>>>>> parent of 63703db... Started dt (not working yet)
	}

	//Camera Update

	UpdateCameraPosition();

	return true;
}

// Call modules after each loop iteration
bool j1Player::PostUpdate()
{
	loading = false;
	onGround = false;

	wallAbove = false;
	wallInFront = false;
	wallBehind = false;

	return true;
}

// Load game state
bool j1Player::Load(pugi::xml_node& data)
{
	player_position.x = data.child("player").child("position").attribute("x").as_int();
	player_position.y = data.child("player").child("position").attribute("y").as_int();

	godMode = data.child("player").child("godmode").attribute("value").as_bool();

	lives = data.child("player").child("lives").attribute("value").as_uint();

	loading = true;

	if (godMode == true)
	{
		collider->type = COLLIDER_NONE;
		animation = &godmode;
	}

	else if (godMode == false)
	{
		collider->type = COLLIDER_PLAYER;
	}

	return true;
}

// Save game state
bool j1Player::Save(pugi::xml_node& data) const
{
	pugi::xml_node pos = data.append_child("position");

	pos.append_attribute("x") = player_position.x;
	pos.append_attribute("y") = player_position.y;

	pugi::xml_node godmode = data.append_child("godmode");
	godmode.append_attribute("value") = godMode;

	pugi::xml_node life = data.append_child("lives");
	life.append_attribute("value") = lives;

	return true;
}

// Called before quitting
bool j1Player::CleanUp()
{
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
			if (c1->rect.y <= c2->rect.y + c2->rect.h && c1->rect.y + c1->rect.h - 2 >= c2->rect.y)
			{
				// right
				if (c1->rect.x + c1->rect.w >= c2->rect.x && c1->rect.x <= c2->rect.x)
				{
					wallInFront = true;
					wallBehind = false;
				}
				// left
				else if (c1->rect.x <= c2->rect.x + c2->rect.w && c1->rect.x + c1->rect.w >= c2->rect.x + c2->rect.w)
				{
					wallInFront = false;
					wallBehind = true;
				}
			}
			// Up & Down Collisions
			if (c1->rect.x + c1->rect.w >= c2->rect.x + 2 && c1->rect.x + 2 <= c2->rect.x + c2->rect.w)
			{
				// down
				if (c1->rect.y + c1->rect.h >= c2->rect.y && c1->rect.y < c2->rect.y)
				{
					onGround = true;
					freefall = false;

					LOG("TOUCHING DOWN");
				}// up
				else if (c1->rect.y <= c2->rect.y + c2->rect.h && c1->rect.y > c2->rect.y)
				{
					onGround = false;

					verticalSpeed = 0.1f;

					LOG("TOUCHING UP");
				}
			}
		}

		//Death

		if (c2->type == COLLIDER_DEATH)
		{
			/*if (!playedFx) {
				App->audio->PlayFx(App->audio->deathFx);
				playedFx = true;
			}

			animation = &deathAnim;
			isDead = true;*/
		}

		//Win

		if (c2->type == COLLIDER_END)
		{
			/*touchingWin = true;
			playerCanMove = false;
			c2->to_delete = true;*/
		}
	}
}

void j1Player::UpdateCameraPosition() {
<<<<<<< HEAD
<<<<<<< HEAD
	if (position.x > 400)
=======
	if (player_position.x > 400) 
>>>>>>> parent of 63703db... Started dt (not working yet)
=======
	if (player_position.x > 400) 
>>>>>>> parent of 63703db... Started dt (not working yet)
	{
		App->render->camera.x = -player_position.x + 400;
	}
	/*
<<<<<<< HEAD
<<<<<<< HEAD
	if (position.y > 700 && App->render->camera.y < -400)
	{
		App->render->camera.y = -position.y + 350;
=======
	if (player_position.y > 700 && App->render->camera.y < -400) 
	{
		App->render->camera.y = -player_position.y + 350;
>>>>>>> parent of 63703db... Started dt (not working yet)
=======
	if (player_position.y > 700 && App->render->camera.y < -400) 
	{
		App->render->camera.y = -player_position.y + 350;
>>>>>>> parent of 63703db... Started dt (not working yet)
	}
	*/
}

void j1Player::LoadPlayerProperties()
{
	pugi::xml_document config_file;
	config_file.load_file("config.xml");

	pugi::xml_node config;
	config = config_file.child("config");

	pugi::xml_node player;
	player = config.child("player");

	//Player
	player_position.x = player.child("initialPlayerPosition").attribute("x").as_int();
	player_position.y = player.child("initialPlayerPosition").attribute("y").as_int();
	playerSize.x = player.child("size").attribute("width").as_int();
	playerSize.y = player.child("size").attribute("height").as_int();
	margin.x = player.child("margin").attribute("x").as_int();
	margin.y = player.child("margin").attribute("y").as_int();

	//Speed

	pugi::xml_node speed = player.child("speed");


	deathByFallColliderHeight = player.child("deathByFallCollider").attribute("h").as_uint();
}