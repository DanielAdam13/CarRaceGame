#pragma once
#include "BaseGame.h"
#include <vector>
#include "Lane.h"
#include "PlayerCar.h"
class Texture;
#include "ScoreCalculator.h"
#include "PowerUpManager.h"

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
	ScoreCalculator m_ScoreCalculator;
	
	std::vector<Lane> m_Lanes;
	PowerUpManager m_PowerUpManager;

	const int m_LaneNr;
	const float m_BorderLineOffset;
	const float m_LaneHeight;

	const Texture* m_PurpleHeart;

	const Texture* m_InvinicibilityText;
	const Texture* m_SmallStatusText;
	const Texture* m_LifeStealStatusText;
	const Texture* m_HealText;
	PlayerCar m_PlayerCar;

	std::vector<Rectf> m_SmallLines;

	float m_ParallaxSpeed;
	int m_SpeedKmh;

	Texture* m_Pause;
	Texture* m_Score;
	const Texture* m_Kmh;
	const Texture* m_DeathTexture;
	const Texture* m_PressRTexture;
	Texture* m_Win;
	const Texture* m_GoalTutorial;
	bool m_DrawGoalTutorial;

	const Texture* m_PausePowerTutorialTexture;
	
	const Texture* m_PauseTutorialText1;
	const Texture* m_TipText1;
	const Texture* m_TipText2;
	const Texture* m_PausePressRTo;
	const Texture* m_PauseRestart;
	
	bool m_Paused;
	bool m_GameWon;

	int m_ScoreTextSize;

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;

	void Restart();
};