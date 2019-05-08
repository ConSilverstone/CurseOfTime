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
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/box.png"));
	m_blocksMovement = true;
}

void Box::Update(sf::Time _frameTime)
{

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
	//Record whether we used to be touching the ground
	bool wereTouchingGround = m_touchingGround;
	//Assume we did not collide
	m_touchingGround = false;
	//Record whether we used to be touching the ground
	bool wereTouchingWall = m_touchingWall;
	//Assume we did not collide
	m_touchingWall = false;

	// Dynamic cast the GameObject ref
	// into a Wall pointer
	// if it succeeds, it was a wall
	Wall* wallCollider = dynamic_cast<Wall*>(&_collider);

	// If it was a wall we hit, we need to more ourselves
	// outside the wall's bounds, aka back where we were
	if (wallCollider != nullptr)
	{
		// We are now touching the ground!
		m_touchingGround = true;

		//Check if we are falling downward
		if (wereTouchingGround == false && m_velocity.y > 0)
		{
			//We have touched the ground
			m_velocity.y = 0;
			m_sprite.setPosition(m_previousPosition);
		}
	}
}

void Box::BoxSlide(sf::Vector2f _velocity)
{
	m_velocity = _velocity;
}
