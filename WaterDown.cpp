#include "WaterDown.h"
#include "Framework/AssetManager.h"

WaterDown::WaterDown()
	: m_animationSystem()
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/WaterDown.png"));
	m_blocksMovement = true;

	// Set up animation
	m_animationSystem.SetSprite(m_sprite);

	// Create individual animations
	Animation& standDown = m_animationSystem.CreateAnimation("standDown");
	standDown.AddFrame(AssetManager::GetTexture("graphics/PlayerWalkDown1.png"));

	Animation& runDown = m_animationSystem.CreateAnimation("runDown");
	runDown.AddFrame(AssetManager::GetTexture("graphics/WaterDown.png"));
	runDown.AddFrame(AssetManager::GetTexture("graphics/WaterDown2.png"));
	runDown.SetPlayBackSpeed(5);
	runDown.SetLoop(true);

	m_animationSystem.Play("runDown");
}

void WaterDown::Update(sf::Time _frameTime)
{
	//Process Animations
	m_animationSystem.Update(_frameTime);
}
