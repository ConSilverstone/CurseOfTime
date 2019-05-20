#include "Potion.h"
#include "Player.h"
#include "Framework/AssetManager.h"

// Constants
#define xVelocity 500.0f
#define yVelocity -300.0f
#define GRAVITY 200.0f

Potion::Potion()
	: GridObject()
	, m_player(nullptr)
{
	PotionEffects(none);
	m_sprite.setTexture(AssetManager::GetTexture("graphics/PotionVial.png"));
	m_blocksMovement = true;
}

void Potion::Update(sf::Time _frameTime)
{
	if (m_player != nullptr)
	{
		if (PotionEffects(Sulphur) && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			m_velocity.x = xVelocity;
			m_velocity.y = yVelocity;
		}
	}
	//Apply gravity to our velocity
		//float velocityChange = (GRAVITY * _frameTime.asSeconds()) * 2;
		//m_velocity.y += velocityChange;

	//Move sprite base on velocity
	//sf::Vector2f positionChange = m_velocity * _frameTime.asSeconds();

	//// Call the update function manually on 
	//// the parent class
	//// This will actually move the character

	GridObject::Update(_frameTime);
}

void Potion::Collide(GameObject & _collider)
{
}

void Potion::SetPlayer(Player * _player)
{

}
