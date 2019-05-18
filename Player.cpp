// Project Includes
# include "Player.h"
# include "Framework/AssetManager.h"
# include "Level.h"
# include "SFML/Audio.hpp"
# include "Dirt.h"
# include "Diamond.h"
# include "Exit.h"
# include "Box.h"
# include "Wall.h"
# include "Spike.h"
# include "Killzone.h"

// Constants
#define SPEED 500.0f
#define JUMP_SPEED -300.0f
#define GRAVITY 200.0f

Player::Player()
	: GridObject()
	, m_pendingMove(0, 0)
	, m_playerMoveSound()
	, m_playerBumpingSound()
	, m_touchingGround(false)
	, m_touchingWall(false)
	, m_timerCountdown(60.00)
	, m_gameStart(false)
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
		m_touchingGround = false;
		m_gameStart = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_velocity.x = -SPEED;
		m_sprite.setTexture(AssetManager::GetTexture("graphics/player/playerStandLeft.png"));
		m_gameStart = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && m_touchingGround == false)
	{
		m_velocity.y = SPEED;
		m_gameStart = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_velocity.x = SPEED;
		m_sprite.setTexture(AssetManager::GetTexture("graphics/player/playerStandRight.png"));
		m_gameStart = true;
	}

	//Apply gravity to our velocity
	if (m_touchingGround == false)
	{
		float velocityChange = (GRAVITY * _frameTime.asSeconds()) * 2;
		m_velocity.y += velocityChange;
	}

	//Move sprite base on velocity
	sf::Vector2f positionChange = m_velocity * _frameTime.asSeconds();

	//Set the player back to not touching ground by default, 
	//so that the player will drop if they aren't standing on the ground.
	//Has to be before update for level spawing so they don't just drop through a platform has they spawn in.
	m_touchingGround = false;
	m_touchingWall = false;

	//If the game has started, down down from 60 seconds
	if (m_gameStart = true)
	{
		m_timerCountdown -= _frameTime.asSeconds();
	}
	//If the count down reaches 0, everything is reset for the current level.
	if (m_timerCountdown <= 0)
	{
		m_gameStart = false;
		m_timerCountdown = 60.0f;
		m_level->ReloadLevel();
	}

	
	//// Call the update function manually on 
	//// the parent class
	//// This will actually move the character
	GridObject::Update(_frameTime);
}

void Player::Collide(GameObject& _collider)
{
	//Get the collider for the player
	sf::FloatRect playerCollider = m_sprite.getGlobalBounds();

	// Create feet collider
	sf::FloatRect feetCollider = playerCollider;
	// Set our feet top to be 10 pixels from the bottom of the player collider
	feetCollider.top += playerCollider.height - 5;
	// Set our feet collider height to be 5 pixels
	feetCollider.height = 5;
	// Shorten the width to not mess with the other colliders
	feetCollider.width -= 10;
	feetCollider.left += 5;

	// Create head collider
	sf::FloatRect headCollider = playerCollider;
	// Set our head top to be 5 pixels from the top of the player collider
	headCollider.top += 5;
	// Set our head collider height to be 5 pixels
	headCollider.height = 5;
	// Shorten the width to not mess with the other colliders
	headCollider.width -= 10;
	headCollider.left += 5;

	//Create a collider for the right hand side of the player
	sf::FloatRect rightCollider = playerCollider;
	//Set it to the right of the player -5
	rightCollider.left += playerCollider.width - 5;
	// Set our right side collider width to be 5 pixels
	rightCollider.width = 5;

	//Create a collider for the right hand side of the player
	sf::FloatRect leftCollider = playerCollider;
	// Set our left side collider width to be 5 pixels
	leftCollider.width = 5;

	///////////
	///WALLS///
	///////////

	//Record whether we used to be touching the ground
	bool wereTouchingGround = m_touchingGround;
	//Assume we did not collide
	m_touchingGround = false;
	//Record whether we used to be touching the ground
	bool wereTouchingWall = m_touchingWall;
	//Assume we did not collide
	m_touchingWall = false;

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
		////Yes feet are touching
		// Create platform top collider
		sf::FloatRect platformTop = wallCollider->GetBounds();
		platformTop.height = 10;

		// Create platform bottom collider
		sf::FloatRect platformBottom = wallCollider->GetBounds();
		//Set it to the bottom of the platform - 5
		platformBottom.height += wallCollider->GetBounds().height - 5;
		platformTop.height = 10;
		// Shorten the width to not mess with the other colliders
		platformBottom.width -= 10;
		platformBottom.left += 5;

		//Create a collider for the right hand side of the platform
		sf::FloatRect platformRight = wallCollider->GetBounds();
		//Set it to the right of the player -5
		platformRight.left += platformRight.width - 5;
		// Set our right side collider width to be 5 pixels
		platformRight.width = 5;

		//Create a collider for the left hand side of the platform
		sf::FloatRect platformLeft = wallCollider->GetBounds();
		// Set our left side collider width to be 5 pixels
		platformLeft.width = 5;

		// Are the feet touching the top of the platform?
		if (feetCollider.intersects(platformTop))
		{
			// We are now touching the ground!
			m_touchingGround = true;

			//Check if we are falling downward
			if (wereTouchingGround == false && m_velocity.y > 0)
			{
				//We have touched the ground
				m_velocity.y = 0;
				m_sprite.setPosition(m_sprite.getPosition().x, wallCollider->getPosition().y - m_sprite.getGlobalBounds().height);
			}
		}

		// Is the head touching the bottom of the platform?
		if (headCollider.intersects(platformBottom))
		{
			// We are now touching the ground!
			m_touchingGround = true;

			//Check if we are jumping
			if (wereTouchingGround == false && m_velocity.y < 0)
			{
				//We have touched the roof
				m_velocity.y = 0;
				m_sprite.setPosition(m_sprite.getPosition().x, wallCollider->getPosition().y + wallCollider->GetBounds().height);
			}
		}

		if (rightCollider.intersects(platformLeft))
		{
			m_touchingWall = true;

			if (wereTouchingWall == false && m_velocity.x > 0)
			{
				m_velocity.x = 0;
				m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
			}
		}

		if (leftCollider.intersects(platformRight))
		{
			m_touchingWall = true;

			if (wereTouchingWall == false && m_velocity.x < 0)
			{
				m_velocity.x = 0;
				m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
			}
		}
	}

	//// Dynamic cast the GameObject ref
	//// if it succeeds, it was a box
	Box* boxCollider = dynamic_cast<Box*>(&_collider);

	if (boxCollider != nullptr)
	{
		// We did hit a box, now we need to move it based on the velocity of the player!
	}

	Exit* exitCollider = dynamic_cast<Exit*>(&_collider);

	if (exitCollider != nullptr)
	{
		// We hit the exit, meaning that the player has beaten the level, reset and load next level.
			m_gameStart = false;
			m_timerCountdown = 60.0f;
			m_level->LoadNextLevel();
		
	}

	//////////////
	///Killzone///
	//////////////


	// Dynamic cast the GameObject ref
	// if it succeeds, it was a killzone
	Killzone* killzoneCollider = dynamic_cast<Killzone*>(&_collider);

	if (killzoneCollider != nullptr)
	{
		// We did hit a killzone! Kill the player
		m_level->ReloadLevel();
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

int Player::GetTimer()
{
	return m_timerCountdown;
}

void Player::ChangeScore(int _changeBy)
{
	m_timerCountdown += _changeBy;
}
