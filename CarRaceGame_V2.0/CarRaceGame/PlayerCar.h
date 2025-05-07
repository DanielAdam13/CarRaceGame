#pragma once
#include "Car.h"

class PlayerCar final : public Car
{
public:
	explicit PlayerCar(const Vector2f& pos, const float width, const float height, const Color4f& color, float screenWidth, float screenHeight);

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, float& parallaxSpeed, const Uint8* pStates = nullptr, float bottomBorderY = 0.f, float topBorderY = 0.f);
	
	bool IsInvincible() const;
	void Hit();

	virtual Rectf GetHitbox() const;

private:
	enum class Movement {
		neutral,
		left,
		up,
		right,
		down
	};
	
	Color4f m_Color;
	float m_Speed;
	float m_Angle;
	bool m_Hit;
	float m_InvincibilityTimer;
	bool m_Invicibility;

	float m_ScreenWidth;
	float m_ScreenHeight;

	void SetBounds(float bottomBorderY, float topBorderY);
};

