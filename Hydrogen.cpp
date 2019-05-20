//Project includes
#include "Hydrogen.h"
#include "Framework/AssetManager.h"

Hydrogen::Hydrogen()
	: m_player(nullptr)
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/Hydrogen.png"));
	m_sprite.setPosition(510, sf::VideoMode::getDesktopMode().height - 100);
}

void Hydrogen::Update(sf::Time _frameTime)
{

}

void Hydrogen::SetPlayer(Player* _player)
{
	m_player = _player;
}