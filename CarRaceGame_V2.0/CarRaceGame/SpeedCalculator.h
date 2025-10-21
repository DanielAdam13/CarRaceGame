#pragma once
class SpeedCalculator final
{
public:
	explicit SpeedCalculator();

	void Draw() const;
	void Update(float elapsedSec);

	int GetSpeed() const;

private:
	int m_Score;
	float m_AccuSec;
};

