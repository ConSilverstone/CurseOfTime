//Project includes
#include "Collagen.h"
#include "Framework/AssetManager.h"

Collagen::Collagen()
  : m_player(nullptr)
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/Collagen.png"));
	m_sprite.setPosition(300, sf::VideoMode::getDesktopMode().height - 100);
}

void Collagen::Update(sf::Time _frameTime)
{

}

void Collagen::SetPlayer(Player* _player)
{
	m_player = _player;
}