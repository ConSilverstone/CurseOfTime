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
	, m_Explosion()
{
	PotionEffects(none);
	m_sprite.setTexture(AssetManager::GetTexture("graphics/PotionVial.png"));
	m_Explosion.setBuffer(AssetManager::GetSoundBuffer("audio/Explosion.wav"));
	m_sprite.setPosition(0, 0);
	m_blocksMovement = true;
}

void Potion::Update(sf::Time _frameTime)
{
	if (m_player != nullptr)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			// What way is the player facing?
			if (m_player->GetPlayerDirectionRight() == true)
			{
				// He is facing to the right so throw the potion to the right.
				m_velocity.x = xVelocity;
				m_velocity.y = yVelocity;
			}
			if (m_player->GetPlayerDirectionRight() == false)
			{
				// He is facing to the left so throw the potion to the left.
				m_velocity.x = -xVelocity;
				m_velocity.y = yVelocity;
			}
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
	// WALLS /////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////

	// If it was a wall we hit, we need to more ourselves
	// outside the wall's bounds, aka back where we were
	if (wallCollider != nullptr)
	{
		m_Explosion.play();
		m_level->deletePotionAt(this);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// CRACKED WALLS /////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////

	// If it was a wall we hit, we need to more ourselves
	// outside the wall's bounds, aka back where we were
	if (crackedCollider != nullptr)
	{
		m_Explosion.play();
		m_level->deleteObjectAt(crackedCollider);
		m_level->deletePotionAt(this);
	}
}

void Potion::SetPlayer(Player * _player)
{
	m_player = _player;
}