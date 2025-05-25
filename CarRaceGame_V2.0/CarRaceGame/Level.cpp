#include "pch.h"
#include "Level.h"
#include "Texture.h"
#include "utils.h"

Level::Level(const Rectf& viewPort)
	: m_ViewPort{ viewPort },
	m_ScoreCalculator{},
	m_Lanes{},
	m_LaneNr{ 5 },
	m_BorderLineOffset{ m_ViewPort.height / 40 },
	m_LaneHeight{ (m_ViewPort.height - 2 * m_BorderLineOffset) / m_LaneNr },
	m_PurpleHeart{ new Texture("Purple_Heart.png") },
	m_InvinicibilityText{ new Texture("Invincible", "Seedymoteldemo-LZl4.otf", 60, Color4f{1,0,1,0.7f}) },
	m_SmallStatusText{ new Texture("Small", "Seedymoteldemo-LZl4.otf", 60, Color4f{1,0,1,0.7f}) },
	m_UnbreakableStatusText{ new Texture("Unbreakable", "Seedymoteldemo-LZl4.otf", 60, Color4f{1,0,1,0.7f}) },
	m_HealText{ new Texture("Health", "Seedymoteldemo-LZl4.otf", 60, Color4f{1,0,1,0.7f}) },
	m_PlayerCar{ Vector2f{ m_ViewPort.width / 5,m_ViewPort.height / 2 }, m_LaneHeight * 0.95f,
		m_LaneHeight * 0.55f, Color4f{ 0.5f, 0.12f, 0.95f, 1 }, m_ViewPort.width, m_ViewPort.height,
			m_SmallStatusText, m_UnbreakableStatusText, m_InvinicibilityText, m_HealText },
	m_SmallLines{},
	m_ParallaxSpeed{ 1.f },
	m_Pause{ new Texture("PAUSE", "Seedymoteldemo-LZl4.otf", 300, Color4f{1,0,1,1}) },
	m_Score{ new Texture(std::to_string(m_ScoreCalculator.GetScore()), "game over.ttf", 60, Color4f{1,1,1,0.6f}) },
	m_Win{ new Texture("YOU WON", "Seedymoteldemo-LZl4.otf", 300, Color4f{1,0,1,1}) },
	m_Kmh{ new Texture(" km/h", "game over.ttf", 30, Color4f{1,1,1,0.6f}) },
	m_DeathTexture{ new Texture("YOU CRASHED", "Seedymoteldemo-LZl4.otf", 230, Color4f{0.9f, 0.27f, 0.55f, 1.f}) },
	m_PressRTexture{ new Texture("Press R to RESTART", "Seedymoteldemo-LZl4.otf", 80, Color4f{1,1,1,1.f}) },
	m_ScoreTextSize{ 60 },
	m_Paused{ false },
	m_GameWon{ false }
{
}

Level::Level(Level&& other) noexcept
	:m_ViewPort{ std::move(other.m_ViewPort) },
	m_ScoreCalculator{ std::move(other.m_ScoreCalculator) },
	m_LaneNr{ std::move(other.m_LaneNr) },
	m_BorderLineOffset{ std::move(other.m_BorderLineOffset) },
	m_LaneHeight{ std::move(other.m_LaneHeight) },
	m_PurpleHeart{ other.m_PurpleHeart },
	m_InvinicibilityText{ other.m_InvinicibilityText },
	m_SmallStatusText{ other.m_SmallStatusText },
	m_UnbreakableStatusText{ other.m_UnbreakableStatusText },
	m_HealText{ other.m_HealText },
	m_PlayerCar{ std::move(other.m_PlayerCar) },
	m_ParallaxSpeed{ std::move(other.m_ParallaxSpeed) },
	m_Pause{ other.m_Pause },
	m_Score{ other.m_Score },
	m_Win{ other.m_Win },
	m_Kmh{ other.m_Kmh },
	m_DeathTexture{ other.m_DeathTexture },
	m_PressRTexture{ other.m_PressRTexture },
	m_ScoreTextSize{ std::move(other.m_ScoreTextSize) },
	m_Paused{ std::move(other.m_Paused) },
	m_GameWon{ std::move(other.m_GameWon)}
{
	for (size_t i{}; i < other.m_Lanes.size(); ++i)
	{
		m_Lanes.push_back(std::move(other.m_Lanes[i]));
	}
	for (size_t j{}; j < other.m_SmallLines.size(); ++j)
	{
		m_SmallLines.push_back(std::move(other.m_SmallLines[j]));
	}
	other.m_PurpleHeart = nullptr;
	other.m_InvinicibilityText = nullptr;
	other.m_SmallStatusText = nullptr;
	other.m_UnbreakableStatusText = nullptr;
	other.m_HealText = nullptr;
	other.m_Pause = nullptr;
	other.m_Score = nullptr;
	other.m_Win = nullptr;
	other.m_Kmh = nullptr;
	other.m_DeathTexture = nullptr;
	other.m_PressRTexture = nullptr;
}

