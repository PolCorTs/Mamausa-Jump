#include "j1EntityManager.h"
#include "j1Player.h"
//
//j1EntityManager::j1EntityManager()
//{
//}
//
//j1EntityManager::~j1EntityManager()
//{
//}
//
//bool j1EntityManager::Awake(pugi::xml_node &)
//{
//	return true;
//}
//
//bool j1EntityManager::Start()
//{
//	for (p2List_item<j1Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next)
//	{
//		iterator->data->Start();
//	}
//
//	return true;
//}
//
//bool j1EntityManager::PreUpdate()
//{
//		for (uint i = 0; i < MAX_ENTITIES; ++i)
//		{
//			if (queue[i].type != ENTITY_TYPES::UNKNOWN)
//			{
//				//SpawnEnemy(queue[i]);
//				queue[i].type = ENTITY_TYPES::UNKNOWN;
//			}
//		}
//
//	return true;
//}
//
//bool j1EntityManager::Update(float dt)
//{
//	
//	for (p2List_item<j1Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next)
//	{
//		iterator->data->Update(dt);
//	}
//
//	return true;
//}
//
//bool j1EntityManager::PostUpdate()
//{
//
//		for (p2List_item<j1Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next)
//		{
//			iterator->data->PostUpdate();
//		}
//
//	return true;
//}
//
//bool j1EntityManager::CleanUp()
//{
//
//	for (p2List_item<j1Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next)
//	{
//		iterator->data->CleanUp();
//	}
//
//	entities.clear();
//
//	player = nullptr;
//
//	return true;
//}
//
//j1Entity* j1EntityManager::CreateEntity(ENTITY_TYPES type, int x, int y)
//{
//	j1Entity* ret = nullptr;
//	switch (type)
//	{
//	case PLAYER:
//		ret = new j1Player(x, y, type);
//		if (ret != nullptr) entities.add(ret); 
//		break;
//
//	return ret;
//}
//
//void j1EntityManager::AddEnemy(int x, int y, ENTITY_TYPES type)
//{
//	for (int i = 0; i < MAX_ENTITIES; ++i)
//	{
//		if (queue[i].type == ENTITY_TYPES::UNKNOWN)
//		{
//				queue[i].type = type;
//				queue[i].position.x = x;
//				queue[i].position.y = y;
//				break;
//		}
//	}
//}
//void j1EntityManager::CreatePlayer()
//{
//	player = (j1Player*)CreateEntity(PLAYER);
//}
//
