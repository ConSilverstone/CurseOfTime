//Project includes
#include "Electricity.h"
#include "Framework/AssetManager.h"

Electricity::Electricity()
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/HighSpike.png"));
}

void Electricity::Update(sf::Time _frameTime)
{

}