Level::~Level()
{
	delete m_Pause;
	m_Pause = nullptr;
	delete m_Score;
	m_Score = nullptr;
	delete m_Win;
	m_Win = nullptr;

	delete m_InvinicibilityText;
	m_InvinicibilityText = nullptr;
	delete m_SmallStatusText;
	m_SmallStatusText = nullptr;
	delete m_UnbreakableStatusText;
	m_UnbreakableStatusText = nullptr;
	delete m_HealText;
	m_HealText = nullptr;
	delete m_Kmh;
	m_Kmh = nullptr;
	delete m_DeathTexture;
	m_DeathTexture = nullptr;
	delete m_PressRTexture;
	m_PressRTexture = nullptr;
	delete m_PurpleHeart;
	m_PurpleHeart = nullptr;

	m_SmallLines.clear();
}

void Level::IntializeLevel()
{
	std::vector<Vector2f> laneStartPositions{};

	m_Lanes.reserve(m_LaneNr);

	for (int i{}; i < m_LaneNr; ++i)
	{
		if (i == 0 || i == m_LaneNr - 1)
		{
			m_Lanes.push_back(Lane(Vector2f{ m_ViewPort.width + 50.f, m_BorderLineOffset + m_LaneHeight / 2 + i * m_LaneHeight },
				m_LaneHeight * 0.95f, m_LaneHeight * 0.55f, 1.5f));
		}
		else
		{
			m_Lanes.push_back(Lane(Vector2f{ m_ViewPort.width + 50.f, m_BorderLineOffset + m_LaneHeight / 2 + i * m_LaneHeight },
				m_LaneHeight * 0.95f, m_LaneHeight * 0.55f, 1.5f));
		}

		if (i % 2 == 0 && i != m_LaneNr - 1)
		{
			const float smallLineWidth{ m_ViewPort.width / 25 };
			const float smallLineHeight{ m_ViewPort.height / 50 };

			for (int k{}; k < 18; ++k)
			{
				m_SmallLines.push_back(Rectf{ 0.f + k * (smallLineWidth * 2.f), m_Lanes[i].GetDrawPosition().y + m_LaneHeight / 2, smallLineWidth, smallLineHeight });
			}
		}

		laneStartPositions.push_back(m_Lanes[i].GetDrawPosition());
	}


	m_PowerUpManager = PowerUpManager(laneStartPositions);

	m_SmallLines.reserve(20);
}

void Level::UdateLevel(float elapsedSec, const Uint8* pStates)
{
	if (!m_Paused)
	{
		if (!m_GameWon && m_PlayerCar.GetHealth() > 0)
		{
			for (size_t i{}; i < m_Lanes.size(); ++i)
			{
				m_Lanes[i].HandleCars(elapsedSec, m_PlayerCar, m_ParallaxSpeed);
			}

			m_PlayerCar.Update(elapsedSec, m_ParallaxSpeed, pStates, m_BorderLineOffset, m_ViewPort.height - m_BorderLineOffset);

			m_ScoreCalculator.Update(elapsedSec);

			m_ParallaxSpeed += 0.06f * elapsedSec;

			//std::cout << m_ParallaxSpeed << std::endl;

			m_PowerUpManager.UpdatePowerUps(elapsedSec, m_ParallaxSpeed, m_PlayerCar);
		}

		for (size_t k{}; k < m_SmallLines.size(); ++k)
		{
			m_SmallLines[k].left -= (m_ParallaxSpeed + 0.5f);

			if (m_SmallLines[k].left <= -m_SmallLines[k].width)
			{
				m_SmallLines[k].left += m_ViewPort.width * 1.2f;
			}
		}
	}

	if (m_ParallaxSpeed >= 10.f)
	{
		m_ParallaxSpeed = 10.f;
		m_GameWon = true;
	}



	static float scoreAcc{ 0.f };
	scoreAcc += elapsedSec;
	if (scoreAcc >= 0.5f)
	{
		int speed{ int(m_ParallaxSpeed * 40) };

		delete m_Score;
		if (speed < 100)
		{
			m_ScoreTextSize = 60;
		}
		else if (speed >= 100 && speed < 200)
		{
			m_ScoreTextSize = 80;
		}
		else if (speed >= 200 && speed < 300)
		{
			m_ScoreTextSize = 100;
		}
		else if (speed >= 300)
		{
			m_ScoreTextSize = 120;
		}
		else if (speed >= 400)
		{
			m_ScoreTextSize = 150;
		}
		else if (speed >= 500)
		{
			m_ScoreTextSize = 180;
		}

		m_Score = new Texture(std::to_string(speed), "game over.ttf", m_ScoreTextSize, Color4f{ 1,1,1,0.7f });
		scoreAcc -= 0.5f;
	}

	if (Lane::m_CarsBroken == 10)
	{
		m_PlayerCar.IncreaseHealth(1);
		Lane::m_CarsBroken -= 10;
	}
}

