#include "j1EntityManager.h"
#include "j1Player.h"
#include "j1Bat.h"
#include "j1Spider.h"

#include "Brofiler/Brofiler.h"

j1EntityManager::j1EntityManager()
{
}

j1EntityManager::~j1EntityManager()
{
}

bool j1EntityManager::Awake(pugi::xml_node &)
{
	return true;
}

bool j1EntityManager::Start()
{
	for (p2List_item<j1Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next)
	{
		iterator->data->Start();
	}

	return true;
}

bool j1EntityManager::PreUpdate()
{
	BROFILER_CATEGORY("EntityManagerPreUpdate", Profiler::Color::Orange)
	
	for (uint i = 0; i < MAX_ENTITIES; ++i)
	{
		if (queue[i].type != ENTITY_TYPES::UNKNOWN)
		{
			//SpawnEnemy(queue[i]);
			queue[i].type = ENTITY_TYPES::UNKNOWN;
		}
	}

	return true;
}

bool j1EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("EntityManagerUpdate", Profiler::Color::LightSeaGreen)

	for (p2List_item<j1Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next)
	{
		iterator->data->Update(dt);
	}

	return true;
}

bool j1EntityManager::PostUpdate()
{
	BROFILER_CATEGORY("EntityManagerPostUpdate", Profiler::Color::Yellow)

	for (p2List_item<j1Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next)
	{
		iterator->data->PostUpdate();
	}

	return true;
}

bool j1EntityManager::CleanUp()
{

	for (p2List_item<j1Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next)
	{
		iterator->data->CleanUp();
	}

	entities.clear();

	player = nullptr;

	return true;
}

j1Entity* j1EntityManager::CreateEntity(ENTITY_TYPES type, int x, int y)
{
	j1Entity* ret = nullptr;
	switch (type)
	{
	case PLAYER:
		ret = new j1Player(x, y, type);
		if (ret != nullptr) entities.add(ret);
		break;

		return ret;
	}
}

void j1EntityManager::AddEnemy(int x, int y, ENTITY_TYPES type)
{
	for (int i = 0; i < MAX_ENTITIES; ++i)
	{
		if (queue[i].type == ENTITY_TYPES::UNKNOWN)
		{
			queue[i].type = type;
			queue[i].position.x = x;
			queue[i].position.y = y;
			break;
		}
	}
}

void j1EntityManager::OnCollision(Collider* c1, Collider* c2)
{
	for (p2List_item<j1Entity*>* it = entities.start; it != nullptr; it = it->next)
	{
		if (it->data->collider == c1)
		{
			it->data->OnCollision(c1, c2);
			it->data->OnCollision(c2, c1);
			break;
		}
	}
}

void j1EntityManager::SpawnEnemy(const EntityInfo& info)
{
	for (uint i = 0; i < MAX_ENTITIES; ++i)
	{
		if (queue[i].type != ENTITY_TYPES::UNKNOWN)
		{
			j1Entity* entity;
			if (queue[i].type == BAT)
				entity = new j1Bat(info.position.x, info.position.y, info.type);

			else if (queue[i].type == Spider)
				entity = new j1Spider(info.position.x, info.position.y, info.type);

			entities.add(entity);
			entity->Start();
			break;
		}
	}
}

void j1EntityManager::DestroyEntities()
{
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		queue[i].type = ENTITY_TYPES::UNKNOWN;
	}

	for (p2List_item<j1Entity*>* iterator = entities.start; iterator; iterator = iterator->next) {
		if (iterator->data->type != ENTITY_TYPES::PLAYER)
		{
			iterator->data->CleanUp();
			int num = entities.find(iterator->data);
			RELEASE(entities.At(num)->data);
			entities.del(entities.At(num));
		}
	}
}

void j1EntityManager::CreatePlayer()
{
	player = (j1Player*)CreateEntity(PLAYER,0,0);
}

bool j1EntityManager::Load(pugi::xml_node&)
{
	return true;
}

bool j1EntityManager::Save(pugi::xml_node&) const
{
	return true;
}

