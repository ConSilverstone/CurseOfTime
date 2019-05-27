//Project includes
#include "Electricity.h"
#include "Framework/AssetManager.h"

Electricity::Electricity()
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/ElectricityInactive.png"));
	m_sprite.setPosition(370, sf::VideoMode::getDesktopMode().height - 100);
}

void Electricity::Update(sf::Time _frameTime)
{
	// Create a local variable based on the function from player.
	int PotionState = m_player->GetPotionState();
	// If the player has selected collagen (represented by 1), then change image.
	if (PotionState == 2)
	{
		m_sprite.setTexture(AssetManager::GetTexture("graphics/Electricity.png"));
	}
	else
	{
		m_sprite.setTexture(AssetManager::GetTexture("graphics/ElectricityInactive.png"));
	}
}

void Electricity::SetPlayer(Player* _player)
{
	m_player = _player;
}
