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
# include "Potion.h"
# include "Collagen.h"
# include "CrackedWall.h"
# include "WaterSide.h"
# include "WaterDown.h"

// Constants
#define SPEED 200.0f
#define JUMP_SPEED -370.0f
#define GRAVITY 220.0f
#define PotionXVelocity 500.0f
#define PotionYVelocity -300.0f

Player::Player()
	: GridObject()
	, m_pendingMove(0, 0)
	, m_playerMoveSound()
	, m_playerBumpingSound()
	, m_touchingSurface(false)
	, m_touchingWall(false)
	, m_touchingRoof(false)
	, m_keyBeenPressed(false)
	, m_timerCountdown(60.00f)
	, m_keyDelay (3.00f)
	, m_numericalPotionState(0)
	, m_gameStart(false)
	, m_potion(nullptr)
	, potionState(none)
	, previousPotionState(none)
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

	////////////////
	/// CONTROLS ///
	////////////////

	// Use the keyboard function to check 
	// which keys are currently held down
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && m_touchingSurface == true && m_touchingRoof == false)
	{
		m_velocity.y = JUMP_SPEED;
		m_touchingSurface = false;
		m_gameStart = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_velocity.x = -SPEED;
		m_sprite.setTexture(AssetManager::GetTexture("graphics/player/playerStandLeft.png"));
		m_gameStart = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && m_touchingSurface == false)
	{
		m_velocity.y = SPEED;
		m_gameStart = true;

		// If the player has pressed this key, it means that they will drop off the roof of they haven't aleady done so. 
		// This will allow them to jump again.
		m_touchingRoof = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_velocity.x = SPEED;
		m_sprite.setTexture(AssetManager::GetTexture("graphics/player/playerStandRight.png"));
		m_gameStart = true;
	}

	/////////////
	// Potions //
	/////////////

	// ELEMENT CONTROLS //
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
	{
		// The player has pressed the 1 key, first we need to check if this element is already active.
		if (potionState == Collagen && m_keyBeenPressed == false)
		{
			// It is active, meaning that the player wishes to deactivate this element
			potionState = none;
			m_keyBeenPressed = true;
		} 
		else if (potionState != Collagen && m_keyBeenPressed == false)
		{
			// It is not active, meaning that the player wishes to activate this element
			potionState = Collagen;
			m_numericalPotionState = 1;
			m_keyBeenPressed = true;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
	{
		// The player has pressed the 1 key, first we need to check if this element is already active.
		if (potionState == Electricity && m_keyBeenPressed == false)
		{
			// It is active, meaning that the player wishes to deactivate this element
			potionState = none;
			m_keyBeenPressed = true;
		}
		 else if (potionState != Electricity && m_keyBeenPressed == false)
		{
			// It is not active, meaning that the player wishes to activate this element
			potionState = Electricity;
			m_numericalPotionState = 2;
			m_keyBeenPressed = true;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
	{
		// The player has pressed the 1 key, first we need to check if this element is already active.
		if (potionState == Sulphur && m_keyBeenPressed == false)
		{
			// It is active, meaning that the player wishes to deactivate this element
			potionState = none;
			m_keyBeenPressed = true;
		}
		else if (potionState != Sulphur && m_keyBeenPressed == false)
		{
			// It is not active, meaning that the player wishes to activate this element
			potionState = Sulphur;
			m_numericalPotionState = 3;
			m_keyBeenPressed = true;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
	{
		// The player has pressed the 4 key, first we need to check if this element is already active.
		if (potionState == Hydrogen && m_keyBeenPressed == false)
		{
			// It is active, meaning that the player wishes to deactivate this element
			potionState = none;
			m_keyBeenPressed = true;
		}
		else if (potionState != Hydrogen && m_keyBeenPressed == false)
		{
			// It is not active, meaning that the player wishes to activate this element
			potionState = Hydrogen;
			m_numericalPotionState = 4;
			m_keyBeenPressed = true;
		}
	}

	// Throwing a potion
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && potionState == Sulphur && m_keyBeenPressed == false)
	{
		m_level->CreatePotion();
		if (m_potion != nullptr)
			{ 
				m_potion->SetPosition(m_sprite.getPosition());
			}
		m_keyBeenPressed = true;
		m_gameStart = true;
	}

	//Apply gravity to our velocity
	// If we are not touching any surface on the level or (we are not touching the roof while collagen is active)
	if (m_touchingSurface == false && m_touchingRoof == false)
	{
			float velocityChange = (GRAVITY * _frameTime.asSeconds()) * 2;
			m_velocity.y += velocityChange;
	}

	//Move sprite base on velocity
	sf::Vector2f positionChange = m_velocity * _frameTime.asSeconds();

	//Set the player back to not touching ground or walls by default, 
	//so that the player will drop if they aren't standing on the ground.
	//Has to be before update for level spawing so they don't just drop through a platform has they spawn in.
	if (m_touchingSurface == true)
		m_touchingSurface = false;
	if (m_touchingWall == true)
		m_touchingWall = false;


	//If the game has started, down down from 60 seconds
	if (m_gameStart == true)
	{
		m_timerCountdown -= _frameTime.asSeconds();
	}
	// A a potion key has been pressed, give a 3 second delay before the player can throw a potion again
	if (m_keyBeenPressed == true)
	{
		m_keyDelay -= _frameTime.asSeconds();
	}
	//If the count down reaches 0, everything is reset for the current level.
	if (m_timerCountdown <= 0)
	{
		m_gameStart = false;
		m_timerCountdown = 60.0f;
		m_level->ReloadLevel();
	}

	// The key delay reaches 0, allow the player to throw a potion or use an element again.
	if (m_keyDelay <= 0)
	{
		m_keyBeenPressed = false;
		m_keyDelay = 3.0f;
	}

	//// Call the update function manually on 
	//// the parent class
	//// This will actually move the character
	GridObject::Update(_frameTime);
}

void Player::Collide(GameObject& _collider)
{

	///////////////////
	// DYNAMIC CASTS //
	///////////////////
	Wall* wallCollider = dynamic_cast<Wall*>(&_collider);
	Spike* spikeCollider = dynamic_cast<Spike*>(&_collider);
	Box* boxCollider = dynamic_cast<Box*>(&_collider);
	Killzone* killzoneCollider = dynamic_cast<Killzone*>(&_collider);
	Exit* exitCollider = dynamic_cast<Exit*>(&_collider);
	CrackedWall* crackedCollider = dynamic_cast<CrackedWall*>(&_collider);
	WaterDown* waterCollider = dynamic_cast<WaterDown*>(&_collider);

	//////////////////////////////////////////////////////////////////////////////////////////
	// PLAYER ////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////

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
	//Set it to the right of the player - 10
	rightCollider.left += playerCollider.width - 10;
	// Set our right side collider width to be 5 pixels
	rightCollider.width = 5;

	//Create a collider for the right hand side of the player
	sf::FloatRect leftCollider = playerCollider;
	// Set our head collider height to be 5 pixels
	leftCollider.height -= 4;
	leftCollider.top += 2;
	// Set our left side collider width to be 5 pixels
	leftCollider.width = 5;
	// Needed as the player can drop through the floor otherwise travelling left and down
	leftCollider.left += 5;

	//////////////////////////////////////////////////////////////////////////////////////////
	// WALLS /////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////

	//Record whether we used to be touching the ground
	bool wereTouchingSurface = m_touchingSurface;
	//Assume we did not collide
	m_touchingSurface = false;
	//Record whether we used to be touching a wall
	bool wereTouchingWall = m_touchingWall;
	//Assume we did not collide
	m_touchingWall = false;

	// Only do something if the thing
	// we touched was a wall

	// If it was a wall we hit, we need to more ourselves
	// outside the wall's bounds, aka back where we were
	if (wallCollider != nullptr)
	{
		// Create platform top collider
		sf::FloatRect platformTop = wallCollider->GetBounds();
		platformTop.height = 10;

		// Create platform bottom collider
		sf::FloatRect platformBottom = wallCollider->GetBounds();
		//Set it to the bottom of the platform - 5
		platformBottom.top += wallCollider->GetBounds().height - 5;
		platformBottom.height = 10;
		// Shorten the width to not mess with the other colliders
		platformBottom.width -= 10;
		platformBottom.left += 5;

		//Create a collider for the right hand side of the platform
		sf::FloatRect platformRight = wallCollider->GetBounds();
		//Set it to the right of the platform -5
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
			m_touchingSurface = true;

				//Check if we are falling downward
				if (wereTouchingSurface == false && m_velocity.y > 0)
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
			// Is the player's active element collagen?

			if (potionState == Collagen)
			{
				//The player is no longer able to move up and isn't affected by gravity
				m_touchingRoof = true;

				//Yes it is, sticky movement
				//Check if we are jumping
				if (wereTouchingSurface == false && m_velocity.y <= 0)
				{
					//We have touched the roof
					m_velocity.y = 0;
					m_sprite.setPosition(m_sprite.getPosition().x, wallCollider->getPosition().y + wallCollider->GetBounds().height); //FOR THE WATER INVERT THESE VALUES //
				}
			}
			else
			{
				//Do not reset the player's ability to move up
				m_touchingSurface = false;
				// No it is not, normal movement
				//Check if we are jumping
				if (wereTouchingSurface == false && m_velocity.y < 0)
				{
					//We have touched the roof
					m_velocity.y = 0;
					m_sprite.setPosition(m_sprite.getPosition().x, wallCollider->getPosition().y + wallCollider->GetBounds().height);
				}
			}
		}

		if (rightCollider.intersects(platformLeft))
		{
			m_touchingWall = true;

			//// Is the player's active element collagen?

			if (potionState == Collagen)
			{
				// Yes it is, sticky movement

				//Reset the player's ability to move up
				m_touchingSurface = true;

				if (wereTouchingWall == false && m_velocity.x >= 0)
				{
					m_velocity.x = 0;
					m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
				}
			}
			else
			{
				//Do not reset the player's ability to move up
				m_touchingSurface = false;
				// No it is not, normal movement
				if (wereTouchingWall == false && m_velocity.x > 0)
				{
					m_velocity.x = 0;
					m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
				}
			}
		}

		if (leftCollider.intersects(platformRight))
		{
			m_touchingWall = true;
			
			// Is the player's active element collagen?

			if (potionState == Collagen)
			{
				//Reset the player's ability to move up
				m_touchingSurface = true;
				// Yes it is, sticky movement
				if (wereTouchingWall == false && m_velocity.x <= 0)
				{
					m_velocity.x = 0;
					m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
				}
			}
			else
			{
				//Do not reset the player's ability to move up
				m_touchingSurface = false;
				// No it isn't, normal movement
				if (wereTouchingWall == false && m_velocity.x < 0)
				{
					m_velocity.x = 0;
					m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// BOXES /////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////

	if (boxCollider != nullptr)
	{
		// We did hit a box, now we need to move it based on the velocity of the player!
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// EXIT //////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////

	if (exitCollider != nullptr)
	{
		// We hit the exit, meaning that the player has beaten the level, reset and load next level.
			m_gameStart = false;
			m_timerCountdown = 60.0f;
			m_level->LoadNextLevel();
		
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// KILLZONE //////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////

	if (killzoneCollider != nullptr)
	{
		// We did hit a killzone! Kill the player
		m_level->ReloadLevel();
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// SPIKES ////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////
	
	if (spikeCollider != nullptr)
	{
		// We did hit a spike! Kill the player
		m_level->ReloadLevel();
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// CRACKED WALL //////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////

	if (crackedCollider != nullptr)
	{
		// Create platform top collider
		sf::FloatRect platformTop = crackedCollider->GetBounds();
		platformTop.height = 10;

		// Create platform bottom collider
		sf::FloatRect platformBottom = crackedCollider->GetBounds();
		//Set it to the bottom of the platform - 5
		platformBottom.top += crackedCollider->GetBounds().height - 5;
		platformBottom.height = 10;
		// Shorten the width to not mess with the other colliders
		platformBottom.width -= 10;
		platformBottom.left += 5;

		//Create a collider for the right hand side of the platform
		sf::FloatRect platformRight = crackedCollider->GetBounds();
		//Set it to the right of the platform -5
		platformRight.left += platformRight.width - 5;
		// Set our right side collider width to be 5 pixels
		platformRight.width = 5;

		//Create a collider for the left hand side of the platform
		sf::FloatRect platformLeft = crackedCollider->GetBounds();
		// Set our left side collider width to be 5 pixels
		platformLeft.width = 5;

		// Are the feet touching the top of the platform?
		if (feetCollider.intersects(platformTop))
		{
			// We are now touching the ground!
			m_touchingSurface = true;

			//Check if we are falling downward
			if (wereTouchingSurface == false && m_velocity.y > 0)
			{
				//We have touched the ground
				m_velocity.y = 0;
				m_sprite.setPosition(m_sprite.getPosition().x, crackedCollider->getPosition().y - m_sprite.getGlobalBounds().height);
			}

		}

		// Is the head touching the bottom of the platform?
		if (headCollider.intersects(platformBottom))
		{
			// We are now touching the ground!
			// Is the player's active element collagen?

			if (potionState == Collagen)
			{
				//Reset the player's ability to move up
				m_touchingSurface = true;
				//Yes it is, sticky movement
				//Check if we are jumping
				if (wereTouchingSurface == false && m_velocity.y <= 0)
				{
					//We have touched the roof
					m_velocity.y = 0;
					m_sprite.setPosition(m_sprite.getPosition().x, crackedCollider->getPosition().y + crackedCollider->GetBounds().height);
				}
			}
			else
			{
				//Do not reset the player's ability to move up
				m_touchingSurface = false;
				// No it is not, normal movement
				//Check if we are jumping
				if (wereTouchingSurface == false && m_velocity.y < 0)
				{
					//We have touched the roof
					m_velocity.y = 0;
					m_sprite.setPosition(m_sprite.getPosition().x, crackedCollider->getPosition().y + crackedCollider->GetBounds().height);
				}
			}
		}

		if (rightCollider.intersects(platformLeft))
		{
			m_touchingWall = true;

			//// Is the player's active element collagen?

			if (potionState == Collagen)
			{
				// Yes it is, sticky movement

				//Reset the player's ability to move up
				m_touchingSurface = true;

				if (wereTouchingWall == false && m_velocity.x >= 0)
				{
					m_velocity.x = 0;
					m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
				}
			}
			else
			{
				//Do not reset the player's ability to move up
				m_touchingSurface = false;
				// No it is not, normal movement
				if (wereTouchingWall == false && m_velocity.x > 0)
				{
					m_velocity.x = 0;
					m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
				}
			}
		}

		if (leftCollider.intersects(platformRight))
		{
			m_touchingWall = true;

			// Is the player's active element collagen?

			if (potionState == Collagen)
			{
				//Reset the player's ability to move up
				m_touchingSurface = true;
				// Yes it is, sticky movement
				if (wereTouchingWall == false && m_velocity.x <= 0)
				{
					m_velocity.x = 0;
					m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
				}
			}
			else
			{
				//Do not reset the player's ability to move up
				m_touchingSurface = false;
				// No it isn't, normal movement
				if (wereTouchingWall == false && m_velocity.x < 0)
				{
					m_velocity.x = 0;
					m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// CRACKED WALL //////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////
	
	
	if (waterCollider != nullptr)
	{
		// Create platform top collider
		sf::FloatRect platformTop = waterCollider->GetBounds();
		platformTop.height = 10;

		// Create platform bottom collider
		sf::FloatRect platformBottom = waterCollider->GetBounds();
		//Set it to the bottom of the platform - 5
		platformBottom.top += waterCollider->GetBounds().height - 5;
		platformBottom.height = 10;
		// Shorten the width to not mess with the other colliders
		platformBottom.width -= 10;
		platformBottom.left += 5;

		//Create a collider for the right hand side of the platform
		sf::FloatRect platformRight = waterCollider->GetBounds();
		//Set it to the right of the platform -5
		platformRight.left += platformRight.width - 5;
		// Set our right side collider width to be 5 pixels
		platformRight.width = 5;

		//Create a collider for the left hand side of the platform
		sf::FloatRect platformLeft = waterCollider->GetBounds();
		// Set our left side collider width to be 5 pixels
		platformLeft.width = 5;

		// Are the feet touching the top of the platform?
		if (feetCollider.intersects(platformTop))
		{
			// We are now touching the ground!
			m_touchingSurface = true;

			//Check if we are falling downward
			if (wereTouchingSurface == false && m_velocity.y > 0)
			{
				//We have touched the ground
				m_velocity.y = 0;
				m_sprite.setPosition(m_sprite.getPosition().x, waterCollider->getPosition().y - m_sprite.getGlobalBounds().height);
			}

		}

		// Is the head touching the bottom of the platform?
		if (headCollider.intersects(platformBottom))
		{
			// We are now touching the ground!
			// Is the player's active element collagen?

			if (potionState == Collagen)
			{
				//Reset the player's ability to move up
				m_touchingSurface = true;
				//Yes it is, sticky movement
				//Check if we are jumping
				if (wereTouchingSurface == false && m_velocity.y <= 0)
				{
					//We have touched the roof
					m_velocity.y = 0;
					m_sprite.setPosition(m_sprite.getPosition().x, waterCollider->getPosition().y + waterCollider->GetBounds().height);
				}
			}
			else
			{
				//Do not reset the player's ability to move up
				m_touchingSurface = false;
				// No it is not, normal movement
				//Check if we are jumping
				if (wereTouchingSurface == false && m_velocity.y < 0)
				{
					//We have touched the roof
					m_velocity.y = 0;
					m_sprite.setPosition(m_sprite.getPosition().x, waterCollider->getPosition().y + waterCollider->GetBounds().height);
				}
			}
		}

		if (rightCollider.intersects(platformLeft))
		{
			m_touchingWall = true;

			//// Is the player's active element Electricity?

			if (potionState == Electricity)
			{
				// If electricity is active and the player touches water, push them to the right until they no longer are touching water (zap movement)

				if (wereTouchingWall == false && m_velocity.x >= 0)
				{
					m_velocity.x = 0;
					m_sprite.setPosition(waterCollider->getPosition().x + waterCollider->GetBounds().width, m_sprite.getPosition().y);
				}
			}
			else
			{
				//Do not reset the player's ability to move up
				m_touchingSurface = false;
				// No it is not, normal movement
				if (wereTouchingWall == false && m_velocity.x > 0)
				{
					m_velocity.x = 0;
					m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
				}
			}
		}

		if (leftCollider.intersects(platformRight))
		{
			m_touchingWall = true;

			// Is the player's active element electricity?

			if (potionState == Electricity)
			{
				if (wereTouchingWall == false && m_velocity.x <= 0)
				{
					// If electricity is active and the player touches water, push them to the left until they no longer are touching water (zap movement)
					m_velocity.x = 0;
					m_sprite.setPosition(waterCollider->getPosition().x - waterCollider->GetBounds().width, m_sprite.getPosition().y);
				}
			}
			else
			{
				//Do not reset the player's ability to move up
				m_touchingSurface = false;
				// No it isn't, normal movement
				if (wereTouchingWall == false && m_velocity.x < 0)
				{
					m_velocity.x = 0;
					m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
				}
			}
		}
	}

}

int Player::GetTimer()
{
	return m_timerCountdown;
}

int Player::GetDelay()
{
	return m_keyDelay;
}

int Player::GetPotionState()
{
	return m_numericalPotionState;
}

void Player::ChangeTimer(int _changeBy)
{
	m_timerCountdown += _changeBy;
}

void Player::ChangeDelay(int _changeBy)
{
	m_keyDelay += _changeBy;
}

void Player::SetPotion(Potion* _potion)
{
	m_potion = _potion;
}
