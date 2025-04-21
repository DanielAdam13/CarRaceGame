#include "pch.h"
#include "Game.h"
#include "PlayerCar.h"
#include <iostream>

Game::Game( const Window& window ) 
	:BaseGame{ window },
	m_Cars{},
	m_PlayerCar{}
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_PlayerCar = new PlayerCar(Vector2f{ GetViewPort().width / 5, GetViewPort().height / 2 }, GetViewPort().width / 15, 
		GetViewPort().height / 15, Color4f{ 0.5f, 0.12f, 0.95f, 1 }, GetViewPort().width, GetViewPort().height);

	for (int i{}; i < 4; ++i)
	{
		m_Cars.push_back(new Car(Vector2f{ GetViewPort().width * 0.8f, GetViewPort().height / 7 + i * GetViewPort().height / 4 }, 
			GetViewPort().width / 15, GetViewPort().height / 15));
	}
}

void Game::Cleanup( )
{
	delete m_PlayerCar;
	m_PlayerCar = nullptr;

	for (Car* pCar : m_Cars)
	{
		if (pCar != nullptr)
		{
			delete pCar;
			pCar = nullptr;
		}
		
	}
	m_Cars.clear();
}

void Game::Update( float elapsedSec )
{
	// Check keyboard state
	const Uint8 *pStates = SDL_GetKeyboardState( nullptr );

	for (size_t i{};i<m_Cars.size();++i)
	{
		if (m_Cars[i] != nullptr)
		{
			m_Cars[i]->Update(elapsedSec);

			if (m_Cars[i]->GetBounds().left < -m_Cars[i]->GetBounds().width)
			{
				delete m_Cars[i];
				m_Cars[i] = nullptr;
			}
		}
	}
	m_PlayerCar->Update(pStates, elapsedSec);

}

void Game::Draw( ) const
{
	ClearBackground( );
	for (const Car* pCar : m_Cars) 
	{
		if (pCar != nullptr)
		{
			pCar->Draw();
		}
		
	}
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
