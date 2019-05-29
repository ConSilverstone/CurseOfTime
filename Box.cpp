// Project Includes
#include "Box.h"
#include "Framework/AssetManager.h"
#include "Level.h"
#include "Player.h"
#include "Wall.h"
#include "Spike.h"
#include "CrackedWall.h"

// Constants
#define GRAVITY 200.0f

Box::Box()
	: GridObject()
	, m_touchingGround(false)
	, m_touchingWall(false)
	, m_player(nullptr)
	, m_AmountMoved(0.0f)
	, m_boxPush()
	, m_isPushed(false)
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/box.png"));
	m_blocksMovement = true;

	////////////////
	/// SOUNDS /////
	////////////////
	m_boxPush.setBuffer(AssetManager::GetSoundBuffer("audio/PushCrate.wav"));
}

void Box::Update(sf::Time _frameTime)
{
	// First we assume that the box isn't moving
	m_velocity.x = 0.0f;

	//Apply gravity to our velocity
	if (m_isPushed == true)
	{
		float velocityChange = (GRAVITY * _frameTime.asSeconds()) * 2;
		m_velocity.y += velocityChange;

		//// Call the update function manually on 
		//// the parent class to move the box
		//// because the box only moves when it falls
		//// we can put it here to lessen game lag
		GridObject::Update(_frameTime);

		// Track how far the box has fallen
		m_AmountMoved += m_sprite.getPosition().y - m_previousPosition.y;
	}

	// If the box has fallen, update the move object to so that it understands it no longer in the same grid position.
	if (m_AmountMoved >= 64)
	{
		m_level->MoveObjectTo(this, m_gridPosition + sf::Vector2i(0, 1));
		m_AmountMoved = 0;
	}

	//Move sprite based on velocity
	sf::Vector2f positionChange = m_velocity * _frameTime.asSeconds();

	//Set the box back to not touching ground by default
	m_touchingGround = false;
	m_touchingWall = false;
}

