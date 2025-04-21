#pragma once
#include "Car.h"

class PlayerCar final : public Car
{
public:
	explicit PlayerCar(const Vector2f& pos, const float width, const float height, const Color4f& color, float screenWidth, float screenHeight);

	virtual void Draw() const override;
	virtual void Update(const Uint8* pStates, float elapsedSec);

private:
	enum class Movement {
		neutral,
		left,
		up,
		right,
		down
	};
	
	Color4f m_Color;
	const float m_Speed;

	float m_ScreenWidth;
	float m_ScreenHeight;

	void SetBounds();
};

