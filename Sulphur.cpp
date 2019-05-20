//Project includes
#include "Sulphur.h"
#include "Potion.h"
#include "Framework/AssetManager.h"

Sulphur::Sulphur()
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/Sulphur.png"));
	m_sprite.setPosition(440, sf::VideoMode::getDesktopMode().height - 100);
}

void Sulphur::Update(sf::Time _frameTime)
{
	GridObject::Update(_frameTime);
}

void Sulphur::SetPlayer(Player* _player)
{
	m_player = _player;
}