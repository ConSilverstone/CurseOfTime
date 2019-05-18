#include "Potion.h"
#include "Framework/AssetManager.h"

Potion::Potion()
	: GridObject()
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/player/playerStandLeft.png"));
	m_blocksMovement = true;
}

void Potion::Update(sf::Time _frameTime)
{
}

void Potion::Collide(GameObject & _collider)
{
}