void Box::Collide(GameObject& _collider)
{

	///////////////////
	// DYNAMIC CASTS //
	///////////////////
	
	// Dynamic cast the GameObject ref
	// into a box pointer
	// if it succeeds, it was a...
	Wall* wallCollider = dynamic_cast<Wall*>(&_collider);
	Box* collidedBoxCollider = dynamic_cast<Box*>(&_collider);
	Spike* spikeCollider = dynamic_cast<Spike*>(&_collider);
	CrackedWall* crackedWallCollider = dynamic_cast<CrackedWall*>(&_collider);

	////////////////////////
	// INITIAL BOX /////////
	////////////////////////

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
		// Shorten the width to not mess with the other colliders
		boxBottom.width -= 10;
		boxBottom.left += 5;

		//Create a collider for the right hand side of the platform
		sf::FloatRect boxRight = boxCollider;;
		//Set it to the right of the platform - 10
		boxRight.left += boxRight.width - 10;
		// Set our right side collider width to be 5 pixels
		boxRight.width = 5;

		//Create a collider for the left hand side of the platform
		sf::FloatRect boxLeft = boxCollider;
		// Push the box left rect in by 10
		boxRight.left += 10;
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

	// We only want to run the collision code if the box has been pushed (as it is the only time the box will move)
	if (m_isPushed == true)
	{
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
			if (boxBottom.intersects(platformTop))
			{
				// We are now touching the ground!
				m_touchingWall = true;

				//If the box is on the wall, don't track its ability to fall as the box is technically always moving downwards.
				m_AmountMoved = 0;

				//Check if we are falling downward
				if (wereTouchingSurface == false && m_velocity.y > 0)
				{
					//We have touched the ground
					m_velocity.y = 0;
					m_sprite.setPosition(m_sprite.getPosition().x, wallCollider->getPosition().y - m_sprite.getGlobalBounds().height);
				}

				m_isPushed = false; // The box has finally landed, therefore is should no longer run collision code //
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

		////////////////////////////////
		////// BOX WE ARE TOUCHING /////
		////////////////////////////////

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

				//If the box is on a box, don't track its ability to fall as the box is technically always moving downwards.
				m_AmountMoved = 0;

				//Check if we are falling downward
				if (wereTouchingSurface == false && m_velocity.y > 0)
				{
					//We have touched the ground
					m_velocity.y = 0;
					m_sprite.setPosition(m_sprite.getPosition().x, collidedBoxCollider->getPosition().y - m_sprite.getGlobalBounds().height);
				}

				m_isPushed = false; // The box has finally landed, therefore is should no longer run collision code //
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

		//////////////////
		////// SPIKES ////
		//////////////////

		// If it was a spike we hit, we need to more ourselves
		// outside the spike's bounds, aka back where we were
		if (spikeCollider != nullptr)
		{
			////Yes the box is touching
			// Create platform top collider
			sf::FloatRect spikeTop = spikeCollider->GetBounds();
			spikeTop.height = 10;
			// Shorten the width to not mess with the other colliders
			spikeTop.width -= 10;
			spikeTop.left += 5;

			// Create platform bottom collider
			sf::FloatRect spikeBottom = spikeCollider->GetBounds();
			//Set it to the bottom of the platform - 5
			spikeBottom.top += spikeCollider->GetBounds().height - 5;
			spikeBottom.height = 10;
			// Shorten the width to not mess with the other colliders
			spikeBottom.width -= 10;
			spikeBottom.left += 5;

			//Create a collider for the right hand side of the platform
			sf::FloatRect spikeRight = spikeCollider->GetBounds();
			//Set it to the right of the platform -5
			spikeRight.left += spikeRight.width - 5;
			// Set our right side collider width to be 5 pixels
			spikeRight.width = 5;

			//Create a collider for the left hand side of the platform
			sf::FloatRect spikeLeft = spikeCollider->GetBounds();
			// Set our left side collider width to be 5 pixels
			spikeLeft.width = 5;

			// Are the feet touching the top of the platform?
			if (boxBottom.intersects(spikeTop))
			{
				// We are now touching the ground!
				m_touchingWall = true;
				m_touchingGround = true;

				//If the box is on a spike, don't track its ability to fall as the box is technically always moving downwards.
				m_AmountMoved = 0;

				//Check if we are falling downward
				if (wereTouchingSurface == false && m_velocity.y > 0)
				{
					//We have touched the ground
					m_velocity.y = 0;
					m_sprite.setPosition(m_sprite.getPosition().x, spikeCollider->getPosition().y - m_sprite.getGlobalBounds().height);
				}

				m_isPushed = false; // The box has finally landed, therefore is should no longer run collision code //
			}

			// Is the head touching the bottom of the platform?
			if (boxTop.intersects(spikeBottom))
			{
				// We are now touching the ground!
				m_touchingWall = true;

				//We have touched the roof
				m_velocity.y = 0;
				m_sprite.setPosition(m_sprite.getPosition().x, spikeCollider->getPosition().y + spikeCollider->GetBounds().height);
			}

			if (boxRight.intersects(spikeLeft))
			{
				// We are now touching the ground!
				m_touchingWall = true;

				if (wereTouchingWall == false && m_velocity.x > 0)
				{
					m_velocity.x = 0;
					m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
				}

			}

			if (boxLeft.intersects(spikeRight))
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

		/////////////////////////
		////// CRACKED WALLS ////
		/////////////////////////

		// If it was a spike we hit, we need to more ourselves
		// outside the spike's bounds, aka back where we were
		if (crackedWallCollider != nullptr)
		{
			////Yes the box is touching
			// Create platform top collider
			sf::FloatRect crackedTop = crackedWallCollider->GetBounds();
			crackedTop.height = 10;

			// Create platform bottom collider
			sf::FloatRect crackedBottom = crackedWallCollider->GetBounds();
			//Set it to the bottom of the platform - 5
			crackedBottom.top += crackedWallCollider->GetBounds().height - 5;
			crackedBottom.height = 10;
			// Shorten the width to not mess with the other colliders
			//platformBottom.width -= 10;
			//platformBottom.left += 5;

			//Create a collider for the right hand side of the platform
			sf::FloatRect crackedRight = crackedWallCollider->GetBounds();
			//Set it to the right of the platform -5
			crackedRight.left += crackedRight.width - 5;
			// Set our right side collider width to be 5 pixels
			crackedRight.width = 5;

			//Create a collider for the left hand side of the platform
			sf::FloatRect crackedLeft = crackedWallCollider->GetBounds();
			// Set our left side collider width to be 5 pixels
			crackedLeft.width = 5;

			// Are the feet touching the top of the platform?
			if (boxBottom.intersects(crackedTop))
			{
				// We are now touching the ground!
				m_touchingWall = true;

				//If the box is on a cracked wall, don't track its ability to fall as the box is technically always moving downwards.
				m_AmountMoved = 0;

				//Check if we are falling downward
				if (wereTouchingSurface == false && m_velocity.y > 0)
				{
					//We have touched the ground
					m_velocity.y = 0;
					m_sprite.setPosition(m_sprite.getPosition().x, crackedWallCollider->getPosition().y - m_sprite.getGlobalBounds().height);
				}

				m_isPushed = false; // The box has finally landed, therefore is should no longer run collision code //
			}

			// Is the head touching the bottom of the platform?
			if (boxTop.intersects(crackedBottom))
			{
				// We are now touching the ground!
				m_touchingWall = true;

				//Check if we are jumping
				if (wereTouchingWall == false && m_velocity.y < 0)
				{
					//We have touched the roof
					m_velocity.y = 0;
					m_sprite.setPosition(m_sprite.getPosition().x, crackedWallCollider->getPosition().y + crackedWallCollider->GetBounds().height);
				}

			}

			if (boxRight.intersects(crackedLeft))
			{
				// We are now touching the ground!
				m_touchingWall = true;

				if (wereTouchingWall == false && m_velocity.x > 0)
				{
					m_velocity.x = 0;
					m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
				}

			}

			if (boxLeft.intersects(crackedRight))
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
	}
}

bool Box::AttemptPush(sf::Vector2i _direction)
{
	// Attempt to move the box in the given direction

	// Get current position
	// Calculate target position
	sf::Vector2i targetPos = m_gridPosition + _direction;
	m_isPushed = true; //The box has moved, turn on gravity and collisions, this needs to be done AFTER we move the box//
	// Check if the space is empty

	// Get list of objects in our target position
	std::vector<GridObject*> targetCellContents = m_level->GetObjectAt(targetPos);

	// Check if any of those objects block movement
	GridObject* blocker = nullptr;
	bool blocked = false;
	for (int i = 0; i < targetCellContents.size(); ++i)
	{
		if (targetCellContents[i]->GetBlocksMovement() == true)
		{
			blocker = targetCellContents[i];
			blocked = true;
		}
	}

	// If empty, move there
	if (blocked == false)
	{
		m_boxPush.play();
		return m_level->MoveObjectTo(this, targetPos);
	}
}
