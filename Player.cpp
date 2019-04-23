// Project Includes
# include "Player.h"
# include "Framework/AssetManager.h"
# include "Level.h"
# include "SFML/Audio.hpp"
# include "Dirt.h"
# include "Diamond.h"
# include "Exit.h"
# include "Boulder.h"
# include "Wall.h"
# include "Spike.h"

// Constants
#define SPEED 500.0f
#define JUMP_SPEED -500.0f
#define GRAVITY 200.0f

Player::Player()
	: GridObject()
	, m_pendingMove(0, 0)
	, m_playerMoveSound()
	, m_playerBumpingSound()
	, m_touchingGround(false)
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/player/playerStandLeft.png"));
	m_playerMoveSound.setBuffer(AssetManager::GetSoundBuffer("audio/footstep1.ogg"));
	m_playerBumpingSound.setBuffer(AssetManager::GetSoundBuffer("audio/bump.wav"));
	m_blocksMovement = true;
}

void Player::Update(sf::Time _frameTime)
{
	// First, assume no keys are pressed
	m_velocity.x = 0.0f;

	// Use the keyboard function to check 
	// which keys are currently held down
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && m_touchingGround == true)
	{
		m_velocity.y = JUMP_SPEED;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_velocity.x = -SPEED;
		m_sprite.setTexture(AssetManager::GetTexture("graphics/player/playerStandLeft.png"));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && m_touchingGround == false)
	{
		m_velocity.y = SPEED;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_velocity.x = SPEED;
		m_sprite.setTexture(AssetManager::GetTexture("graphics/player/playerStandRight.png"));
	}

	//Apply gravity to our velocity
	if (m_touchingGround == false)
	{
		float velocityChange = GRAVITY * _frameTime.asSeconds();
		m_velocity.y += velocityChange;
	}

	//Move sprite base on velocity
	sf::Vector2f positionChange = m_velocity * _frameTime.asSeconds();


	//// Call the update function manually on 
	//// the parent class
	//// This will actually move the character
	GridObject::Update(_frameTime);
}

void Player::Collide(GameObject& _collider)
{
	///////////
	///WALLS///
	///////////

	//Record whether we used to be touching the ground
	bool wereTouchingGround = m_touchingGround;
	//Assume we did not collide
	m_touchingGround = false;

	//Get the collider for the player
	sf::FloatRect playerCollider = m_sprite.getGlobalBounds();

	// Only do something if the thing
	// we touched was a wall

	// Dynamic cast the GameObject ref
	// into a Wall pointer
	// if it succeeds, it was a wall
	Wall* wallCollider = dynamic_cast<Wall*>(&_collider);

	// If it was a wall we hit, we need to more ourselves
	// outside the wall's bounds, aka back where we were
	if (wallCollider != nullptr)
	{
		// We did hit the ground!
		m_velocity.y = 0;
		m_velocity.x = 0;
		////Yes feet are touching
		m_touchingGround = true;

		// Create feet collider
		sf::FloatRect feetCollider = playerCollider;
		// Set our feet top to be 10 pixels from the bottom of the player collider
		feetCollider.top += playerCollider.height - 5;
		// Set our feet collider height to be 10 pixels
		feetCollider.height = 5;
		// Set our feet top to be 10 pixels from the bottom of the player collider
		feetCollider.width = feetCollider.width - 5;
		// Set our feet collider height to be 10 pixels
		feetCollider.width = 5;

		// Create platform top collider
		sf::FloatRect platformTop = wallCollider->GetBounds();
		platformTop.height = 10;
		platformTop.width = 10;

		// Are the feet touching the top of the platform?
		if (feetCollider.intersects(platformTop))
		{
			//Check if we are going upward
			if (wereTouchingGround == false && m_velocity.y < 0)
			{
				m_velocity.y = -JUMP_SPEED;
			}

			//Check if we are falling downward
			if (wereTouchingGround == false && m_velocity.y >= 0)
			{
				//We have touched the ground
				m_velocity.y = 0;
			}
		}
	}

	////////////
	///Spikes///
	////////////

	// Dynamic cast the GameObject ref
	// if it succeeds, it was a spike
	Spike* spikeCollider = dynamic_cast<Spike*>(&_collider);
	
	if (spikeCollider != nullptr)
	{
		// We did hit a spike! Kill the player
		m_level->ReloadLevel();
	}
}

