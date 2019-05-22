// Project Includes
#include "Box.h"
#include "Framework/AssetManager.h"
#include "Level.h"
#include "Player.h"
#include "Diamond.h"
#include "Wall.h"

// Constants
#define GRAVITY 200.0f

Box::Box()
	: GridObject()
	, m_touchingGround(false)
	, m_touchingWall(false)
	, m_player(nullptr)
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/box.png"));
	m_blocksMovement = true;
}

void Box::Update(sf::Time _frameTime)
{
	// First we assume that the box isn't moving
	m_velocity.x = 0.0f;

	//Apply gravity to our velocity
	if (m_touchingGround == false)
	{
		float velocityChange = (GRAVITY * _frameTime.asSeconds()) * 2;
		m_velocity.y += velocityChange;
	}

	//Move sprite base on velocity
	sf::Vector2f positionChange = m_velocity * _frameTime.asSeconds();

	//Set the box back to not touching ground by default
	m_touchingGround = false;
	m_touchingWall = false;

	//// Call the update function manually on 
	//// the parent class
	//// This will actually move the character
	GridObject::Update(_frameTime);
}

void Box::Collide(GameObject& _collider)
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

	///////////
	///WALLS///
	///////////

	//Record whether we used to be touching the ground
	bool wereTouchingSurface = m_touchingWall;
	//Assume we did not collide
	m_touchingWall = false;
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
		platformBottom.top += wallCollider->GetBounds().height - 5;
		platformBottom.height = 10;
		// Shorten the width to not mess with the other colliders
		//platformBottom.width -= 10;
		//platformBottom.left += 5;

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
			m_touchingWall = true;

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

				//Do not reset the player's ability to move up
				m_touchingWall = false;
				// No it is not, normal movement
				//Check if we are jumping
				if (wereTouchingWall == false && m_velocity.y < 0)
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

				//Do not reset the player's ability to move up
				m_touchingWall = false;
				// No it is not, normal movement
				if (wereTouchingWall == false && m_velocity.x > 0)
				{
					m_velocity.x = 0;
					m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
				}
			
		}

		if (leftCollider.intersects(platformRight))
		{
				m_touchingWall = true;

				//Do not reset the player's ability to move up
				m_touchingWall = false;
				// No it isn't, normal movement
				if (wereTouchingWall == false && m_velocity.x < 0)
				{
					m_velocity.x = 0;
					m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
				}
			}
		}
	
}

void Box::BoxSlide(sf::Vector2f _velocity)
{
	m_velocity = _velocity;
}
