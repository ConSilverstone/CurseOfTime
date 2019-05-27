//Project includes
#include "Collagen.h"
#include "Framework/AssetManager.h"

Collagen::Collagen()
  : m_player(nullptr)
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/CollagenInactive.png"));
	m_sprite.setPosition(300, sf::VideoMode::getDesktopMode().height - 100);
}

void Collagen::Update(sf::Time _frameTime)
{
	// Create a local variable based on the function from player.
	int PotionState = m_player->GetPotionState();
	// If the player has selected collagen (represented by 1), then change image.
	if (PotionState == 1)
	{
		m_sprite.setTexture(AssetManager::GetTexture("graphics/Collagen.png"));
	}
	else
	{
		m_sprite.setTexture(AssetManager::GetTexture("graphics/CollagenInactive.png"));
	}
}

void Collagen::SetPlayer(Player* _player)
{
	m_player = _player;
}