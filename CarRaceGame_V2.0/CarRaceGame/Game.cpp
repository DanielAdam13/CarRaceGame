#include "pch.h"
#include "Game.h"
#include "PlayerCar.h"
#include <iostream>
#include "utils.h"

Game::Game(const Window& window)
	:BaseGame{ window },
	m_PlayerCar{ new PlayerCar(Vector2f{ GetViewPort().width / 5, GetViewPort().height / 2 }, GetViewPort().width / 10,
		GetViewPort().height / 10, Color4f{ 0.5f, 0.12f, 0.95f, 1 }, GetViewPort().width, GetViewPort().height) },
	m_Lanes{},
	m_LaneNr{ 5 },
	m_BorderLineOffset{ GetViewPort().height / 40 },
	m_LaneHeight{ (GetViewPort().height - 2 * m_BorderLineOffset) / m_LaneNr }
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
				m_LaneHeight * 0.95f, m_LaneHeight * 0.6f, 2.f));
		}
	}
}

void Game::Cleanup()
{
	delete m_PlayerCar;
	m_PlayerCar = nullptr;
}

void Game::Update( float elapsedSec )
{
	// Check keyboard state
	const Uint8 *pStates = SDL_GetKeyboardState( nullptr );

	for (size_t i{}; i < m_Lanes.size(); ++i)
	{
		m_Lanes[i].HandleCars(elapsedSec);
	}
	
	m_PlayerCar->Update(pStates, elapsedSec, m_BorderLineOffset, GetViewPort().height - m_BorderLineOffset);

}

void Game::Draw( ) const
{
	ClearBackground( );
	
	for (size_t i{}; i < m_Lanes.size(); ++i)
	{
		m_Lanes[i].Draw();
	}


	utils::SetColor(Color4f{ 1,0,1,1 });
	utils::DrawLine(Vector2f{ 0.f, m_BorderLineOffset }, Vector2f{ GetViewPort().width, m_BorderLineOffset }, 3);
	utils::DrawLine(Vector2f{ 0.f, GetViewPort().height - m_BorderLineOffset }, Vector2f{ GetViewPort().width, GetViewPort().height - m_BorderLineOffset }, 3);

	m_PlayerCar->Draw();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
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
