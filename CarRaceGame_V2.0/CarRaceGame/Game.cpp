#include "pch.h"
#include "Game.h"
#include "PlayerCar.h"
#include <iostream>
#include "utils.h"
#include "Texture.h"

Game::Game(const Window& window)
	:BaseGame{ window },
	m_UIManager{},
	m_Lanes{},
	m_LaneNr{ 5 },
	m_BorderLineOffset{ GetViewPort().height / 40 },
	m_LaneHeight{ (GetViewPort().height - 2 * m_BorderLineOffset) / m_LaneNr },
	m_PlayerCar{ new PlayerCar(Vector2f{ GetViewPort().width / 5, GetViewPort().height / 2 }, m_LaneHeight * 0.95f, 
		m_LaneHeight * 0.55f, Color4f{ 0.5f, 0.12f, 0.95f, 1 }, GetViewPort().width, GetViewPort().height) },
	m_SmallLines{},
	m_ParallaxSpeed{ 1.f },
	m_Pause{ new Texture("LOL XD", "Seedymoteldemo-LZl4.otf", 300, Color4f{1,0,1,1}) },
	m_Score{ new Texture(std::to_string(m_UIManager.GetScore()), "game over.ttf", 60, Color4f{1,1,1,1}) },
	m_Win{ new Texture("YOU WON", "Seedymoteldemo-LZl4.otf", 300, Color4f{1,0,1,1}) },
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
	m_Lanes.reserve(m_LaneNr);

	for (int i{}; i < m_LaneNr; ++i)
	{
		if (i == 0 || i == m_LaneNr - 1)
		{
			m_Lanes.push_back(Lane(Vector2f{ GetViewPort().width + 50.f, m_BorderLineOffset + m_LaneHeight / 2 + i * m_LaneHeight },
				m_LaneHeight * 0.95f, m_LaneHeight * 0.55f, 1.5f));
		}
		else
		{
			m_Lanes.push_back(Lane(Vector2f{ GetViewPort().width + 50.f, m_BorderLineOffset + m_LaneHeight / 2 + i * m_LaneHeight },
				m_LaneHeight * 0.95f, m_LaneHeight * 0.55f, 1.5f));
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
	}

	m_SmallLines.reserve(20);
	
}

void Game::Cleanup()
{
	delete m_PlayerCar;
	m_PlayerCar = nullptr;
	delete m_Pause;
	m_Pause = nullptr;
	delete m_Score;
	m_Score = nullptr;
	delete m_Win;
	m_Win = nullptr;

	m_SmallLines.clear();
}

void Game::Update( float elapsedSec )
{
	// Check keyboard state
	const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	
	if (!m_GameWon && !m_Paused)
	{
		for (size_t i{}; i < m_Lanes.size(); ++i)
		{
			m_Lanes[i].HandleCars(elapsedSec, m_PlayerCar, m_ParallaxSpeed);
		}

		m_PlayerCar->Update(elapsedSec, m_ParallaxSpeed, pStates, m_BorderLineOffset, GetViewPort().height - m_BorderLineOffset);
		
		m_UIManager.Update(elapsedSec);

		m_ParallaxSpeed += 0.06f * elapsedSec;

		std::cout << m_ParallaxSpeed << std::endl;
	}

	if (m_ParallaxSpeed >= 9.f)
	{
		m_ParallaxSpeed = 9.f;
		m_GameWon = true;
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

	m_PlayerCar->Draw();

	m_Score->Draw(Vector2f{ GetViewPort().width - 120.f, GetViewPort().height - 70.f });

	if (m_GameWon)
	{
		m_Win->Draw(Vector2f{ GetViewPort().width / 10, 50.f });
	}
	else if (m_Paused)
	{
		m_Pause->Draw(Vector2f{ GetViewPort().width / 4.5f, 50.f});
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
		if (!m_Paused)
		{
			m_Paused = true;
		}
		else m_Paused = false;
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
