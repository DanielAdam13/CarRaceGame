#pragma once
#include <vector>
class Car;

class Lane
{
public:
	explicit Lane(const Vector2f& lanePosition, float carWidth, float carHeight, float carSpawnCooldown);
	~Lane();

	void Draw() const;
	void HandleCars(float elapsedSec);

	Vector2f GetDrawPosition() const;

private:
	const Vector2f m_LanePosition;
	std::vector<Car*> m_Cars;
	
	const float m_MinCarSpawnCooldown;

	const float m_CarWidth;
	const float m_CarHeight;

	float m_RandomSec;
	
	float m_AccuSec;

};

