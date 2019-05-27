//Project includes
#include "Sulphur.h"
#include "Potion.h"
#include "Framework/AssetManager.h"

Sulphur::Sulphur()
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/SulphurInactive.png"));
	m_sprite.setPosition(440, sf::VideoMode::getDesktopMode().height - 100);
}

void Sulphur::Update(sf::Time _frameTime)
{
	// Create a local variable based on the function from player.
	int PotionState = m_player->GetPotionState();
	// If the player has selected collagen (represented by 1), then change image.
	if (PotionState == 3)
	{
		m_sprite.setTexture(AssetManager::GetTexture("graphics/Sulphur.png"));
	}
	else
	{
		m_sprite.setTexture(AssetManager::GetTexture("graphics/SulphurInactive.png"));
	}
}

void Sulphur::SetPlayer(Player* _player)
{
	m_player = _player;
}