#include "Potion.h"
#include "Player.h"
#include "Wall.h"
#include "CrackedWall.h"
#include "Level.h"
#include "Framework/AssetManager.h"

// Constants
#define xVelocity 300.0f
#define yVelocity -300.0f
#define GRAVITY 200.0f

Potion::Potion()
	: GridObject()
	, m_player(nullptr)
	, m_touchingSurface(false)
	, m_touchingWall(false)
{
	PotionEffects(none);
	m_sprite.setTexture(AssetManager::GetTexture("graphics/PotionVial.png"));
	m_sprite.setPosition(0, 0);
	m_blocksMovement = true;
}

void Potion::Update(sf::Time _frameTime)
{
	if (m_player != nullptr)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			m_velocity.x = xVelocity;
			m_velocity.y = yVelocity;
		}
	}
		
	//Apply gravity to our velocity
	// If we are not touching any surface on the level or (we are not touching the roof while collagen is active)
	if (m_touchingSurface == false)
	{
		float velocityChange = (GRAVITY * _frameTime.asSeconds()) * 2;
		m_velocity.y += velocityChange;
	}

	//Move sprite base on velocity
	sf::Vector2f positionChange = m_velocity * _frameTime.asSeconds();

	//Set the potion back to not touching ground or walls by default, 
	//Has to be before update for level spawing so they don't just drop through a platform has they spawn in.
	if (m_touchingSurface == true)
		m_touchingSurface = false;
	if (m_touchingWall == true)
		m_touchingWall = false;

	//// Call the update function manually on 
	//// the parent class
	//// This will actually move the character

	GridObject::Update(_frameTime);
}

void Potion::Collide(GameObject & _collider)
{
	///////////////////
	// DYNAMIC CASTS //
	///////////////////
	Wall* wallCollider = dynamic_cast<Wall*>(&_collider);
	CrackedWall* crackedCollider = dynamic_cast<CrackedWall*>(&_collider);

	//////////////////////////////////////////////////////////////////////////////////////////
	// PLAYER ////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////

	//Get the collider for the player
	sf::FloatRect potionCollider = m_sprite.getGlobalBounds();

	// Create feet collider
	sf::FloatRect potionBottomCollider = potionCollider;
	// Set our feet top to be 10 pixels from the bottom of the player collider
	potionBottomCollider.top += potionCollider.height - 2.5f;
	// Set our feet collider height to be 5 pixels
	potionBottomCollider.height = 2.5f;
	// Shorten the width to not mess with the other colliders
	potionBottomCollider.width -= 5;
	potionBottomCollider.left += 2.5f;

	// Create head collider
	sf::FloatRect potionTopCollider = potionCollider;
	// Set our head top to be 5 pixels from the top of the player collider
	potionTopCollider.top += 2.5f;
	// Set our head collider height to be 5 pixels
	potionTopCollider.height = 2.5f;
	// Shorten the width to not mess with the other colliders
	potionTopCollider.width -= 5;
	potionTopCollider.left += 2.5f;

	//Create a collider for the right hand side of the player
	sf::FloatRect potionRightCollider = potionCollider;
	//Set it to the right of the player - 10
	potionRightCollider.left += potionCollider.width - 5;
	// Set our right side collider width to be 5 pixels
	potionRightCollider.width = 2.5f;

	//Create a collider for the right hand side of the player
	sf::FloatRect potionLeftCollider = potionCollider;
	// Set our head collider height to be 5 pixels
	potionLeftCollider.height -= 2;
	potionLeftCollider.top += 1;
	// Set our left side collider width to be 5 pixels
	potionLeftCollider.width = 2.5f;
	// Needed as the player can drop through the floor otherwise travelling left and down
	potionLeftCollider.left += 2.5f;

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

	// If it was a wall we hit, we need to more ourselves
	// outside the wall's bounds, aka back where we were
	if (wallCollider != nullptr)
	{

		m_level->deleteObjectAt(this);
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
		if (potionBottomCollider.intersects(platformTop))
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
		if (potionTopCollider.intersects(platformBottom))
		{
			// We are now touching the roof!

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

		if (potionRightCollider.intersects(platformLeft))
		{
			m_touchingWall = true;

				//Do not reset the player's ability to move up
				m_touchingSurface = false;
				// No it is not, normal movement
				if (wereTouchingWall == false && m_velocity.x > 0)
				{
					m_velocity.x = 0;
					m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
				}
			
		}

		if (potionLeftCollider.intersects(platformRight))
		{
			m_touchingWall = true;

				// No it isn't, normal movement
				if (wereTouchingWall == false && m_velocity.x < 0)
				{
					m_velocity.x = 0;
					m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
				}
			
		}
	}
}

void Potion::SetPlayer(Player * _player)
{
	m_player = _player;
}
