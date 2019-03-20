#pragma once

// Project Includes
# include "Framework/SpriteObject.h"

// Forward Declaration
// (Only works with pointers or references)
class Level;

class GridObject : public SpriteObject
{
public:
	GridObject();

	void SetGridPosition(sf::Vector2i _newPosition);
	void SetGridPosition(int _x, int _y);

	void SetLevel(Level* _newLevel);

	sf::Vector2i GetGridPosition();
	bool GetBlocksMovement();

	// Overriding Methods
	void Update(sf::Time _frameTime);

protected:
	bool m_blocksMovement;
	sf::Vector2i m_gridPosition;
	Level* m_level;

	// Moving Objects
	sf::Vector2f m_velocity; // pixels per second
	sf::Vector2f m_previousPosition;
};