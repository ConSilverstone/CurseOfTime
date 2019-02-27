// Project Includes
#include "Diamond.h"
#include "Level.h"
#include "Framework/AssetManager.h"
#include "Player.h"

Diamond::Diamond()
	: GridObject()
	, m_currentTime(0.0f)
	, m_timeFallen(0.3f)
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/Coin.png"));
	m_blocksMovement = true;
}

void Diamond::Update(sf::Time _frameTime)
{
	if (canItFall(sf::Vector2i(0, 1)) == true)
	{
		m_currentTime += _frameTime.asSeconds();
	}
	else
	{
		m_currentTime = 0;
	}

	if (m_currentTime > m_timeFallen)
	{
		// Attempt to move the boulder
		attemptFall(sf::Vector2i(0, 1));

		// Reset the timer
		m_currentTime = 0;
	}
}

bool Diamond::canItFall(sf::Vector2i _direction)
{
	// Attempt to move in the given direction

	// Get current position
	// Calculate target position
	sf::Vector2i targetPos = m_gridPosition + _direction;

	// Check if the space is empty

	// Get list of objects in our target position
	std::vector<GridObject*> targetCellContents = m_level->GetObjectAt(targetPos);

	// Check if any of those objects block movement
	bool blocked = false;
	GridObject* blocker = nullptr;
	for (int i = 0; i < targetCellContents.size(); ++i)
	{
		if (targetCellContents[i]->GetBlocksMovement() == true)
		{
			blocked = true;
			blocker = targetCellContents[i];
		}
	}

	// If empty, move there
	if (!blocked)
	{
		return true;
	}
	else
	{
		Player* killPlayer = dynamic_cast<Player*>(blocker);
		if (killPlayer != nullptr)
		{
			return true;
		}
		else {
			return false;
		}
	}
}
bool Diamond::attemptFall(sf::Vector2i _direction)
{
	// Attempt to move in the given direction

	// Get current position
	// Calculate target position
	sf::Vector2i targetPos = m_gridPosition + _direction;

	// Check if the space is empty

	// Get list of objects in our target position
	std::vector<GridObject*> targetCellContents = m_level->GetObjectAt(targetPos);

	// Check if any of those objects block movement
	bool blocked = false;
	GridObject* blocker = nullptr;
	for (int i = 0; i < targetCellContents.size(); ++i)
	{
		if (targetCellContents[i]->GetBlocksMovement() == true)
		{
			blocked = true;
			blocker = targetCellContents[i];
		}
	}

	// If empty, move there
	if (!blocked)
	{
		return m_level->MoveObjectTo(this, targetPos);
	}
	else
	{
		// We were blocked!
		// Can we push the thing blocking us?
		// Do a dynamic cast to a diamond to see if it can fall
		Diamond* diamond = dynamic_cast<Diamond*>(blocker);
		// Do a dymanic cast to see of the diamond hit a player
		Player* player = dynamic_cast<Player*>(blocker);
		// If so (the blocker is a boulder (not nullptr))
		
		if (diamond != nullptr)
		{

		}


		// did we it a player?
		if (player != nullptr)
		{
			// We hit a player! Kill them!
			m_level->ReloadLevel();
		}
	}

	// If movement is blocked, do nothing, return false
	// Default
	return false;
}