void Level::DrawLevel() const
{
	for (size_t i{}; i < m_Lanes.size(); ++i)
	{
		m_Lanes[i].Draw();

		if (i == 1 || i == m_Lanes.size() - 2)
		{
			utils::SetColor(Color4f{ 1,0,1,1 });
			utils::DrawLine(Vector2f{ 0.f, m_Lanes[i].GetDrawPosition().y + m_LaneHeight / 2 }, Vector2f{ m_ViewPort.width, m_Lanes[i].GetDrawPosition().y + m_LaneHeight / 2 }, 6);
		}
	}

	for (size_t k{}; k < m_SmallLines.size(); ++k)
	{
		float smallLineX{ m_SmallLines[k].left };

		utils::SetColor(Color4f{ 1,0,1,1 });
		utils::FillRect(smallLineX, m_SmallLines[k].bottom, m_SmallLines[k].width, m_SmallLines[k].height);
	}

	utils::SetColor(Color4f{ 1,0,1,1 });
	utils::DrawLine(Vector2f{ 0.f, m_BorderLineOffset }, Vector2f{ m_ViewPort.width, m_BorderLineOffset }, 8);
	utils::DrawLine(Vector2f{ 0.f, m_ViewPort.height - m_BorderLineOffset }, Vector2f{ m_ViewPort.width, m_ViewPort.height - m_BorderLineOffset }, 8);

	m_PlayerCar.Draw(Vector2f{ 20.f, m_ViewPort.height - 100.f });

	if (m_PlayerCar.GetHealth() <= 0)
	{
		m_DeathTexture->Draw(Vector2f{ m_ViewPort.width / 21, m_ViewPort.height / 3 });
		m_PressRTexture->Draw(Vector2f{ m_ViewPort.width / 3.6f, m_ViewPort.height / 6 });
	}
	else if (m_GameWon)
	{
		m_Win->Draw(Vector2f{ m_ViewPort.width / 10, 50.f });
	}
	else if (m_Paused)
	{
		m_Pause->Draw(Vector2f{ m_ViewPort.width / 4.5f, 50.f });
	}

	m_Score->Draw(Vector2f{ m_ViewPort.width - m_ScoreTextSize * 3, m_ViewPort.height - m_ScoreTextSize });
	m_Kmh->Draw(Vector2f{ m_ViewPort.width - m_ScoreTextSize * 3 + m_Score->GetWidth(), m_ViewPort.height - m_ScoreTextSize });

	m_PowerUpManager.DrawPowerUps();

	for (int i{}; i < m_PlayerCar.GetHealth(); ++i)
	{
		m_PurpleHeart->Draw(Rectf{ m_ViewPort.width / 2 - (m_PlayerCar.GetHealth() * m_PurpleHeart->GetWidth() / 2 + (m_PlayerCar.GetHealth() - 1) * m_PurpleHeart->GetWidth() * 2) / 2 +
			i * (m_PurpleHeart->GetWidth() * 2 * 1.5f),
			m_ViewPort.height / 12,
			m_PurpleHeart->GetWidth() * 2, m_PurpleHeart->GetHeight() * 2 });
	}
}

void Level::HandlePause()
{
	if (!m_Paused)
	{
		m_Paused = true;
	}
	else m_Paused = false;
}
