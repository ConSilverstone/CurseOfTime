// Project Includes
#include "GridObject.h"
#include "Level.h"

GridObject::GridObject()
	: SpriteObject()
	, m_gridPosition(0, 0)
	, m_level(nullptr)
	, m_blocksMovement(false)
{
}

void GridObject::SetGridPosition(sf::Vector2i _newPosition)
{
	m_gridPosition = _newPosition;
	m_sprite.setPosition(
		m_gridPosition.x * m_level->GetCellSize(),
		m_gridPosition.y * m_level->GetCellSize());
}

void GridObject::SetGridPosition(int _x, int _y)
{
	SetGridPosition(sf::Vector2i(_x, _y));
}

void GridObject::SetLevel(Level * _newLevel)
{
	m_level = _newLevel;
}

sf::Vector2i GridObject::GetGridPosition()
{
	return m_gridPosition;
}

bool GridObject::GetBlocksMovement()
{
	return m_blocksMovement;
}

void GridObject::Update(sf::Time _frameTime)
{
	// Get the current position
	sf::Vector2f currentPosition = m_sprite.getPosition();

	// Record our current position as the new previous position
	m_previousPosition = currentPosition;

	// Calculate the amount moved
	sf::Vector2f amountMoved = m_velocity * _frameTime.asSeconds();

	// Calculate new position
	sf::Vector2f newPosition = currentPosition + amountMoved;

	// Set our sprite to this new position
	m_sprite.setPosition(newPosition);
}
