#pragma once
#include "BaseGame.h"
#include <vector>
#include "Lane.h"
class PlayerCar;
class Texture;
#include "UIManager.h"

class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:
	// variables
	UIManager m_UIManager;
	
	std::vector<Lane> m_Lanes;
	const int m_LaneNr;
	const float m_BorderLineOffset;
	const float m_LaneHeight;
	PlayerCar* m_PlayerCar;

	std::vector<Rectf> m_SmallLines;
	float m_ParallaxSpeed;

	Texture* m_Pause;
	Texture* m_Score;
	Texture* m_Win;
	bool m_Paused;
	bool m_GameWon;

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;
};