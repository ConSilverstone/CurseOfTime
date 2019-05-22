#include "Delay.h"
#include "Framework/AssetManager.h"

Delay::Delay()
	: TextObject()
	, m_player(nullptr)
{
	m_text.setFont(AssetManager::GetFont("fonts/mainFont.ttf"));
	m_text.setString("Cooldown: 0");
	m_text.setPosition(300, sf::VideoMode::getDesktopMode().height - 150);
}

void Delay::Update(sf::Time _frameTime)
{
	if (m_player != nullptr)
	{
		int delay = m_player->GetDelay();
		m_text.setString("Cooldown: " + std::to_string(delay));
	}
}

void Delay::SetPlayer(Player* _player)
{
	m_player = _player;
}
