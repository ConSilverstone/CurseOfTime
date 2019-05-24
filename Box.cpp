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
	sf::FloatRect boxCollider = m_sprite.getGlobalBounds();

		// Create platform top collider
		sf::FloatRect boxTop = boxCollider;
		boxTop.height = 10;

		// Create platform bottom collider
		sf::FloatRect boxBottom = boxCollider;
		//Set it to the bottom of the platform - 5
		boxBottom.top += boxCollider.height - 10;
		boxBottom.height = 10;

		//Create a collider for the right hand side of the platform
		sf::FloatRect boxRight = boxCollider;;
		//Set it to the right of the platform -5
		boxRight.left += boxRight.width - 5;
		// Set our right side collider width to be 5 pixels
		boxRight.width = 5;

		//Create a collider for the left hand side of the platform
		sf::FloatRect boxLeft = boxCollider;
		// Set our left side collider width to be 5 pixels
		boxLeft.width = 5;

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
		////Yes the box is touching
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
		if (boxBottom.intersects(platformTop))
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
		if (boxTop.intersects(platformBottom))
		{
			// We are now touching the ground!
			m_touchingWall = true;

			//Check if we are jumping
			if (wereTouchingWall == false && m_velocity.y < 0)
			{
				//We have touched the roof
				m_velocity.y = 0;
				m_sprite.setPosition(m_sprite.getPosition().x, wallCollider->getPosition().y + wallCollider->GetBounds().height);
			}

		}

		if (boxRight.intersects(platformLeft))
		{
			// We are now touching the ground!
			m_touchingWall = true;
			
			if (wereTouchingWall == false && m_velocity.x > 0)
			{
				m_velocity.x = 0;
				m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
			}

		}

		if (boxLeft.intersects(platformRight))
		{
			// We are now touching the ground!
			m_touchingWall = true;

			if (wereTouchingWall == false && m_velocity.x < 0)
			{
				m_velocity.x = 0;
				m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
			}
		}
	}

	//////////////////
	////// BOXES /////
	//////////////////

	// Dynamic cast the GameObject ref
	// into a box pointer
	// if it succeeds, it was a box
	Box* collidedBoxCollider = dynamic_cast<Box*>(&_collider);

	// If it was a box we hit, we need to move ourselves
	// outside the box's bounds, aka back where we were
	if (collidedBoxCollider != nullptr)
	{
		// Create platform top collider
		sf::FloatRect boxWeAreTouchingTop = collidedBoxCollider->GetBounds();
		boxWeAreTouchingTop.height = 10;

		// Create platform bottom collider
		sf::FloatRect boxWeAreTouchingBottom = collidedBoxCollider->GetBounds();
		//Set it to the bottom of the platform - 5
		boxWeAreTouchingBottom.top += collidedBoxCollider->GetBounds().height - 10;
		boxWeAreTouchingBottom.height = 10;
		// Shorten the width to not mess with the other colliders
		boxWeAreTouchingBottom.width -= 10;
		boxWeAreTouchingBottom.left += 5;

		//Create a collider for the right hand side of the platform
		sf::FloatRect boxWeAreTouchingRight = collidedBoxCollider->GetBounds();
		//Set it to the right of the platform -5
		boxWeAreTouchingRight.left += boxWeAreTouchingRight.width - 5;
		// Set our right side collider width to be 5 pixels
		boxWeAreTouchingRight.width = 5;

		//Create a collider for the left hand side of the platform
		sf::FloatRect boxWeAreTouchingLeft = collidedBoxCollider->GetBounds();
		// Set our left side collider width to be 5 pixels
		boxWeAreTouchingLeft.width = 5;

		// Are the feet touching the top of the platform?
		if (boxBottom.intersects(boxWeAreTouchingTop))
		{
			// We are now touching the ground!
			m_touchingWall = true;

			//Check if we are falling downward
			if (wereTouchingSurface == false && m_velocity.y > 0)
			{
				//We have touched the ground
				m_velocity.y = 0;
				m_sprite.setPosition(m_sprite.getPosition().x, collidedBoxCollider->getPosition().y - m_sprite.getGlobalBounds().height);
			}

		}

		// Is the head touching the bottom of the platform?
		if (boxTop.intersects(boxWeAreTouchingBottom))
		{
			// We are now touching the ground!
			m_touchingWall = true;

			// No it is not, normal movement
			//Check if we are jumping
			if (wereTouchingWall == false && m_velocity.y < 0)
			{
				//We have touched the roof
				m_velocity.y = 0;
				m_sprite.setPosition(m_sprite.getPosition().x, collidedBoxCollider->getPosition().y + wallCollider->GetBounds().height);
			}

		}

		if (boxRight.intersects(boxLeft))
		{
			// We are now touching the ground!
			m_touchingWall = true;

			// No it is not, normal movement
			if (wereTouchingWall == false && m_velocity.x > 0)
			{
				m_velocity.x = 0;
				m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
			}

		}

		if (boxLeft.intersects(boxWeAreTouchingRight))
		{
			// We are now touching the ground!
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

void Box::BoxSlide(sf::Vector2f _velocity)
{
	m_velocity = _velocity;
}
