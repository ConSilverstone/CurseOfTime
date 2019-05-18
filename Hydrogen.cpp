//Project includes
#include "Hydrogen.h"
#include "Framework/AssetManager.h"

Hydrogen::Hydrogen()
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/HighSpike.png"));
}

void Hydrogen::Update(sf::Time _frameTime)
{

}