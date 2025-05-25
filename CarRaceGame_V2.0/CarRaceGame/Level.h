#pragma once
#include <vector>
#include "ScoreCalculator.h"
#include "Lane.h"
#include "PowerUpManager.h"
#include "PlayerCar.h"

class Texture;

class Level
{
public:
	explicit Level(const Rectf& viwePort);
	Level(const Level& other) = delete;
	Level& operator=(const Level& rhs) = delete;
	Level(Level&& other) noexcept;
	Level& operator=(Level&& rhs) = delete;
	~Level();

	void IntializeLevel();
	void UdateLevel(float elapsedSec, const Uint8* pStates);
	void DrawLevel() const;

	void HandlePause();

private:
	// VARIABLES
	const Rectf m_ViewPort;
	ScoreCalculator m_ScoreCalculator;

	std::vector<Lane> m_Lanes;
	PowerUpManager m_PowerUpManager;

	int m_LaneNr;
	const float m_BorderLineOffset;
	const float m_LaneHeight;

	const Texture* m_PurpleHeart;

	const Texture* m_InvinicibilityText;
	const Texture* m_SmallStatusText;
	const Texture* m_UnbreakableStatusText;
	const Texture* m_HealText;
	PlayerCar m_PlayerCar;

	std::vector<Rectf> m_SmallLines;
	float m_ParallaxSpeed;

	Texture* m_Pause;
	Texture* m_Score;
	const Texture* m_Kmh;
	const Texture* m_DeathTexture;
	const Texture* m_PressRTexture;
	Texture* m_Win;

	bool m_Paused;
	bool m_GameWon;

	int m_ScoreTextSize;
};

