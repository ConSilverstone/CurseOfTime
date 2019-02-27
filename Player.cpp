// Project Includes
# include "Player.h"
# include "Framework/AssetManager.h"
# include "Level.h"
# include "SFML/Audio.hpp"
# include "Dirt.h"
# include "Diamond.h"
# include "Exit.h"
# include "Boulder.h"

Player::Player()
	: GridObject()
	, m_pendingMove(0, 0)
	, m_playerMoveSound()
	, m_playerBumpingSound()
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/player/playerStandLeft.png"));
	m_playerMoveSound.setBuffer(AssetManager::GetSoundBuffer("audio/footstep1.ogg"));
	m_playerBumpingSound.setBuffer(AssetManager::GetSoundBuffer("audio/bump.wav"));
	m_blocksMovement = true;
}

void Player::Input(sf::Event _gameEvent)
{
	// Read the mput from the keyoboard and covert it to a direction 
	// for the player to move and then move

	// Was the event a key press
	if (_gameEvent.type == sf::Event::KeyPressed)
	{
		// Yes it was a key press!

		// What key was pressed?
		if (_gameEvent.key.code == sf::Keyboard::W)
		{
			// It was W!
			// Move up
			m_pendingMove = sf::Vector2i(0, -1);
		}
		// What key was pressed?
		else if (_gameEvent.key.code == sf::Keyboard::S)
		{
			// It was S!
			// Move down
			m_pendingMove = sf::Vector2i(0, 1);
		}
		else if (_gameEvent.key.code == sf::Keyboard::A)
		{
			// It was A!
			// Move down
			m_pendingMove = sf::Vector2i(-1, 0);
			m_sprite.setTexture(AssetManager::GetTexture("graphics/player/playerStandLeft.png"));
		}
		else if (_gameEvent.key.code == sf::Keyboard::D)
		{
			// It was D!
			// Move down
			m_pendingMove = sf::Vector2i(1, 0);
			m_sprite.setTexture(AssetManager::GetTexture("graphics/player/playerStandRight.png"));
		}
	}
}

void Player::Update(sf::Time _frameTime)
{
	// IF we have movement waiting to be processed,
	if (m_pendingMove.x != 0 || m_pendingMove.y != 0)
	{
		// Move in that direction
		bool moveSuccessful = AttemptMove(m_pendingMove);

		if (moveSuccessful == true)
		{
			// Play walking sound
			m_playerMoveSound.play();
		}
		else
		{
			m_playerBumpingSound.play();
		}
		// clear pending move.
		m_pendingMove = sf::Vector2i(0, 0);

	}
}

bool Player::AttemptMove(sf::Vector2i _direction)
{
	//Attempt to move in the given direction

	// Get current position
	// Calculate target position
	sf::Vector2i targetPos = m_gridPosition + _direction;

	// Check if the spaces are empty
	// Get list of objects in our target position
	std::vector<GridObject*> targetCellContents = m_level->GetObjectAt(targetPos);

	// Check if any of those objects block movement
	bool blocked = false;
	for (int i = 0; i < targetCellContents.size(); i++)
	{
		if (targetCellContents[i]->GetBlocksMovement() == true)
		{
			blocked = true;
		}
	}

	// If empty, move there
	if (blocked == false)
	{
		return m_level->MoveObjectTo(this, targetPos);
	}
	else 
	{
		// Check what the thing is, should we care?
		// Loop through the contents to see if it is a thing we care about
		for (int i = 0; i < targetCellContents.size(); ++i)
		{
			// Check if this element in the vector is dirt
			// by doing a dynamic cast
			Dirt* dirt = dynamic_cast<Dirt*>(targetCellContents[i]);
			// Check if this element in the vector is THE DIAMOND
			// by doing a dynamic cast
			Diamond* diamond = dynamic_cast<Diamond*>(targetCellContents[i]);
			// Check if this element in the vector is the exit
			// by doing a dynamic cast
			Exit* exit = dynamic_cast<Exit*>(targetCellContents[i]);
			// Check if this element in the vector is the exit
			// by doing a dynamic cast
			Boulder* boulder = dynamic_cast<Boulder*>(targetCellContents[i]);


			// If dynamic cast succeeds, it will NOT a nullptr
			// aka, the object IS dirt
			if (dirt != nullptr)
			{
				// The object IS dirt!
				// DELETE IT
				m_level->deleteObjectAt(dirt, targetPos);
				// and move the player into that position
				return m_level->MoveObjectTo(this, targetPos);
			}
			// OR check is it is a Diamond
			if (diamond != nullptr)
			{
				// The object IS a diamond!
				// give them that score tho.
				m_level->SetCurrentScore();
				// Check if the level is complete yet
				m_level->LevelComplete();
				// DELETE IT
				m_level->deleteObjectAt(diamond, targetPos);
				// and move the player into that position
				return m_level->MoveObjectTo(this, targetPos);
			}
			// OR check is it is the exit and that the level is complete
			if (exit != nullptr)
			{
				// The Object IS the exit, but is the level complete?
				if (m_level->LevelComplete() == true)
				{
					//All diamonds are collected, move to the next level.
					m_level->LoadNextLevel();
				}
			}
			// OR check is it a boulder for and can it be pushed
			if (boulder != nullptr)
			{
				// The Object IS a boulder, but can it be moved?
				bool isPushed = boulder->attemptPush(_direction);
				// Only move the player if they were able t push the boulder
				if (isPushed == true)
				{ // and move the player into that position
					return m_level->MoveObjectTo(this, targetPos);
				}
			}
		}
	}
	// If movement is blocked, do nothing, return false
	return false;
}
