// Project Includes
#include "Boulder.h"
#include "Framework/AssetManager.h"
#include "Level.h"
#include "Player.h"
#include "Diamond.h"

Boulder::Boulder()
	: GridObject()
	, m_currentTime(0.0f)
	, m_timeFallen(0.5f)
	, hasSlid(false)
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/box.png"));
	m_blocksMovement = true;
}

void Boulder::Update(sf::Time _frameTime)
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

bool Boulder::canItFall(sf::Vector2i _direction)
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
		Boulder* boulderSlide = dynamic_cast<Boulder*>(blocker);
		Diamond* boulderSlideDiamond = dynamic_cast<Diamond*>(blocker);
		if (killPlayer != nullptr)
		{
			return true;
		}
		else if (boulderSlide != nullptr) 
		{
			return true;
		}
		else if (boulderSlideDiamond != nullptr) 
		{
			return true;
		}else{
			return false;
		}
	}
}

bool Boulder::attemptFall(sf::Vector2i _direction)
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
			hasSlid = false;
			return m_level->MoveObjectTo(this, targetPos);
		}
		else
		{
			// We were blocked!
			// Do a dynamic cast to a boulder to see if we can fall
			Boulder* boulder = dynamic_cast<Boulder*>(blocker);
			// Do a dymanic cast to see of the boulder hit a player
			Player* player = dynamic_cast<Player*>(blocker);
			// Do a dynamic cast to a diamond see if we can fall 
			Diamond* diamond = dynamic_cast<Diamond*>(blocker);

				// If so (the blocker is a boulder (not nullptr)
				if (boulder != nullptr & hasSlid == false)
				{
					//it is a boulder!
					//Can we move left or right?

					sf::Vector2i newTargetPos = m_gridPosition;
					
						newTargetPos.x = m_gridPosition.x + 1;
						targetCellContents = m_level->GetObjectAt(newTargetPos);
					

					// Check if any of those objects block movement
					blocked = false;
					for (int i = 0; i < targetCellContents.size(); ++i)
					{
						if (targetCellContents[i]->GetBlocksMovement() == true)
						{
							blocked = true;
							blocker = targetCellContents[i];
						}
					}

						if (blocked == false)
						{
							hasSlid = true;
							return m_level->MoveObjectTo(this, newTargetPos);
						}
						else
						{

								newTargetPos.x = m_gridPosition.x - 1;
								targetCellContents = m_level->GetObjectAt(newTargetPos);
								
							
							// Check if any of those objects block movement
							blocked = false;
							for (int i = 0; i < targetCellContents.size(); ++i)
							{
								if (targetCellContents[i]->GetBlocksMovement() == true)
								{
									blocked = true;
									blocker = targetCellContents[i];
								}
							}

							if (blocked == false)
							{
								hasSlid = true;
								return m_level->MoveObjectTo(this, newTargetPos);
							}
						}
					
					///AS THE BOULDER IS MOVEING SIDEWARDS///
					// Do a dymanic cast to see of the boulder hit a player
					Player* player = dynamic_cast<Player*>(blocker);

					// did we hit a player? (while moving sidewards)
					if (player != nullptr)
					{
						// We hit a player! Kill them!
						m_level->ReloadLevel();
					}
				}

				// If so (the blocker is a boulder (not nullptr)
				if (diamond != nullptr & hasSlid == false)
				{
					//it is a diamond!
					//Can we move left or right?

					sf::Vector2i newTargetPos = m_gridPosition;
					
						newTargetPos.x = m_gridPosition.x + 1;
						targetCellContents = m_level->GetObjectAt(newTargetPos);
					

					// Check if any of those objects block movement
					blocked = false;
					for (int i = 0; i < targetCellContents.size(); ++i)
					{
						if (targetCellContents[i]->GetBlocksMovement() == true)
						{
							blocked = true;
							blocker = targetCellContents[i];
						}
					}

						if (blocked == false)
						{
							hasSlid = true;
							return m_level->MoveObjectTo(this, newTargetPos);
						}
						else
						{

								newTargetPos.x = m_gridPosition.x - 1;
								targetCellContents = m_level->GetObjectAt(newTargetPos);
								
							
							// Check if any of those objects block movement
							blocked = false;
							for (int i = 0; i < targetCellContents.size(); ++i)
							{
								if (targetCellContents[i]->GetBlocksMovement() == true)
								{
									blocked = true;
									blocker = targetCellContents[i];
								}
							}

							if (blocked == false)
							{
								hasSlid = true;
								return m_level->MoveObjectTo(this, newTargetPos);
							}
						}
					
					///AS THE BOULDER IS MOVEING SIDEWARDS///
					// Do a dymanic cast to see of the boulder hit a player
					Player* player = dynamic_cast<Player*>(blocker);

					// did we hit a player? (while moving sidewards)
					if (player != nullptr)
					{
						// We hit a player! Kill them!
						m_level->ReloadLevel();
					}
				}
			
			///AS THE BOULDER IS MOVEING DOWN///
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

bool Boulder::attemptPush(sf::Vector2i _direction)
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

	// If movement is blocked, do nothing, return false
	// Default
	return false;
}
