//Project includes
#include "Electricity.h"
#include "Framework/AssetManager.h"

Electricity::Electricity()
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/Electricity.png"));
	m_sprite.setPosition(370, sf::VideoMode::getDesktopMode().height - 100);
}

void Electricity::Update(sf::Time _frameTime)
{

}

void Electricity::SetPlayer(Player* _player)
{
	m_player = _player;
}
