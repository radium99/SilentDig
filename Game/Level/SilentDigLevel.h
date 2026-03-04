#pragma once
#include "Level/Level.h"
#include "Interface/ICanPlayerMove.h"
#include <vector>

namespace Wanted { class Actor; }

enum class TileType { Wall, Ground, Empty };

class SilentDigLevel : public Wanted::Level, public ICanPlayerMove
{
	RTTI_DECLARATIONS(SilentDigLevel, Level)

public:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	// Inherited via ICanPlayerMove
	virtual bool CanMove(
		const Wanted::Vector2& playerPosition,
		const Wanted::Vector2& nextPosition) override;

private:
	// BSP    .
	void CreateWorld();

	Wanted::Actor* player = nullptr;

	//   ũ
	int mapWidth = 40;
	int mapHeight = 40;

	//   ⹰ ˻.
	std::vector<std::vector<TileType>> map;
};