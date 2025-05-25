#include "pch.h"
#include "Game.h"
#include "PlayerCar.h"
#include <iostream>
#include "utils.h"
#include "Texture.h"
#include "PowerUp.h"

Game::Game(const Window& window)
	:BaseGame{ window },
	m_ScoreCalculator{},
	m_Lanes{},
	m_LaneNr{ 5 },
	m_BorderLineOffset{ GetViewPort().height / 40 },
	m_LaneHeight{ (GetViewPort().height - 2 * m_BorderLineOffset) / m_LaneNr },
	m_PurpleHeart{ new Texture("Purple_Heart.png") },
	m_InvinicibilityText{ new Texture("Invincible", "Seedymoteldemo-LZl4.otf", 60, Color4f{1,0,1,0.7f}) },
	m_SmallStatusText{ new Texture("Small", "Seedymoteldemo-LZl4.otf", 60, Color4f{1,0,1,0.7f}) },
	m_LifeStealStatusText{ new Texture("Lifesteal", "Seedymoteldemo-LZl4.otf", 60, Color4f{1,0,1,0.7f}) },
	m_HealText{ new Texture("Health", "Seedymoteldemo-LZl4.otf", 60, Color4f{1,0,1,0.7f}) },
	m_PlayerCar{ Vector2f{ GetViewPort().width / 5, GetViewPort().height / 2 }, m_LaneHeight * 0.95f, 
		m_LaneHeight * 0.55f, Color4f{ 0.5f, 0.12f, 0.95f, 1 }, GetViewPort().width, GetViewPort().height,
			m_SmallStatusText, m_LifeStealStatusText, m_InvinicibilityText, m_HealText },
	m_SmallLines{},
	m_ParallaxSpeed{ 1.f },
	m_SpeedKmh{ int(m_ParallaxSpeed * 40) },
	m_Pause{ new Texture("PAUSE", "Seedymoteldemo-LZl4.otf", 300, Color4f{1,0,1,1}) },
	m_Score{ new Texture(std::to_string(m_ScoreCalculator.GetScore()), "game over.ttf", 60, Color4f{1,1,1,0.6f})},
	m_Win{ new Texture("YOU WON", "Seedymoteldemo-LZl4.otf", 300, Color4f{1,0,1,1}) },
	m_Kmh{ new Texture(" km/h", "game over.ttf", 30, Color4f{1,1,1,0.6f}) },
	m_DeathTexture{ new Texture("YOU CRASHED", "Seedymoteldemo-LZl4.otf", 230, Color4f{0.9f, 0.27f, 0.55f, 1.f}) },
	m_PressRTexture{ new Texture("Press R to RESTART", "Seedymoteldemo-LZl4.otf", 80, Color4f{1,1,1,1.f}) },
	m_ScoreTextSize{ 60 },
	m_GoalTutorial{ new Texture("Reach 400 km/h", "game over.ttf", 120, Color4f{1,1,1,1.f}) },
	m_DrawGoalTutorial{ true },
	m_PauseTutorial{ new Texture("haha","game over.ttf", 60, Color4f{1,1,1,1.f}) },
	m_Paused{ false },
	m_GameWon{ false }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	std::vector<Vector2f> laneStartPositions{};

	m_Lanes.reserve(m_LaneNr);

	for (int i{}; i < m_LaneNr; ++i)
	{
		if (i == 0 || i == m_LaneNr - 1)
		{
			m_Lanes.push_back(Lane(Vector2f{ GetViewPort().width + 50.f, m_BorderLineOffset + m_LaneHeight / 2 + i * m_LaneHeight },
				m_LaneHeight * 0.95f, m_LaneHeight * 0.55f, 2.f));
		}
		else
		{
			m_Lanes.push_back(Lane(Vector2f{ GetViewPort().width + 50.f, m_BorderLineOffset + m_LaneHeight / 2 + i * m_LaneHeight },
				m_LaneHeight * 0.95f, m_LaneHeight * 0.55f, 1.7f));
		}

		if (i % 2 == 0 && i != m_LaneNr - 1)
		{
			const float smallLineWidth{ GetViewPort().width / 25 };
			const float smallLineHeight{ GetViewPort().height / 50 };

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

void Game::Cleanup()
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
	delete m_LifeStealStatusText;
	m_LifeStealStatusText = nullptr;
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

	delete m_GoalTutorial;
	m_GoalTutorial = nullptr;
	delete m_PauseTutorial;
	m_PauseTutorial = nullptr;

	m_SmallLines.clear();
}

void Game::Update( float elapsedSec )
{
	// Check keyboard state
	const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	
	if (!m_Paused)
	{
		if (!m_GameWon && m_PlayerCar.GetHealth() > 0)
		{
			for (size_t i{}; i < m_Lanes.size(); ++i)
			{
				m_Lanes[i].HandleCars(elapsedSec, m_PlayerCar, m_ParallaxSpeed);
			}
			m_ScoreCalculator.Update(elapsedSec);


			if (m_SpeedKmh < 100)
			{
				m_ParallaxSpeed += 0.06f * elapsedSec;
			}
			else if (m_SpeedKmh >= 100 && m_SpeedKmh < 200)
			{
				m_ParallaxSpeed += 0.08f * elapsedSec;
			}
			else if (m_SpeedKmh >= 200 && m_SpeedKmh < 300)
			{
				m_ParallaxSpeed += 0.1f * elapsedSec;
			}
			else if (m_SpeedKmh >= 300 && m_SpeedKmh < 400)
			{
				m_ParallaxSpeed += 0.12f * elapsedSec;
			}
			else if (m_SpeedKmh >= 400)
			{
				m_ParallaxSpeed = 400;
			}

			//std::cout << m_ParallaxSpeed << std::endl;

			m_PowerUpManager.UpdatePowerUps(elapsedSec, m_ParallaxSpeed, m_PlayerCar);

			m_PlayerCar.Update(elapsedSec, m_ParallaxSpeed, pStates, m_BorderLineOffset, GetViewPort().height - m_BorderLineOffset);
		}

		for (size_t k{}; k < m_SmallLines.size(); ++k)
		{
			m_SmallLines[k].left -= (m_ParallaxSpeed + 0.5f);

			if (m_SmallLines[k].left <= -m_SmallLines[k].width)
			{
				m_SmallLines[k].left += GetViewPort().width * 1.2f;
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
		m_SpeedKmh = int(m_ParallaxSpeed * 40);

		delete m_Score;
		if (m_SpeedKmh < 100)
		{
			m_ScoreTextSize = 60;
		}
		else if (m_SpeedKmh >= 100 && m_SpeedKmh < 200)
		{
			m_ScoreTextSize = 80;
		}
		else if (m_SpeedKmh >= 200 && m_SpeedKmh < 300)
		{
			m_ScoreTextSize = 100;
		}
		else if (m_SpeedKmh >= 300 && m_SpeedKmh < 400)
		{
			m_ScoreTextSize = 120;
		}
		else if (m_SpeedKmh >= 400)
		{
			m_ScoreTextSize = 150;
		}

		m_Score = new Texture(std::to_string(m_SpeedKmh), "game over.ttf", m_ScoreTextSize, Color4f{1,1,1,0.7f});
		scoreAcc -= 0.5f;
	}

	if (Lane::m_CarsBroken == 10)
	{
		m_PlayerCar.IncreaseHealth(1);
		Lane::m_CarsBroken -= 10;
	}

	if (m_DrawGoalTutorial)
	{
		static float goalAcc{ 0.f };
		goalAcc += elapsedSec;

		if (goalAcc >= 3.f)
		{
			m_DrawGoalTutorial = false;
			goalAcc -= 3.f;
		}
	}
}

void Game::Draw( ) const
{
	ClearBackground( );
	
	for (size_t i{}; i < m_Lanes.size(); ++i)
	{
		m_Lanes[i].Draw();

		if (i == 1 || i == m_Lanes.size() - 2)
		{
			utils::SetColor(Color4f{ 1,0,1,1 });
			utils::DrawLine(Vector2f{ 0.f, m_Lanes[i].GetDrawPosition().y + m_LaneHeight / 2}, Vector2f{ GetViewPort().width, m_Lanes[i].GetDrawPosition().y + m_LaneHeight / 2 }, 6);
		}
	}

	for (size_t k{}; k < m_SmallLines.size(); ++k)
	{
		float smallLineX{ m_SmallLines[k].left};

		utils::SetColor(Color4f{ 1,0,1,1 });
		utils::FillRect(smallLineX, m_SmallLines[k].bottom, m_SmallLines[k].width, m_SmallLines[k].height);
	}

	utils::SetColor(Color4f{ 1,0,1,1 });
	utils::DrawLine(Vector2f{ 0.f, m_BorderLineOffset }, Vector2f{ GetViewPort().width, m_BorderLineOffset }, 8);
	utils::DrawLine(Vector2f{ 0.f, GetViewPort().height - m_BorderLineOffset }, Vector2f{ GetViewPort().width, GetViewPort().height - m_BorderLineOffset }, 8);

	m_PlayerCar.Draw(Vector2f{ 20.f, GetViewPort().height - 100.f });

	m_PowerUpManager.DrawPowerUps();

	if (m_PlayerCar.GetHealth() <= 0)
	{
		m_DeathTexture->Draw(Vector2f{ GetViewPort().width / 21, GetViewPort().height / 3 });
		m_PressRTexture->Draw(Vector2f{ GetViewPort().width / 3.6f, GetViewPort().height / 6 });
	}
	else if (m_GameWon)
	{
		m_Win->Draw(Vector2f{ GetViewPort().width / 10, 50.f });
	}
	else if (m_Paused)
	{
		m_Pause->Draw(Vector2f{ GetViewPort().width / 4.5f, 50.f});
	}

	m_Score->Draw(Vector2f{ GetViewPort().width - m_ScoreTextSize * 3, GetViewPort().height - m_ScoreTextSize });
	m_Kmh->Draw(Vector2f{ GetViewPort().width - m_ScoreTextSize * 3 + m_Score->GetWidth(), GetViewPort().height - m_ScoreTextSize });

	for (int i{}; i < m_PlayerCar.GetHealth(); ++i)
	{
		m_PurpleHeart->Draw(Rectf{ GetViewPort().width / 2 - (m_PlayerCar.GetHealth() * m_PurpleHeart->GetWidth() / 2 + (m_PlayerCar.GetHealth() - 1) * m_PurpleHeart->GetWidth() * 2) / 2 +
			i * (m_PurpleHeart->GetWidth() * 2 * 1.5f),
			GetViewPort().height / 12,
			m_PurpleHeart->GetWidth() * 2, m_PurpleHeart->GetHeight() * 2});
	}

	if (m_DrawGoalTutorial)
	{
		m_GoalTutorial->Draw(Vector2f{ GetViewPort().width / 5, GetViewPort().height / 2 });
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	switch ( e.keysym.sym )
	{
	case SDLK_ESCAPE:
		if (!m_DrawGoalTutorial)
		{
			if (!m_Paused)
			{
				m_Paused = true;
			}
			else m_Paused = false;
		}
		break;
	case SDLK_r:
		Restart();
		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.2f, 0.1f, 0.3f, 1.f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::Restart()
{
	m_PlayerCar.RestartPlayer(Vector2f{ GetViewPort().width / 5, GetViewPort().height / 2 });
	m_ParallaxSpeed = 1.f;
	m_SpeedKmh = int(m_ParallaxSpeed * 40);
	m_DrawGoalTutorial = true;
	for (size_t i{}; i < m_Lanes.size(); ++i)
	{
		m_Lanes[i].ClearLane();
	}
	m_PowerUpManager.ClearPowerUps();
}
