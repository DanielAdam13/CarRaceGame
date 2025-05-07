#pragma once
class Car
{
public:
	explicit Car(const Vector2f& pos, const float width, const float height);

	virtual void Draw() const;
	virtual void Update(float elapsedSec, float& parallaxSpeed);

	virtual Rectf GetBounds() const;
	virtual Rectf GetHitbox() const;

protected:
	
	Vector2f m_Position;
	float m_Speed;
	Color4f m_Color;

	const float m_Width;
	const float m_Height;
private:

};

