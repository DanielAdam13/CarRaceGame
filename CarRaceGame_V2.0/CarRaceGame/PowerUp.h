#pragma once
#include "PlayerCar.h"
class Texture;

class PowerUp
{
public:
	explicit PowerUp(int index, const Vector2f& pos, float size);

	void MovePowerUp(float elapsedSec, float& parallaxSpeed);
	void AffectPlayer(float elapsedSec, PlayerCar& player, float& parallaxSpeed);
	void Draw() const;

	Rectf GetBounds() const;
	std::string GetTitle() const;

	void AbsorbPowerUp();

	enum class State
	{
		moving,
		taken
	};

	PowerUp::State GetCurrentState() const;
	bool ShouldDelete() const;

	friend bool PlayerCar::AboutToEnd(const PowerUp* powerUp);

private:
	State m_CurrentState;

	const int m_PowerUpIndex;
	Vector2f m_Position;
	const float m_Size;
	Color4f m_Color;

	std::string m_Title;

	float m_DurAccSec;
	float m_PowerUpDuration;

	bool m_DeleteMarker;

	bool m_CanHeal;
};

