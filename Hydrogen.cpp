//Project includes
#include "Hydrogen.h"
#include "Framework/AssetManager.h"

Hydrogen::Hydrogen()
	: m_player(nullptr)
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/HydrogenInactive.png"));
	m_sprite.setPosition(510, sf::VideoMode::getDesktopMode().height - 100);
}

void Hydrogen::Update(sf::Time _frameTime)
{
	// Create a local variable based on the function from player.
	int PotionState = m_player->GetPotionState();
	// If the player has selected collagen (represented by 1), then change image.
	if (PotionState == 4)
	{
		m_sprite.setTexture(AssetManager::GetTexture("graphics/Hydrogen.png"));
	}
	else
	{
		m_sprite.setTexture(AssetManager::GetTexture("graphics/HydrogenInactive.png"));
	}
}

void Hydrogen::SetPlayer(Player* _player)
{
	m_player = _player;
}