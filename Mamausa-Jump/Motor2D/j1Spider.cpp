#include "j1spider.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1FadeToBlack.h"
#include "j1Player.h"
#include "j1Map.h"
#include "j1Scene.h"


#define GRAVITY 1

j1spider::j1spider(int x, int y, ENTITY_TYPES type) : j1Entity(x, y, ENTITY_TYPES::SPIDER)
{
	animation = NULL;

	idle.LoadEnemyAnimations("idle", "spider");
	move.LoadEnemyAnimations("move", "spider");

	// Setting spider position
	initialPosition.x = position.x = x;
	initialPosition.y = position.y = y;
}

j1spider::~j1spider() {}

bool j1spider::Start()
{
	// Textures are loaded
	LOG("Loading spider textures");
	sprites = App->tex->Load("textures/enemies/spider/zombie&spider.png");

	LoadspiderProperties();

	animation = &idle;

	collider = App->collisions->AddCollider({ (int)position.x - margin.x, (int)position.y - margin.y, colliderSize.x, colliderSize.y }, COLLIDER_ENEMY, App->entity);

	return true;
}

bool j1spider::Update(float dt, bool do_logic)
{

		if (dead == false) {
			collider->SetPos(position.x, position.y);

			position.y += GRAVITY + GRAVITY * dt;

			if (do_logic || path_created) {
				if ((App->entity->player->position.x - position.x) <= DETECTION_RANGE && (App->entity->player->position.x - position.x) >= -DETECTION_RANGE && App->entity->player->collider->type == COLLIDER_PLAYER && !dead)
				{
					iPoint origin = { App->map->WorldToMap((int)position.x + colliderSize.x / 2, (int)position.y + colliderSize.y / 2) };
					iPoint destination;
					if (position.x < App->entity->player->position.x)
						destination = { App->map->WorldToMap((int)App->entity->player->position.x + App->entity->player->playerSize.x + 1, (int)App->entity->player->position.y + App->entity->player->playerSize.y / 2) };
					else
						destination = { App->map->WorldToMap((int)App->entity->player->position.x, (int)App->entity->player->position.y + App->entity->player->playerSize.y / 2) };

					if (App->path->IsWalkable(destination) && App->path->IsWalkable(origin))
					{
						path = App->path->CreatePath(origin, destination);
						Move(*path, dt);
						path_created = true;
					}
				}
				else if (path_created) {
					path->Clear();
					path_created = false;
				}
			}

			if (App->entity->player->position == App->entity->player->initialPosition)
			{
				animation = &idle;
				position = initialPosition;
			}

			// Blitting the spider
			SDL_Rect r = animation->GetCurrentFrame(dt);

			if (position.x - App->entity->player->position.x >= 0)
				Draw(r, false, 0, 0);
			else
				Draw(r, true, 0, 0);
		}

	return true;
}

bool j1spider::CleanUp()
{
	LOG("Unloading spider");
	App->tex->UnLoad(sprites);
	if (collider != nullptr)
		collider->to_delete = true;

	if (path_created) {
		path->Clear();
		path_created = false;
	}

	return true;
}

void j1spider::OnCollision(Collider * col_1, Collider * col_2)
{
	COLLISION_DIRECTION direction;

	if (col_2->type == COLLIDER_WALL)
	{
		direction = col_1->CheckDirection(col_2->rect);

		if (direction == UP_COLLISION)
			position.y = col_2->rect.y - colliderSize.y + 1;

		else if (direction == DOWN_COLLISION)
			position.y = col_2->rect.y + col_2->rect.h;

		else if (direction == RIGHT_COLLISION)
			position.x = col_2->rect.x + col_2->rect.w;

		else if (direction == LEFT_COLLISION)
			position.x = col_2->rect.x - colliderSize.x;
	}

	if (col_2->type == COLLIDER_PLAYER || col_2->type == COLLIDER_DEATH) {
		dead = true;
		collider->to_delete = true;
		int num = App->entity->entities.find(this);
		RELEASE(App->entity->entities.At(num)->data);
		App->entity->entities.del(App->entity->entities.At(num));
	}

}

bool j1spider::Load(pugi::xml_node& data)
{
	return true;
}

bool j1spider::Save(pugi::xml_node& data) const
{
	pugi::xml_node pos = data.append_child("position");

	pos.append_attribute("x") = position.x;
	pos.append_attribute("y") = position.y;

	return true;
}

void j1spider::LoadspiderProperties()
{
	pugi::xml_document config_file;
	config_file.load_file("config.xml");
	pugi::xml_node config;
	config = config_file.child("config");
	pugi::xml_node spider;
	spider = config.child("spider");

	speed = spider.attribute("speed").as_int();

	// Copying the values of the collider
	margin.x = spider.child("margin").attribute("x").as_int();
	margin.y = spider.child("margin").attribute("y").as_int();
	colliderSize.x = spider.child("colliderSize").attribute("w").as_int();
	colliderSize.y = spider.child("colliderSize").attribute("h").as_int();
}

void j1spider::Move(p2DynArray<iPoint>& path, float dt)
{
	direction = App->path->CheckDirectionGround(path);

	if (direction == Movement::DOWN)
	{
		animation = &move;
		position.y += speed * dt;
	}

	else if (direction == Movement::UP)
	{
		animation = &move;
		position.y -= speed * dt;
	}

	else if (direction == Movement::RIGHT)
	{
		animation = &move;
		position.x += speed * dt;
	}

	else if (direction == Movement::LEFT)
	{
		animation = &move;
		position.x -= speed * dt;
	}
}
