#pragma once
#include <vector>
class Car;
class PlayerCar;

class Lane
{
public:
	explicit Lane(const Vector2f& lanePosition, float carWidth, float carHeight, float carSpawnCooldown);
	~Lane();

	void Draw() const;
	void HandleCars(float elapsedSec, PlayerCar* player, float& parallaxSpeed);

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