//bool Player::AttemptMove(sf::Vector2i _direction)
//{
//	////Attempt to move in the given direction
//
//	//// Get current position
//	//// Calculate target position
//	//sf::Vector2i targetPos = m_gridPosition + _direction;
//
//	//// Check if the spaces are empty
//	//// Get list of objects in our target position
//	//std::vector<GridObject*> targetCellContents = m_level->GetObjectAt(targetPos);
//
//	//// Check if any of those objects block movement
//	//bool blocked = false;
//	//for (int i = 0; i < targetCellContents.size(); i++)
//	//{
//	//	if (targetCellContents[i]->GetBlocksMovement() == true)
//	//	{
//	//		blocked = true;
//	//	}
//	//}
//
//	//// If empty, move there
//	//if (blocked == false)
//	//{
//	//	return m_level->MoveObjectTo(this, targetPos);
//	//}
//	//else 
//	//{
//	//	// Check what the thing is, should we care?
//	//	// Loop through the contents to see if it is a thing we care about
//	//	for (int i = 0; i < targetCellContents.size(); ++i)
//	//	{
//	//		// Check if this element in the vector is dirt
//	//		// by doing a dynamic cast
//	//		Dirt* dirt = dynamic_cast<Dirt*>(targetCellContents[i]);
//	//		// Check if this element in the vector is THE DIAMOND
//	//		// by doing a dynamic cast
//	//		Diamond* diamond = dynamic_cast<Diamond*>(targetCellContents[i]);
//	//		// Check if this element in the vector is the exit
//	//		// by doing a dynamic cast
//	//		Exit* exit = dynamic_cast<Exit*>(targetCellContents[i]);
//	//		// Check if this element in the vector is the exit
//	//		// by doing a dynamic cast
//	//		Boulder* boulder = dynamic_cast<Boulder*>(targetCellContents[i]);
//
//
//	//		// If dynamic cast succeeds, it will NOT a nullptr
//	//		// aka, the object IS dirt
//	//		if (dirt != nullptr)
//	//		{
//	//			// The object IS dirt!
//	//			// DELETE IT
//	//			m_level->deleteObjectAt(dirt, targetPos);
//	//			// and move the player into that position
//	//			return m_level->MoveObjectTo(this, targetPos);
//	//		}
//	//		// OR check is it is a Diamond
//	//		if (diamond != nullptr)
//	//		{
//	//			// The object IS a diamond!
//	//			// give them that score tho.
//	//			m_level->SetCurrentScore();
//	//			// Check if the level is complete yet
//	//			m_level->LevelComplete();
//	//			// DELETE IT
//	//			m_level->deleteObjectAt(diamond, targetPos);
//	//			// and move the player into that position
//	//			return m_level->MoveObjectTo(this, targetPos);
//	//		}
//	//		// OR check is it is the exit and that the level is complete
//	//		if (exit != nullptr)
//	//		{
//	//			// The Object IS the exit, but is the level complete?
//	//			if (m_level->LevelComplete() == true)
//	//			{
//	//				//All diamonds are collected, move to the next level.
//	//				m_level->LoadNextLevel();
//	//			}
//	//		}
//	//		// OR check is it a boulder for and can it be pushed
//	//		if (boulder != nullptr)
//	//		{
//	//			// The Object IS a boulder, but can it be moved?
//	//			bool isPushed = boulder->attemptPush(_direction);
//	//			// Only move the player if they were able t push the boulder
//	//			if (isPushed == true)
//	//			{ // and move the player into that position
//	//				return m_level->MoveObjectTo(this, targetPos);
//	//			}
//	//		}
//	//	}
//	//}
//	// If movement is blocked, do nothing, return false
//	return false;
//}
