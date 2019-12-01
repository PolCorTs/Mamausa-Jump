#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"

j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name.create("pathfinding");
}

// Destructor
j1PathFinding::~j1PathFinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.Clear();
	RELEASE_ARRAY(map);
	return true;
}

// Sets up the walkability map
void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

// Utility: return true if pos is inside the map boundaries
bool j1PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool j1PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

// Utility: return the walkability value of a tile
uchar j1PathFinding::GetTileAt(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

Movement j1PathFinding::CheckDirection(p2DynArray<iPoint>& path) const
{
	if (path.Count() >= 2)
	{
		iPoint tile = path[0];
		iPoint next_tile = path[1];

		int x_difference = next_tile.x - tile.x;
		int y_difference = next_tile.y - tile.y;

		if (x_difference == 1 && y_difference == 1) return DOWN_RIGHT;
		else if (x_difference == 1 && y_difference == -1) return UP_RIGHT;
		else if (x_difference == -1 && y_difference == 1) return DOWN_LEFT;
		else if (x_difference == -1 && y_difference == -1) return UP_LEFT;
		else if (x_difference == 1) return RIGHT;
		else if (x_difference == -1) return LEFT;
		else if (y_difference == 1)	return DOWN;
		else if (y_difference == -1) return UP;
	}

	else return NONE;
}

Movement j1PathFinding::CheckDirectionGround(p2DynArray<iPoint>& path) const
{
	if (path.Count() >= 2)
	{
		iPoint tile = path[0];
		iPoint next_tile = path[1];

		int x_difference = next_tile.x - tile.x;
		int y_difference = next_tile.y - tile.y;

		if (x_difference == 1) return RIGHT;
		else if (x_difference == -1) return LEFT;
		else if (y_difference == 1)	return DOWN;
		else if (y_difference == -1) return UP;
	}

	else return NONE;
}

// To request all tiles involved in the last generated path
const p2DynArray<iPoint>* j1PathFinding::GetLastPath() const
{
	return &last_path;
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
p2List_item<PathNode>* PathList::Find(const iPoint& point) const
{
	p2List_item<PathNode>* item = list.start;
	while (item)
	{
		if (item->data.pos == point)
			return item;
		item = item->next;
	}
	return NULL;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
p2List_item<PathNode>* PathList::GetNodeLowestScore() const
{
	p2List_item<PathNode>* ret = NULL;
	int min = 65535;

	p2List_item<PathNode>* item = list.end;
	while (item)
	{
		if (item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& list_to_fill) const
{
	iPoint cell;
	uint before = list_to_fill.list.count();


	//north-east
	cell.create(pos.x + 1, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	//north-west
	cell.create(pos.x - 1, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// south-east
	cell.create(pos.x + 1, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// south-west
	cell.create(pos.x - 1, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// north
	cell.create(pos.x, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// south
	cell.create(pos.x, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x + 1, pos.y);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));



	return list_to_fill.list.count();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination)
{
	g = parent->g + 1;

	int x_distance = abs(pos.x - destination.x);
	int y_distance = abs(pos.y - destination.y);

	h = (x_distance + y_distance) * min(x_distance, y_distance);

	return g + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
p2DynArray<iPoint>* j1PathFinding::CreatePath(iPoint& origin, iPoint& destination)
{
	BROFILER_CATEGORY("CreatePath", Profiler::Color::SlateGray)

		last_path.Clear();
	// If origin or destination are not walkable, return -1
	if (IsWalkable(origin) && IsWalkable(destination)) {

		// We create two lists: open, close, and we add the origin tile to open, and iterate while we have tile in the open list
		PathList open, close;
		PathNode origin(0, origin.DistanceNoSqrt(destination), origin, nullptr);
		open.list.add(origin);

		while (open.list.count() > 0)
		{
			// We move the lowest score cell from open list to the closed list
			close.list.add(open.GetNodeLowestScore()->data);
			open.list.del(open.GetNodeLowestScore());

			if (close.list.end->data.pos != destination)
			{
				// We fill a list of all adjancent nodes
				PathList adjancent;

				// We iterate adjancent nodes:
				close.list.end->data.FindWalkableAdjacents(adjancent);

				for (p2List_item<PathNode>* iterator = adjancent.list.start; iterator != nullptr; iterator = iterator->next)
				{
					// Ignore nodes in the closed list
					if (close.Find(iterator->data.pos))
						continue;

					// If it is already in the open list, check if it is a better path (compare G)
					else if (open.Find(iterator->data.pos))
					{
						PathNode tmp = open.Find(iterator->data.pos)->data;
						iterator->data.CalculateF(destination);
						if (tmp.g > iterator->data.g)
						{
							// If it is a better path, Update the parent
							tmp.parent = iterator->data.parent;
						}
					}
					// If it is NOT found, calculate its F and add it to the open list
					else
					{
						iterator->data.CalculateF(destination);
						open.list.add(iterator->data);
					}
				}
				adjancent.list.clear();
			}
			else
			{
				// If we just added the destination, we are done!
				for (p2List_item<PathNode>* iterator = close.list.end; iterator->data.parent != nullptr; iterator = close.Find(iterator->data.parent->pos))
				{
					// Backtrack to create the final path
					last_path.PushBack(iterator->data.pos);
					if (iterator->data.parent == nullptr)
						last_path.PushBack(close.list.start->data.pos);
				}

				// We use the Pathnode::parent and Flip() the path when you are finish
				last_path.Flip();
				return &last_path;
			}
		}
	}

	return nullptr;
}

