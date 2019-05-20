#include "Timer.h"
#include "Framework/AssetManager.h"

Timer::Timer()
	: TextObject()
	, m_player(nullptr)
{
	m_text.setFont(AssetManager::GetFont("fonts/mainFont.ttf"));
	m_text.setString("Timer: 0");
	m_text.setPosition((sf::VideoMode::getDesktopMode().width / 2) - 50, 100);
}

void Timer::Update(sf::Time _frameTime)
{
	if (m_player != nullptr)
	{
		int timer = m_player->GetTimer();
		m_text.setString("Timer: " + std::to_string(timer));
	}
}

void Timer::SetPlayer(Player* _player)
{
	m_player = _player;
}

