#pragma once
#include "Common/TileType.h"
#include "Math/Vector2.h"

class IMapPathfinder
{
public:
	
	virtual ~IMapPathfinder(){}
	virtual TileType GetTileAt(int x, int y) const = 0;
	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;
	virtual bool IsWalkable(int x, int y)const = 0;

};